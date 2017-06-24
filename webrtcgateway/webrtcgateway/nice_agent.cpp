#include "stdafx.h"
#include "nice_agent.h"
#include "dtls_srtp.h"
#include "rtp_rtcp.h"

static const gchar *candidate_type_name[] = {"host", "srflx", "prflx", "relay"};

void cb_nice_recv(NiceAgent *agent, guint stream_id, guint component_id, guint len, gchar *buf, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->nice_recv_data(stream_id, component_id, len, buf);    
}

void cb_candidate_gathering_done(NiceAgent *agent, guint stream_id, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->candidate_gathering_done(stream_id);
}
void cb_component_state_changed(NiceAgent *agent, guint stream_id, guint component_id, guint state, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->component_state_changed(stream_id, component_id, state);
}

void cb_new_selected_pair_full(NiceAgent* agent, guint stream_id,guint component_id, NiceCandidate *lcandidate, NiceCandidate* rcandidate, gpointer data)
{
    nice_agent* pAgent = (nice_agent*)data;
    pAgent->new_selected_pair_full(stream_id, component_id, lcandidate, rcandidate);
}

GMainLoop* nice_agent::gloop = NULL;
GThread* nice_agent::gloopthread = NULL;
void nice_agent::init()
{
    /*dtls_srtp::init("D:\\github\\Desert-Eagle\\webrtcgateway\\bin\\file\\certs\\mycert.pem",
                        "D:\\github\\Desert-Eagle\\webrtcgateway\\bin\\file\\certs\\mycert.key");*/
    dtls_srtp::init(NULL, NULL);

    g_networking_init();
    gloop = g_main_loop_new(NULL, FALSE);
    gloopthread = g_thread_new("loop thread", &loop_thread, gloop);    
}
void nice_agent::release()
{
    g_main_loop_quit (gloop);
    g_thread_join (gloopthread);
    g_main_loop_unref(gloop);
    dtls_srtp::release();
}
void* nice_agent::loop_thread(void *data)
{
    GMainLoop* ploop = (GMainLoop*)data;
    std::cout << "loop thread going..." << std::endl;
    g_main_loop_run(ploop);
    std::cout << "loop thread quit..." << std::endl;
    return 0;
}



