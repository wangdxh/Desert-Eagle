// streamserver.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "flvbuffer.h"
#include "rtspto.h"
#include <iostream>
int _tmain(int argc, _TCHAR* argv[])
{	
    {
        char szinfo [128] = {0};
        strcpy(szinfo, "ffffffffffxxxxxxxxxx");
        szinfo[20] = 0;
        std::string strinfo = std::string(&szinfo[0], 20);
        int len = strinfo.length();

        std::string request_data = " OPTIONS    rtsp://192.168.1.188/h264/?deviceid=1234@kedacom.com&abc222=3333 RTSP/1.0 \r\n CSeq  :  2 \r\n User-Agent: LibVLC/2.1.2 (LIVE555 Streaming Media v2013.12.05) \r\n\r\n";
        std::map< std::string, std::string > mapitem;
       //get_all_options_from_text(request_data , mapitem);

		request_data = "rtsp://172.16/fdsaf/fasfs/?abc=we";
		std::string baseurl;
		baseurl = get_base_rtsp_url(request_data);
		std::cout << baseurl <<std::endl;
		request_data = "rtsp://172.16/fdsaf/fasfs/";
		baseurl = get_base_rtsp_url(request_data);
		std::cout << baseurl  <<std::endl;;
    }
	{
        boost::asio::mutable_buffer buftest2;
        int nsize2 = boost::asio::buffer_size(buftest2);
        const uint8_t* pbyte2 = boost::asio::buffer_cast<const uint8_t*>(buftest2);

		char szinfo[11] = {"1111111111"};
		boost::asio::mutable_buffer buftest(szinfo, strlen(szinfo));
		shared_const_buffer_flv tempbuf(buftest, shared_const_buffer_flv::em_http_flv);
        copyed_buffer buft = copyed_buffer(boost::asio::buffer(szinfo, strlen(szinfo)));
        copyed_buffer buft2 = buft;
        buft = copyed_buffer(boost::asio::buffer(szinfo, strlen(szinfo)));;
        bool b = buft.isnull();
        printf("%d buftis bool \r\n", b);
        {
            int nsize2 = boost::asio::buffer_size(buft.m_buffer);
            int nuse = buft2.m_streamdata.use_count();
            nuse = buft.m_streamdata.use_count();
            const uint8_t* pbyte2 = boost::asio::buffer_cast<const uint8_t*>(buft.m_buffer);
            bool b = buft.isnull();
            int x = 1;
        }
		
		const boost::asio::const_buffer* pbuffer = tempbuf.getstreamdata();
		int nsize = boost::asio::buffer_size(*pbuffer);
		char szChnukBuf[32];
		int nLen = sprintf(szChnukBuf, "%x\r\n", nsize);
		const uint8_t* pbyte = boost::asio::buffer_cast<const uint8_t*>(*pbuffer);
		if (0x17 == pbyte[0])
		{
		}

		char* szChunkEnd = "\r\n";

		tempbuf.setchunk(szChnukBuf, nLen, szChunkEnd, strlen(szChunkEnd));

		{
			shared_const_buffer_flv::const_iterator iter = tempbuf.begin();
			for(; iter != tempbuf.end(); iter++)
			{
				printf("%x  end %x\r\n", iter, tempbuf.end());
				printf("%d size\r\n", boost::asio::buffer_size(*iter));
			}
		}
	}
	boost::asio::io_service io_service;	
    tcp_server<stream_rtsp_to> server_rtsp_to(io_service, tcp::endpoint(tcp::v4(), 554));
	tcp_server<stream_httpflv_to> server_Httpflv_to(io_service, tcp::endpoint(tcp::v4(), 1984));
	tcp_server<stream_flv_from> server_flv_from(io_service, tcp::endpoint(tcp::v4(), 1985));
    
	io_service.run();

	return 0;
}

