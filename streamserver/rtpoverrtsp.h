#ifndef _WXH_RTP_OVER_RTSP_H_
#define _WXH_RTP_OVER_RTSP_H_

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

typedef struct {  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char NRI:2;  
    unsigned char F:1;      
} NALU_HEADER; /**//* 1 BYTES */  

typedef struct {  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char NRI:2;   
    unsigned char F:1;      
} FU_INDICATOR; /**//* 1 BYTES */  

typedef struct {  
    //byte 0  
    unsigned char TYPE:5;  
    unsigned char R:1;  
    unsigned char E:1;  
    unsigned char S:1;      
} FU_HEADER; /**//* 1 BYTES */  


void get_rtsp_rtp_video_total_len(const uint8_t* pbuffer, uint32_t dwbuflen, uint32_t& ntotallen, uint32_t& nrtpnums)
{

    ntotallen = 0;
    nrtpnums = 0;
    uint8_t* pdata = (uint8_t*)pbuffer+5;
    uint8_t* pend = (uint8_t*)pbuffer+dwbuflen-9;
    while (pdata < pend)
    {
        int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
        pdata += 4;

        if (nnallen <= MAX_RTP_PKT_LENGTH)
        {
            ntotallen += 12 + nnallen;
            nrtpnums++;
        }
        else
        {
            int k = 0, last = 0;  
            k = nnallen / MAX_RTP_PKT_LENGTH;//需要k个1400字节的RTP包，这里为什么不加1呢？因为是从0开始计数的。  
            last = nnallen% MAX_RTP_PKT_LENGTH;//最后一个RTP包的需要装载的字节数    
            nrtpnums += (k+1);
            ntotallen +=   (k+1)*14 + k*1400 + last -1;            
        }
        pdata += nnallen;
    }
    // 4个字节头，when rtp over rtsp
    ntotallen += 4*nrtpnums;
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
    pbuffer[2] = (dwlen >> 8)&0xff;
    pbuffer[3] = dwlen&0xff;
}

