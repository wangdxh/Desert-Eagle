// webrtcgateway.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "offer_answer_server.h"
#include "nice_agent.h"



int _tmain(int argc, _TCHAR* argv[])
{      
    nice_agent::init();

    boost::asio::io_service service;
    offer_answer_server server_instance(&service, 9002);
    boost::system::error_code ec;
     service.run(ec);

     nice_agent::release();
}