nice_agent::nice_agent(websocket_server* pserver, connection_hdl hdl, gboolean controlling, gchar* stun_addr , guint stun_port)
{
    pserver_ = pserver;
    hdl_ = hdl;
    agent = nullptr;        
    agent = nice_agent_new(g_main_loop_get_context (gloop), NICE_COMPATIBILITY_RFC5245);
    thread_audio_ = nullptr;

    if (stun_addr)
    {
        g_object_set(agent, "stun-server", stun_addr, NULL);
        g_object_set(agent, "stun-server-port", stun_port, NULL);
    }
    g_object_set(agent, "upnp", false, NULL);
    g_object_set(agent, "ice-tcp", false, NULL);
    g_object_set(agent, "controlling-mode", controlling, NULL);        

    websocket_server::connection_ptr conn = pserver->get_con_from_hdl(hdl);
    std::string strhost = conn->get_host();
    

    NiceAddress addr_local;
    nice_address_init (&addr_local);
    nice_address_set_from_string (&addr_local, strhost.c_str());
    nice_agent_add_local_address (agent, &addr_local);

    g_signal_connect(agent, "candidate-gathering-done", G_CALLBACK(cb_candidate_gathering_done), this);
    g_signal_connect(agent, "component-state-changed", G_CALLBACK(cb_component_state_changed), this);
    g_signal_connect (agent, "new-selected-pair-full",G_CALLBACK (cb_new_selected_pair_full), this);
    dtls_ = new dtls_srtp;
}
nice_agent::~nice_agent()
{
    if (agent)
    {
        if (thread_audio_)
        {
            thread_audio_->interrupt();
            thread_audio_->join();
            delete thread_audio_;
            thread_audio_ = nullptr;
        }
        for(auto iter :mapstream_componet )
        {
             uint32_t ncomponet = iter.second;
            for (int inx = 1; inx <= ncomponet; inx++)
            {
                nice_agent_attach_recv(agent, iter.first, inx, g_main_loop_get_context (gloop), NULL, NULL);
            }
        }        

        g_object_unref(agent);            
    }         
    if (dtls_)
    {
        delete dtls_;
    }
}
int32_t nice_agent::add_stream(char* szstreamname, uint32_t componentnum)
{
    guint stream_id = nice_agent_add_stream(agent, componentnum);
    if (stream_id == 0)
    {
        std::cout << "Failed to add stream" << std::endl;
    }
    else
    {
        nice_agent_set_stream_name (agent, stream_id, szstreamname);
        mapstream_componet[stream_id] = componentnum;
    }
    dtls_->create(this, 1, 1, DTLS_ROLE_SERVER);
    return stream_id;
}
bool nice_agent::start_gather(int32_t streamid)
{
    // Attach to the component to receive the data
    // Without this call, candidates cannot be gathered
    std::map < int32_t, uint32_t>::iterator iter = mapstream_componet.find(streamid);
    if (mapstream_componet.end() == iter)
    {
        return false;
    }
    uint32_t ncomponet = iter->second;
    for (int inx = 1; inx <= ncomponet; inx++)
    {
        nice_agent_attach_recv(agent, streamid, inx, g_main_loop_get_context (gloop), cb_nice_recv, this);
    }
    if (!nice_agent_gather_candidates(agent, streamid))
    {
        g_error("Failed to start candidate gathering");
        return false;
    }
    return true;
}
void nice_agent::candidate_gathering_done(int32_t stream_id)
{
    /*gchar* sdp = nice_agent_generate_local_sdp (agent);
    printf("Generated SDP from agent :\n%s\n\n", sdp);
    printf("Copy the following line to remote client:\n");
    gchar* sdp64 = g_base64_encode ((const guchar *)sdp, strlen (sdp));
    printf("\n  %s\n", sdp64);
    g_free (sdp);
    g_free (sdp64);*/

    gchar *local_ufrag = NULL;
    gchar *local_password = NULL;
    gchar ipaddr[INET6_ADDRSTRLEN];
    GSList *cands = NULL;


    if (!nice_agent_get_local_credentials(agent, stream_id,&local_ufrag, &local_password)) {return;}

    cands = nice_agent_get_local_candidates(agent, stream_id, 1);
    printf("%s %s", local_ufrag, local_password);

    NiceCandidate *c = (NiceCandidate *)g_slist_nth(cands, 0)->data;

    nice_address_to_string(&c->addr, ipaddr);

    // (foundation),(prio),(addr),(port),(type)
    printf(" %s,%u,%s,%u,%s\r\n",
        c->foundation,
        c->priority,
        ipaddr,
        nice_address_get_port(&c->addr),
        candidate_type_name[c->type]);


    int nssrc = 12345678;
    char szsdp[1024*10] = {0}; 
    if (1)
    {
        sprintf(szsdp, "v=0\r\no=- 1495799811084970 1495799811084970 IN IP4 %s\r\ns=Streaming Test\r\nt=0 0\r\n"
            "a=group:BUNDLE video\r\na=msid-semantic: WMS janus\r\n"
            "m=video 1 RTP/SAVPF 96\r\nc=IN IP4 %s\r\na=mid:video\r\na=sendonly\r\na=rtcp-mux\r\n"
            "a=ice-ufrag:%s\r\n"
            "a=ice-pwd:%s\r\na=ice-options:trickle\r\na=fingerprint:sha-256 %s\r\na=setup:actpass\r\na=connection:new\r\n"
            "a=rtpmap:96 H264/90000\r\n"
            "a=ssrc:%d cname:janusvideo\r\n"
            "a=ssrc:%d msid:janus janusv0\r\n"
            "a=ssrc:%d mslabel:janus\r\n"
            "a=ssrc:%d label:janusv0\r\n"
            "a=candidate:%s 1 udp %u %s %d typ %s\r\n", 
            ipaddr, ipaddr,
            local_ufrag, local_password,dtls_srtp::get_local_fingerprint(),
            nssrc, nssrc, nssrc, nssrc,
            c->foundation, c->priority, ipaddr, nice_address_get_port(&c->addr),
            candidate_type_name[c->type]
        );
    }
    else
    {
        sprintf(szsdp, "v=0\r\no=- 1495799811084970 1495799811084970 IN IP4 %s\r\ns=Streaming Test\r\nt=0 0\r\na=group:BUNDLE audio\r\na=msid-semantic: WMS janus\r\nm=audio 1 RTP/SAVPF 0\r\nc=IN IP4 %s\r\na=mid:audio\r\na=sendonly\r\na=rtcp-mux\r\n"
            "a=ice-ufrag:%s\r\n"
            "a=ice-pwd:%s\r\na=ice-options:trickle\r\na=fingerprint:sha-256 %s\r\na=setup:actpass\r\na=connection:new\r\na=rtpmap:0 PCMU/8000\r\n"
            "a=ssrc:%d cname:janusaudio\r\n"
            "a=ssrc:%d msid:janus janusa0\r\n"
            "a=ssrc:%d mslabel:janus\r\n"
            "a=ssrc:%d label:janusa0\r\n"
            "a=candidate:%s 1 udp %u %s %d typ %s\r\n", 
            ipaddr, ipaddr,
            local_ufrag, local_password,dtls_srtp::get_local_fingerprint(),
            nssrc, nssrc, nssrc, nssrc,
            c->foundation, c->priority, ipaddr, nice_address_get_port(&c->addr),
            candidate_type_name[c->type]
        );
    }
    pserver_->send(hdl_, szsdp, websocketpp::frame::opcode::text);
    printf("\r\n%s\r\n", szsdp);

    if (local_ufrag)
        g_free(local_ufrag);
    if (local_password)
        g_free(local_password);
    if (cands)
        g_slist_free_full(cands, (GDestroyNotify)&nice_candidate_free);
}
bool nice_agent::set_remote_sdp(const char* sdp)
{
    remote_sdp = sdp;                
    return true;
}
bool nice_agent::set_remote_candidate(const char* szcandidate)
{
    std::string szsdp = remote_sdp +"a="+ szcandidate + "\r\n";
    gchar* ufrag = NULL;
    gchar* pwd = NULL;
    GSList * plist = nice_agent_parse_remote_stream_sdp (agent, 1, szsdp.c_str(), &ufrag, &pwd);
    if (ufrag && pwd && g_slist_length(plist) > 0)
    {
        ufrag[strlen(ufrag)-1] = 0;
        pwd[strlen(pwd)-1] = 0;

        NiceCandidate* c = (NiceCandidate*)g_slist_nth(plist, 0)->data; 

        if (!nice_agent_set_remote_credentials(agent, 1, ufrag, pwd)) 
        {
            g_message("failed to set remote credentials");
            return false;
        }

        // Note: this will trigger the start of negotiation.

        if (nice_agent_set_remote_candidates(agent, 1, 1, plist) < 1) 
        {
            g_message("failed to set remote candidates");
            return false;
        }
        g_free(ufrag);
        g_free(pwd);
        //g_slist_free(plist);
        g_slist_free_full(plist, (GDestroyNotify)&nice_candidate_free);
        return true;
    }
    return false;
}
void nice_agent::component_state_changed(int32_t streamid, uint32_t componentid, guint state)
{
    std::cout << this <<" state changed " << streamid << ":"<< componentid << " " << nice_component_state_to_string((NiceComponentState )state) 
        << state << std::endl;
    if (state == NICE_COMPONENT_STATE_READY) 
    {

    }
}

