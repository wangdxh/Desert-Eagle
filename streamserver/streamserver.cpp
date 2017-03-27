// streamserver.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "flvbuffer.h"


int _tmain(int argc, _TCHAR* argv[])
{	
	{
        boost::asio::mutable_buffer buftest2;
        int nsize2 = boost::asio::buffer_size(buftest2);
        const uint8_t* pbyte2 = boost::asio::buffer_cast<const uint8_t*>(buftest2);

		char szinfo[11] = {"1111111111"};
		boost::asio::mutable_buffer buftest(szinfo, strlen(szinfo));
		shared_const_buffer_flv tempbuf(buftest);
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
	tcp_server<stream_httpflv_to> server_Httpflv_to(io_service, tcp::endpoint(tcp::v4(), 1984));
	tcp_server<stream_flv_from> server_flv_from(io_service, tcp::endpoint(tcp::v4(), 1985));
	io_service.run();

	return 0;
}

