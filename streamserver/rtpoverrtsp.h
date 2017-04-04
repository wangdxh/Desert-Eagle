#ifndef _WXH_RTP_OVER_RTSP_H_
#define _WXH_RTP_OVER_RTSP_H_

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>  
#include <iostream>
#include <stdint.h>

// h264 to rtp
#define MAX_RTP_PKT_LENGTH     1400  
#define PAYLOAD_TYPE_H264                    96 

typedef struct   
{  
    /**//* byte 0 */  
    unsigned char csrc_len:4;        /**//* expect 0 */  
    unsigned char extension:1;        /**//* expect 1, see RTP_OP below */  
    unsigned char padding:1;        /**//* expect 0 */  
    unsigned char version:2;        /**//* expect 2 */  
    /**//* byte 1 */  
    unsigned char payload:7;        /**//* RTP_PAYLOAD_RTSP */  
    unsigned char marker:1;        /**//* expect 1 */  
    /**//* bytes 2, 3 */  
    unsigned short seq_no;
    /**//* bytes 4-7 */  
    unsigned  long timestamp;          
    /**//* bytes 8-11 */  
    unsigned long ssrc;            /**//* stream number is used here. */  
} RTP_FIXED_HEADER;  
 


void get_rtsp_rtp_video_total_len(const uint8_t* pbuffer, uint32_t dwbuflen, uint32_t& ntotallen, uint32_t& numbernalus)
{
	// last nalu marker is 1, return number of nalus
    ntotallen = 0;
    numbernalus = 0;
    uint8_t* pdata = (uint8_t*)pbuffer+5; // first 5 byte is the flv video prefix
    uint8_t* pend = (uint8_t*)pbuffer+dwbuflen-4; // the last 4 bytes is the flv last tag len
    while (pdata < pend)
    {
        int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
        pdata += 4;

        if (nnallen <= MAX_RTP_PKT_LENGTH)
        {
            ntotallen += 4 + 12 + nnallen;         
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

void set_buf_rtp_video_header(uint8_t* pbuffer, uint32_t dwssrc, uint32_t dwtimestample, uint32_t dwseqnum, bool marker)
{
    RTP_FIXED_HEADER* rtp_hdr = (RTP_FIXED_HEADER*)pbuffer;
    rtp_hdr->payload     = PAYLOAD_TYPE_H264;  //负载类型号，  
    rtp_hdr->version     = 2;  //版本号，此版本固定为2              
    rtp_hdr->ssrc        = htonl(dwssrc);  
    rtp_hdr->timestamp=htonl(dwtimestample);
    //设置rtp M 位；  
    rtp_hdr->marker = marker;  
    rtp_hdr->seq_no  = htons(dwseqnum); //序列号，每发送一个RTP包增1，htons，将主机字节序转成网络字节序。  
}
void set_buf_rtp_over_rtsp_tag(uint8_t* pbuffer, uint8_t bychannel, uint16_t dwlen)
{
    pbuffer[0] = 0x24;
    pbuffer[1] = bychannel;
    pbuffer[2] = (dwlen >> 8) & 0xff;
    pbuffer[3] = dwlen & 0xff;
}

bool generate_rtp_info_over_rtsp(const uint8_t* pbuffer, uint32_t dwbuflen, 
                                 uint8_t* pdest, uint32_t dwdestlen, uint32_t dwnumnalus, uint32_t dwtimestample, uint32_t& seq_num)
{
    memset(pdest, 0, dwdestlen);
    uint8_t* pdata = (uint8_t*)pbuffer +5;
    uint8_t* pend = (uint8_t*)pbuffer+dwbuflen-4;
    uint32_t dwdestinx = 0;
    
    uint32_t dwssrc = 0x10;

	bool bmarker = false;
    for (int inx = 0; inx < dwnumnalus; inx++)
    {
		if (inx == dwnumnalus-1) { bmarker = true;}

        int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
        pdata += 4;

        uint8_t bynaltype = pdata[0];
        if (nnallen <= MAX_RTP_PKT_LENGTH)
        {
            set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, 12+nnallen);
            dwdestinx += 4;

            set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, bmarker);
            dwdestinx += 12;

            memcpy(&pdest[dwdestinx], pdata, nnallen);
            dwdestinx += nnallen;
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
					npacketsize = (nnallen - 1) % MAX_RTP_PKT_LENGTH;
					blastfu = true;
				}

				set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, npacketsize);
				dwdestinx += 4;

				set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, bmarker&blastfu);
				dwdestinx += 12;

				pdest[dwdestinx++] = (bynaltype & 0xe0) | 28;

				if (inxpack == 0)
				{
					pdest[dwdestinx++] = (bynaltype & 0x1f) | 0x80;//S
				}
				// last pack
				else if (inxpack == npacket - 1)
				{
					pdest[dwdestinx++] = (bynaltype & 0x1f) | 0x40;//E
				}
				else
				{
					pdest[dwdestinx++] = bynaltype & 0x1f;
				}
				memcpy(&pdest[dwdestinx], psrcnalu, npacketsize);
				dwdestinx += npacketsize;
			}  
        }

        pdata += nnallen;
    }
	if (pdata != pend || dwdestinx != dwdestlen)
	{
		printf("**************************************************error generate rtp total len wrong\r\n");
	}
    
    return dwdestlen == dwdestinx;
}