void nice_agent::new_selected_pair_full(guint stream_id,guint component_id, NiceCandidate *lcandidate, NiceCandidate* rcandidate)
{
    std::cout << this << "Component is ready enough, starting DTLS handshake...\n";
    dtls_->handshake();
}

void read_send_audio(void* pdata);
void read_send_video(void* pdata);

void nice_agent::nice_recv_data(int32_t streamid, uint32_t componentid, guint len, gchar *buf)
{
    std::cout << this << " recv data from stream: " << streamid << " componetid: " << componentid << " len: " << len << "first data :"<< (int)buf[0] <<std::endl;
    
    
    int buflen = len;
    if (is_dtls(buf))
    {
        dtls_->incoming_msg(buf, len);
    }
    else if (is_rtp(buf))
    {        
        this->dtls_->srtp_unprotect_rtp_buf(buf, &buflen);   
    }
    else if (is_rtcp(buf))
    {        
        this->dtls_->srtp_unprotect_rtcp_buf(buf, &buflen);
        
        if (nullptr == thread_audio_)
        {
            thread_audio_ = new boost::thread(boost::bind(read_send_video, this));            
        }
    }    
}

bool nice_agent::send_data(int32_t streamid, uint32_t componentid, guint len, gchar *buf)
{
    int nret = nice_agent_send(this->agent, streamid, componentid, len, buf);
    return nret;
}

