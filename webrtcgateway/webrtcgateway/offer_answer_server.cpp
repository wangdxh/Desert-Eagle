#include "offer_answer_server.h"
#include "nice_agent.h"


offer_answer_server::offer_answer_server(boost::asio::io_service* ptr_io_service, uint16_t port) 
{
    // Initialize Asio Transport
    m_server.init_asio(ptr_io_service);

    // Register handler callbacks
    m_server.set_open_handler(bind(&offer_answer_server::on_open,this,::_1));
    m_server.set_close_handler(bind(&offer_answer_server::on_close,this,::_1));
    m_server.set_message_handler(bind(&offer_answer_server::on_message,this,::_1,::_2));
    m_server.listen(port);
    m_server.start_accept();
}

void offer_answer_server::on_open(connection_hdl hdl) 
{    
    websocket_server::connection_ptr conn = m_server.get_con_from_hdl(hdl);
    std::cout << conn->get_uri()->str()  << " : " << conn->get_uri()->get_resource() << " - "<< conn->get_remote_endpoint() << std::endl;

    nice_agent_ptr agent = std::make_shared<nice_agent>(&m_server, hdl, true);            
    int nstreamid = agent->add_stream("audio", 1);
    agent->start_gather(nstreamid);
    connection_data data;
    data.agent = agent;
    data.msgid = 0;
    m_connections[hdl] = data;
}

void offer_answer_server::on_close(connection_hdl hdl) 
{        
    std::cout << "on_close" << std::endl;
    m_connections.erase(hdl);        
}

void offer_answer_server::on_message(connection_hdl hdl, websocket_server::message_ptr msg) 
{        
    std::cout << "on_message" << std::endl;            
    //m_server.send(hdl, "this great\r\n", websocketpp::frame::opcode::text);
    std::cout << msg->get_payload();
    connection_data& data = get_data_from_hdl(hdl);
    if (0 == data.msgid)
    {
        data.agent->set_remote_sdp(msg->get_payload().c_str());
    }
    else if (1 == data.msgid)
    {
        data.agent->set_remote_candidate(msg->get_payload().c_str());
    }
    data.msgid++;
}

connection_data& offer_answer_server::get_data_from_hdl(connection_hdl hdl) 
{    
    auto it = m_connections.find(hdl);

    if (it == m_connections.end()) {
        // this connection is not in the list. This really shouldn't happen
        // and probably means something else is wrong.
        throw std::invalid_argument("No data available for session");
    }

    return it->second;
}