bool generate_rtp_info_over_rtsp(const uint8_t* pbuffer, uint32_t dwbuflen, 
                                 uint8_t* pdest, uint32_t dwdestlen,  uint32_t dwtimestample, uint32_t& seq_num)
{
    memset(pdest, 0, dwdestlen);

    uint8_t* pdata = (uint8_t*)pbuffer +5;
    uint8_t* pend = (uint8_t*)pbuffer+dwbuflen-9;
    uint32_t dwdestinx = 0;
    RTP_FIXED_HEADER* rtp_hdr; 
    NALU_HEADER* nalu_hdr;  
    FU_INDICATOR* fu_ind;  
    FU_HEADER* fu_hdr;  
    uint32_t dwssrc = 0x10;
    while (pdata < pend)
    {
        int nnallen = (pdata[0]<<24) | (pdata[1]<<16) | (pdata[2]<<8) | (pdata[3]);
        pdata += 4;

        uint8_t bynaltype = pdata[0];
        if (nnallen <= MAX_RTP_PKT_LENGTH)
        {
            set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, 12+nnallen);
            dwdestinx += 4;

            set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, true);
            //设置NALU HEADER,并将这个HEADER填入sendbuf[12]  
            nalu_hdr =(NALU_HEADER*)&pdest[dwdestinx+12]; //将sendbuf[12]的地址赋给nalu_hdr，之后对nalu_hdr的写入就将写入sendbuf中；          

            nalu_hdr->F = (bynaltype & 0x80)>>7;  
            nalu_hdr->NRI = (bynaltype & 0x60)>>5;  //有效数据在n->nal_reference_idc的第6，7位，需要右移5位才能将其值赋给nalu_hdr->NRI。  
            nalu_hdr->TYPE = (bynaltype & 0x1f) ;  


            memcpy(&pdest[dwdestinx+13], &pdata[1], nnallen-1);
            dwdestinx += 12 + nnallen;
        }
        else
        {
            //得到该nalu需要用多少长度为1400字节的RTP包来发送  
            int k = 0, last = 0;  
            k = nnallen / 1400;//需要k个1400字节的RTP包，这里为什么不加1呢？因为是从0开始计数的。  
            last = nnallen % 1400;//最后一个RTP包的需要装载的字节数
            int t = 0;//用于指示当前发送的是第几个分片RTP包


            while(t <= k)  
            {                
                if(!t)//发送一个需要分片的NALU的第一个分片，置FU HEADER的S位,t = 0时进入此逻辑。  
                {
                    set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, 14+1400);
                    dwdestinx += 4;

                    set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, false);

                    //设置FU INDICATOR,并将这个HEADER填入sendbuf[12]  
                    fu_ind =(FU_INDICATOR*)&pdest[dwdestinx+12]; //将sendbuf[12]的地址赋给fu_ind，之后对fu_ind的写入就将写入sendbuf中；  
                    fu_ind->F = (bynaltype & 0x80)>>7;
                    fu_ind->NRI = (bynaltype & 0x60)>>5;
                    fu_ind->TYPE = 28;  //FU-A类型。  

                    //设置FU HEADER,并将这个HEADER填入sendbuf[13]  
                    fu_hdr =(FU_HEADER*)&pdest[dwdestinx+13];  
                    fu_hdr->E = 0;  
                    fu_hdr->R = 0;  
                    fu_hdr->S = 1;  
                    fu_hdr->TYPE = (bynaltype & 0x1f);  

                    memcpy(&pdest[dwdestinx+14], &pdata[1], 1400);
                    dwdestinx += 14 + 1400;

                    t++;  
                }  
                //发送一个需要分片的NALU的非第一个分片，清零FU HEADER的S位，如果该分片是该NALU的最后一个分片，置FU HEADER的E位  
                else if(k == t)//发送的是最后一个分片，注意最后一个分片的长度可能超过1400字节（当 l> 1386时）。  
                {
                    set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, (14 + last-1));
                    dwdestinx += 4;

                    //设置rtp M 位；当前传输的是最后一个分片时该位置1  
                    set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, true);

                    //设置FU INDICATOR,并将这个HEADER填入sendbuf[12]  
                    fu_ind =(FU_INDICATOR*)&pdest[dwdestinx+12]; 
                    fu_ind->F= (bynaltype & 0x80)>>7;
                    fu_ind->NRI=(bynaltype & 0x60)>>5;
                    fu_ind->TYPE=28;  

                    //设置FU HEADER,并将这个HEADER填入sendbuf[13]  
                    fu_hdr = (FU_HEADER*)&pdest[dwdestinx+13];  
                    fu_hdr->R = 0;  
                    fu_hdr->S = 0;  
                    fu_hdr->TYPE = (bynaltype & 0x1f); 
                    fu_hdr->E = 1;  

                    //将nalu最后剩余的l-1(去掉了一个字节的NALU头)字节内容写入sendbuf[14]开始的字符串。  
                    memcpy(&pdest[dwdestinx+14], &pdata[ t*1400 + 1], last-1);                                        
                    dwdestinx += 14 + last-1;

                    t++;                  
                }  
                //既不是第一个分片，也不是最后一个分片的处理。  
                else if(t < k && 0 != t)  
                {  
                    set_buf_rtp_over_rtsp_tag(&pdest[dwdestinx], 0, 14 + 1400);
                    dwdestinx += 4;

                    set_buf_rtp_video_header(&pdest[dwdestinx], dwssrc, dwtimestample, seq_num++, false);
                    //设置FU INDICATOR,并将这个HEADER填入sendbuf[12]  
                    fu_ind = (FU_INDICATOR*)&pdest[dwdestinx+12]; //将sendbuf[12]的地址赋给fu_ind，之后对fu_ind的写入就将写入sendbuf中；  
                    fu_ind->F = (bynaltype & 0x80)>>7;
                    fu_ind->NRI = (bynaltype & 0x60)>>5;
                    fu_ind->TYPE = 28;  

                    //设置FU HEADER,并将这个HEADER填入sendbuf[13]  
                    fu_hdr =(FU_HEADER*)&pdest[dwdestinx+13];  
                    fu_hdr->R = 0;  
                    fu_hdr->S = 0;  
                    fu_hdr->E = 0;  
                    fu_hdr->TYPE = (bynaltype & 0x1f); 

                    memcpy(&pdest[dwdestinx+14], &pdata[t * 1400 + 1], 1400);//去掉起始前缀的nalu剩余内容写入sendbuf[14]开始的字符串。  
                    dwdestinx += 14 + 1400;
                    t++;  
                }  
            }  
        }

        pdata += nnallen;
    }
    return dwdestlen == dwdestinx;
}

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>  
#include <iostream>
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
    for(auto& mapitems:mapitems)
    {
        std::cout << mapitems.first << ":" << mapitems.second << "*"<< std::endl;
    }
}

#endif