std::string get_base_rtsp_url(std::string& strurl)
{
	std::string baseurl;
	std::vector<std::string> item;
	boost::split_regex( item, strurl, boost::regex( "\\?" ) );             
	if (item.size() > 2)
	{
		return strurl;
	}
	baseurl = item[0];
	if (baseurl.back() != '/')
	{
		baseurl += '/';
	}
	return baseurl;
}
int get_url_track_num(std::string& strual)
{
	int track = 0;
	if (strual.back() == '1')
	{
		track = 1;
	}
	else if (strual.back() == '2')
	{
		track = 2;
	}
	return track;
}

// all the queryid and the option will in map ,  the request operator is get by 'command'
bool get_all_options_from_text(const std::string& strrequest, std::map< std::string, std::string >& mapitems)
{
    mapitems.clear();
    std::vector<std::string> veclines;
    boost::split_regex( veclines, strrequest, boost::regex( "\r\n" ) ); 
    bool byfirst = false;
    for(auto& strline : veclines)
    {
        if (strline.empty()) {continue;}

        if (false == byfirst)
        {
            byfirst = true;
            std::vector<std::string> item;
            boost::trim(strline);
            boost::split_regex( item, strline, boost::regex( "[ ]+" ) );             
            if (item.size() != 3)
            {
                return false;
            }
            mapitems["methond"] = item[0];
            mapitems["url"] = item[1];
			mapitems["baseurl"] = get_base_rtsp_url(item[1]);
            mapitems["protocol"] = item[2];
            
            boost::regex expression("([0-9a-zA-Z@.]+)=([0-9a-zA-Z@.]+)"); 
            boost::smatch what;
            std::string::const_iterator start = item[1].begin();
            std::string::const_iterator end = item[1].end();
            while( boost::regex_search(start, end, what, expression) )  
            {
                mapitems[what[1].str()] = what[2].str();
                start = what[0].second;
            }
        }
        else
        {
            std::vector<std::string> item;
            boost::trim(strline);
            boost::split_regex( item, strline, boost::regex( ":" ) ); 
            if (item.size() != 2)
            {
                return false;
            } 
            else
            {
                boost::trim(item[0]);
                boost::trim(item[1]);
                mapitems[item[0]] = item[1];
            }
        }
    }
	std::cout << "-----------------------------------------------------------" << std::endl;
    for(auto& mapitems:mapitems)
    {		
        std::cout << mapitems.first << ":" << mapitems.second << std::endl;
    }
	return true;
}

#define MAX_RTSP_TCP_BUFFER_LEN_  32*1024
bool get_rtsp_message_from_buffer(boost::asio::streambuf& readstreambuf, std::string& strmessage)
{
    /*strmessage.clear();
    boost::asio::const_buffer buffer = readstreambuf.data();
    const char* pdatabuf = boost::asio::buffer_cast<char*>(buffer);
    int nbuflen = boost::asio::buffer_size(buffer);*/

    boost::asio::streambuf::const_buffers_type cbt = readstreambuf.data(); 
    std::string request_data(boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt)); 
    int nbuflen = request_data.length(); 

    //should not be this, but from the buffer alloc this maybe right
        
    char* ptempbuffer = (char*)request_data.c_str();
    int nconsume = 0;
    
    while (nconsume < nbuflen)
    {
        if ('$' == ptempbuffer[nconsume])
        {
            int nleft = nbuflen-nconsume;
            if ( nleft < 4) break;
            
            uint8_t byhigh = ptempbuffer[nconsume+2];
            uint8_t bylow = ptempbuffer[nconsume+3];
            int nrtplen = byhigh << 8 | bylow;

            if(nleft < nrtplen+4) break;
            nconsume += nrtplen+4;
        }
        else
        {
            const char* pend = strstr(&ptempbuffer[nconsume], "\r\n\r\n");
            if (pend)
            {
                int nmsglen = pend - (&ptempbuffer[nconsume]) + 4;
                strmessage += std::string(&ptempbuffer[nconsume], nmsglen);
                nconsume += nmsglen;
            }
            else
            {
                break;
            }            
        }
    }
    if (nconsume > 0) readstreambuf.consume(nconsume);    
    if (strmessage.length() > 0) return true;
    
    if (0 == nconsume && nbuflen > MAX_RTSP_TCP_BUFFER_LEN_)
    {
        std::cout << "the buffer is bigger than 32k, still no rtsp or rtp get, whar r u doing, clear all xxxxxx\r\n";
        readstreambuf.consume(nbuflen);        
    }    
    return false;
}

#endif