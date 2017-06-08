// webrtcgateway.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

/*#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>*/
#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> websocket_server;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::lock_guard;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

/* on_open insert connection_hdl into channel
 * on_close remove connection_hdl from channel
 * on_message queue send to all channels
 */


class offer_answer_server {
public:
    offer_answer_server(boost::asio::io_service* ptr_io_service, uint16_t port) {
        // Initialize Asio Transport
        m_server.init_asio(ptr_io_service);

        // Register handler callbacks
        m_server.set_open_handler(bind(&offer_answer_server::on_open,this,::_1));
        m_server.set_close_handler(bind(&offer_answer_server::on_close,this,::_1));
        m_server.set_message_handler(bind(&offer_answer_server::on_message,this,::_1,::_2));
        m_server.listen(port);
        m_server.start_accept();
    }

    void on_open(connection_hdl hdl) 
    {
        m_connections.insert(hdl);
        websocket_server::connection_ptr conn = m_server.get_con_from_hdl(hdl);
        std::cout << conn->get_uri()->str()  << " : " << conn->get_uri()->get_resource() << " - "<< conn->get_remote_endpoint() << std::endl;         
    }

    void on_close(connection_hdl hdl) 
    {        
        std::cout << "on_close" << std::endl;
        m_connections.erase(hdl);        
    }

    void on_message(connection_hdl hdl, websocket_server::message_ptr msg) 
    {        
            std::cout << "on_message" << std::endl;            
            m_server.send(hdl, "this great\r\n", websocketpp::frame::opcode::text);
            std::cout << msg->get_payload();
    }

private:
    typedef std::set<connection_hdl,std::owner_less<connection_hdl> > con_list;
    websocket_server m_server;
    con_list m_connections;
};



int _tmain(int argc, _TCHAR* argv[])
{  
    boost::asio::io_service service;    
    offer_answer_server server_instance(&service, 9002);
    boost::system::error_code ec;
     service.run(ec);
}