bool nice_agent::send_data_need_protect(int32_t streamid, uint32_t componentid, guint len, gchar *buf)
{
    int protectedlen = len;
    
    int res = srtp_protect(dtls_->srtp_out, buf, &protectedlen);
    //~ JANUS_LOG(LOG_VERB, "[%I64u] ... SRTP protect %s (len=%d-->%d)...\n", handle->handle_id, janus_get_srtp_error(res), pkt->length, protectedlen);
    if(res != err_status_ok) 
    {
        rtp_header *header = (rtp_header *)buf;
        guint32 timestamp = ntohl(header->timestamp);
        guint16 seq = ntohs(header->seq_number);
        printf("SRTP protect error... %d (len=%d-->%d, ts=%d, seq=%d)\n", res, len, protectedlen, timestamp, seq);
        return false;
    }
    else 
    {
        /* Shoot! */
        //~ JANUS_LOG(LOG_VERB, "[%I64u] ... Sending SRTP packet (pt=%u, ssrc=%u, seq=%u, ts=%u)...\n", handle->handle_id,
        //~ header->type, ntohl(header->ssrc), ntohs(header->seq_number), ntohl(header->timestamp));
        int sent = nice_agent_send(agent, streamid, componentid, protectedlen, buf);
        if(sent < protectedlen) 
        {
            printf("only sent %d bytes? (was %d)\n", sent, protectedlen);
        }
    }
    return TRUE;
}

void read_send_audio(void* pdata)
{
    nice_agent* pagent = (nice_agent*)pdata;
    // ./file/music.mulaw payload is 0
    char readbuf[1024] = {0};
    gint16 seq = 1;
    gint32 ts = 0;
    rtp_header *header = (rtp_header *)readbuf;
    header->version = 2;
    header->markerbit = 1;
    header->type = 0;
    header->seq_number = htons(seq);
    header->timestamp = htonl(ts);
    header->ssrc = htonl(12345678);	/* The gateway will fix this anyway */

    char filename[MAX_PATH] = {0};
    strcpy(filename, "D:\\github\\Desert-Eagle\\webrtcgateway\\bin\\file\\music.mulaw");
    FILE *audio = fopen(filename, "rb");
    if (!audio)
    {
        printf("[%s] Ooops, audio file missing!\n", filename);
        return;
    }
    else
    {
        printf("[%s] open file ok!\n", filename);
    }
    try
    {
        while (true)
        {        
            Sleep(18);
            boost::this_thread::interruption_point();

            int read = fread(readbuf + RTP_HEADER_SIZE, sizeof(char), 160, audio);
            if(feof(audio)) 
            {
                printf("file is over\r\n");
                break;
            }
            if(read < 0)
                break;

            pagent->send_data_need_protect(1, 1, read + RTP_HEADER_SIZE, readbuf);

            seq++;
            header->seq_number = htons(seq);
            ts += 160;
            header->timestamp = htonl(ts);
            header->markerbit = 0;
        }
    }
    catch (...)
    {
         std::cout << "Interrupt exception was thrown." << std::endl;   
    }
    fclose(audio);
}


