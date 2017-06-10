#ifndef _WXH_OFFER_ANSWER_SERVER_H_
#define _WXH_OFFER_ANSWER_SERVER_H_


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

class nice_agent;
typedef std::shared_ptr<nice_agent> nice_agent_ptr;

struct connection_data {
    nice_agent_ptr agent;
    uint32_t msgid;
};

class offer_answer_server 
{
public:
    offer_answer_server(boost::asio::io_service* ptr_io_service, uint16_t port);
    void on_open(connection_hdl hdl) ;
    void on_close(connection_hdl hdl) ;
    void on_message(connection_hdl hdl, websocket_server::message_ptr msg);    
    connection_data& get_data_from_hdl(connection_hdl hdl);
private:    
     typedef std::map<connection_hdl,connection_data,std::owner_less<connection_hdl>> con_list;
    websocket_server m_server;
    con_list m_connections;
};

#endif