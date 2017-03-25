// streamserver.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "flvbuffer.h"


int _tmain(int argc, _TCHAR* argv[])
{	
	{
		char szinfo[11] = {"1111111111"};
		boost::asio::mutable_buffer buftest(szinfo, strlen(szinfo));
		shared_const_buffer_flv tempbuf(buftest);
		
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
	return 0;
}