void set_buf_rtp_video_header(uint8_t* pbuffer, uint32_t dwssrc, uint32_t dwtimestample, uint16_t dwseqnum, bool marker)
{
    rtp_header* rtp_hdr = (rtp_header*)pbuffer;
    rtp_hdr->type     = 96;  //负载类型号，  
    rtp_hdr->version     = 2;  //版本号，此版本固定为2              
    rtp_hdr->ssrc        = htonl(dwssrc);  
    rtp_hdr->timestamp=htonl(dwtimestample);
    //设置rtp M 位；  
    rtp_hdr->markerbit = marker;      
    rtp_hdr->seq_number = htons(dwseqnum); //序列号，每发送一个RTP包增1，htons，将主机字节序转成网络字节序。  
}

#define MAX_RTP_PKT_LENGTH     1400  

void get_rtsp_rtp_video_total_len(const uint8_t* pbuffer, uint32_t dwbuflen, uint32_t& ntotallen, uint32_t& numbernalus)
{
    // last nalu marker is 1, return number of nalus
    ntotallen = 0;
    numbernalus = 0;
    uint8_t* pdata = (uint8_t*)pbuffer; // first 5 byte is the flv video prefix
    uint8_t* pend = (uint8_t*)pbuffer+dwbuflen; // the last 4 bytes is the flv last tag len
    while (pdata < pend)
    {
        int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
        pdata += 4;

        if ((nnallen-1) <= MAX_RTP_PKT_LENGTH)
        {
            ntotallen += 4 + 12 + nnallen;        //may be max_len + 1 
        }
        else
        {
            int npacket = (nnallen - 1 + MAX_RTP_PKT_LENGTH - 1) / MAX_RTP_PKT_LENGTH;
            ntotallen += npacket * (4 + 2 + 12) + nnallen - 1;         
        }
        pdata += nnallen;
        numbernalus += 1;
    }
    // 4个字节头，when rtp over rtsp
    if (pdata != pend)
    {
        printf("**************************************************error rtp total len wrong\r\n");
    }
}

