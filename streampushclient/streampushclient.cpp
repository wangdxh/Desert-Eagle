// streampushclient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <boost/asio.hpp>
#include "h264frame.h"

using namespace std;
using namespace boost::asio;

int writebufferto(ip::tcp::socket& sock, const char* pchar, int nlen)
{
    boost::system::error_code ec;
    int nsize = sock.write_some(boost::asio::buffer(pchar, nlen), ec);
    if (!ec)
    {
        if (nsize != nlen)
        {
            printf("write buffer error\r\n");
        }
        return 0;
    }
    else
    {
        printf("socket error %s\r\n", ec.message().c_str());
    }
    return -1;
}

int _tmain(int argc, _TCHAR* argv[])
{

    io_service service;
    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 1985);
    ip::tcp::socket sock(service);
    sock.open(ip::tcp::v4()); 
    try
    {
        sock.connect(ep);
    }
    
    catch (...)
    {
        printf("connect error\r\n");
        return -1;
    }
    


    char* strdeviceid = "123abcdef32153421";
    int nLen = strlen(strdeviceid );

    int nRet = writebufferto(sock, (const char*)&nLen, 4);
    nRet = writebufferto(sock, strdeviceid, nLen);            
    if (nRet)
    {
        return -1;
    }



    //FILE* pFile = fopen("D:\\dashmp4-master\\dashmp4demo\\Debug\\test.h264", "rb");
    FILE* pFile = fopen("./testokmy.h264", "rb");
    if (!pFile)
    {
        printf("read file error");
        return -1;
    }




    u8* buffer = new u8[1*1024*1024];
    u8* dstflvbuffer = new u8[1*1024*1024];
    u8* flvheader = new u8[1*1024*1024];
    u32 dwTime = 0;
    u32 dwLen = 0;	

    CFlv flv;

    while (1)
    {
        printf("started to send\r\n");
        while (1)
        {
            //Sleep(30);
            int nRet = fread(&dwLen, 1, 4, pFile);
            if (4 != nRet) 
            {
                break;
            }

            nRet = fread(buffer, 1, dwLen, pFile);
            if (nRet != dwLen)
            {
                break;	
            }
            Sleep(30);

            Buffer bufH264;
            bufH264.pBuffer = buffer;
            bufH264.dwBufLen = dwLen;
            Buffer bufFlvheader;
            bufFlvheader.pBuffer = flvheader;
            bufFlvheader.dwBufLen = 1*1024*1024;

            Buffer bufFlvData;
            bufFlvData.pBuffer = dstflvbuffer;
            bufFlvData.dwBufLen = 1*1024*1024;
            u32 dwRet = flv.ConvertH264ToFlv(&bufH264, 0, &bufFlvData, &bufFlvheader);
            if ((dwRet & HAS_FLVHEADER) == HAS_FLVHEADER)
            {

                int nRet = writebufferto(sock, (const char*)(&bufFlvheader.dwBufLen), 4);            
                nRet = writebufferto(sock, (const char*)bufFlvheader.pBuffer, bufFlvheader.dwBufLen);            
                if (nRet)
                {
                    return -1;
                }                    
            }
            if ((dwRet & HAS_FLVFRAMEDATA) == HAS_FLVFRAMEDATA)
            {                    
                int nRet = writebufferto(sock, (const char*)(&bufFlvData.dwBufLen), 4);            
                nRet = writebufferto(sock, (const char*)bufFlvData.pBuffer, bufFlvData.dwBufLen);            
                if (nRet)
                {
                    return -1;
                }
            }

        }
        fseek(pFile, 0, SEEK_SET);
    }

    fclose(pFile);
    return 0;
}