#include "../streampushclient/h264frame.h"
void read_send_video(void* pdata)
{
    nice_agent* pagent = (nice_agent*)pdata;
    // ./file/music.mulaw payload is 0
    char rtpbuffer[8*1024] = {0};
    gint16 seq = 1;
    gint32 ts = 0;

    rtp_header *header = (rtp_header *)rtpbuffer;
    header->version = 2;
    header->markerbit = 0;
    header->type = 96;
    header->seq_number = htons(seq);
    header->timestamp = htonl(ts);
    header->ssrc = htonl(12345678);

    char filename[MAX_PATH] = {0};
    //strcpy(filename, "D:\\github\\Desert-Eagle\\streampushclient\\testokmy.h264");
    strcpy(filename, "..\\..\\streampushclient\\testokmy.h264");
    FILE *video = fopen(filename, "rb");
    if (!video)
    {
        printf("[%s] Ooops, video file missing!\n", filename);
        return;
    }
    else
    {
        printf("[%s] open file ok!\n", filename);
    }

    uint32_t dwLen = 0;	
    uint8_t* buffer = new uint8_t[1*1024*1024];
    uint32_t dwrealbufLen = 0;	
    uint8_t* realbuffer = new uint8_t[1*1024*1024];
    try
    {
        while (true)
        {
            Sleep(35);
            boost::this_thread::interruption_point();


            int nRet = fread(&dwLen, 1, 4, video);
            if (4 != nRet) 
            {
                break;
            }

            nRet = fread(buffer, 1, dwLen, video);
            if (nRet != dwLen)
            {
                break;	
            }
            
            if(feof(video)) 
            {
                printf("file is over\r\n");
                break;
            }            

            H264Frame tframe(buffer, dwLen, 0);
            dwrealbufLen = tframe.GetTotalFrameSize();
            tframe.WriteFrameToBuffer(realbuffer);

            uint32_t dwtotallen = 0;
            uint32_t dwnumnalus = 0;
            get_rtsp_rtp_video_total_len(realbuffer, dwrealbufLen, dwtotallen, dwnumnalus);



            uint8_t* pdata = (uint8_t*)realbuffer;
            uint8_t* pend = (uint8_t*)realbuffer+dwrealbufLen;
             
            bool bmarker = false;
            for (int inx = 0; inx < dwnumnalus; inx++)
            {
                if (inx == dwnumnalus-1) { bmarker = true;}

                int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
                pdata += 4;

                uint8_t bynaltype = pdata[0];
                if ((nnallen-1) <= MAX_RTP_PKT_LENGTH)
                {
                    memset(rtpbuffer, 0, sizeof(rtpbuffer));
                    set_buf_rtp_video_header((uint8_t*)rtpbuffer, 12345678, ts, seq++, bmarker);
                    memcpy(&rtpbuffer[12], pdata, nnallen);
                    pagent->send_data_need_protect(1, 1, 12+nnallen, rtpbuffer);
                }
                else
                {
                    bool blastfu = false;
                    int npacket = (nnallen - 1 + MAX_RTP_PKT_LENGTH - 1) / MAX_RTP_PKT_LENGTH;
                    for(int inxpack = 0; inxpack < npacket; inxpack++)
                    {
                        int npacketsize = MAX_RTP_PKT_LENGTH;
                        uint8_t* psrcnalu = pdata + 1 + inxpack * MAX_RTP_PKT_LENGTH;
                        if (inxpack == npacket-1)
                        {
                            npacketsize = (nnallen - 1) - inxpack * MAX_RTP_PKT_LENGTH; // 2801 nallen  
                            blastfu = true;
                        }
                        memset(rtpbuffer, 0, sizeof(rtpbuffer));

                        set_buf_rtp_video_header((uint8_t*)rtpbuffer, 12345678, ts, seq++, bmarker&blastfu);
                        int dwdestinx = 0;
                        dwdestinx += 12;

                        rtpbuffer[dwdestinx++] = (bynaltype & 0xe0) | 28;

                        if (inxpack == 0)
                        {
                            rtpbuffer[dwdestinx++] = (bynaltype & 0x1f) | 0x80;//S
                        }
                        // last pack
                        else if (inxpack == npacket - 1)
                        {
                            rtpbuffer[dwdestinx++] = (bynaltype & 0x1f) | 0x40;//E
                        }
                        else
                        {
                            rtpbuffer[dwdestinx++] = bynaltype & 0x1f;
                        }
                        memcpy(&rtpbuffer[dwdestinx], psrcnalu, npacketsize);
                        pagent->send_data_need_protect(1, 1, dwdestinx+npacketsize, rtpbuffer);
                        dwdestinx += npacketsize;
                    }  
                }

                pdata += nnallen;
            }

            ts += 40*90;
        }
    }
    catch (...)
    {
        std::cout << "Interrupt exception was thrown." << std::endl;   
    }
    fclose(video);
    delete [] buffer;
    delete [] realbuffer;
}

