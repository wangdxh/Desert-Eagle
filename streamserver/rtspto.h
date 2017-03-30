#ifndef _WXH_RTSP_TO_H_
#define _WXH_RTSP_TO_H_

#include "flvbuffer.h"
/*#include <boost/format.hpp>  
#include <boost/tokenizer.hpp>  
#include <boost/algorithm/string.hpp>  */
// http://www.cplusplus.com/faq/sequences/strings/split/

#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

class stream_rtsp_to:
    public stream_session,
    public std::enable_shared_from_this<stream_rtsp_to>
{
public:
    stream_rtsp_to(tcp::socket socket)
        : socket_(std::move(socket))
    {
        m_bfirstkeycoming = false;
        sprintf(m_szendpoint, "ip:%s port:%d", socket_.remote_endpoint().address().to_string().c_str(),
            socket_.remote_endpoint().port());
        printf("rtsp_to new client info : %s\r\n", m_szendpoint);
    }
    ~stream_rtsp_to()
    {        
        printf("rtsp_to client leave info : %s\r\n", m_szendpoint);
    }
    void start()
    {
        do_read_header();
    }

    void deliver(const shared_const_buffer_flv& msg)
    {		
        if (msg.isflvstream())
        {
            // rtsp do not need flv header
            if (msg.isflvheader()) {return ;}
            
            if (false == m_bfirstkeycoming )
            {
                if (!msg.iskeyframe())
                {
                    printf("rtsp_to data keyframe is not coming %s  \r\n", m_szendpoint);
                    return;
                }
                else
                {
                    m_bfirstkeycoming = true;
                }                            
            }
            if (write_msgs_.size() > MAX_STREAM_BUFFER_NUMS)
            {
                //buffer is full, do not need p-frame,so wait the I-frame
                m_bfirstkeycoming = false;
                printf("rpst_to the buffer over the max number %d, %s\r\n", MAX_STREAM_BUFFER_NUMS, m_szendpoint);
                return;
            }
        }
        
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);//会将消息先放到write_msgs_中
        if (!write_in_progress)
        {
            //write message
            do_write();
        }
    }
private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read_until(socket_, m_readstreambuf, "\r\n\r\n",
            [this, self](boost::system::error_code ec, std::size_t /*length*/)//lambada
        {
            if (!ec)
            {
                boost::asio::const_buffer buf2 = m_readstreambuf.data();
                
                //将boost::asio::streambuf转为std::string  
                boost::asio::streambuf::const_buffers_type cbt = m_readstreambuf.data(); 
                std::string request_data(boost::asio::buffers_begin(cbt), boost::asio::buffers_end(cbt)); 

                int nPos = request_data.find_last_of("\r\n\r\n");
                m_readstreambuf.consume(nPos+1+4);
                buf2 = m_readstreambuf.data();

                std::vector<std::string> vec;
                boost::split_regex( vec, request_data, boost::regex( "\r\n\r\n" ) ); 
                for(auto command : vec)
                {
                    std::vector<std::string> vecitem;
                    boost::split_regex( vecitem, command, boost::regex( "\r\n" ) ); 
                    for(auto item : vecitem)
                    {
                        if (!item.empty())
                        {
                            printf("item -%s- \r\n", item.c_str());
                        }                        
                    }
                }
                // for test 172.16.64.92:1984/live/liveflv?deviceid=12341234324
                boost::asio::const_buffer buf = m_readstreambuf.data();
                
                const char* pData = boost::asio::buffer_cast<const char*>(buf);
                char* tag = "deviceid=";
                const char* pstart = strstr(pData, tag);
                if (pstart)
                {
                    const char* pend = strstr(pstart, " ");
                    if (pend)
                    {
                        int nlen = pend - pstart-strlen(tag);
                        m_streamname = std::string(pstart+strlen(tag), nlen);                        
                    }
                }                

                bool bexists = is_stream_hub_exists(m_streamname);
                std::string strresponse;
                if (bexists)
                {
                    strresponse = 		
                        "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: video/x-flv\r\nTransfer-Encoding: chunked\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
                }
                else
                {
                    strresponse = "HTTP/1.1 404 notfounded \r\n\r\n";   
                }

                shared_const_buffer_flv httpresponse(boost::asio::buffer(strresponse));
                httpresponse.setisflvstream(false);
                this->deliver(httpresponse);

                if (bexists)
                {
                    hub_ = get_stream_hub(m_streamname);
                    hub_->join_rtsp(shared_from_this());
                    // again
                    do_read_header();
                }
                else
                {
                    // will exit and close this socket
                    printf("no stream hub rtsp client will quit %s\r\n", m_szendpoint);
                }                
            }
            else
            {
                // read error
                if (!m_streamname.empty())
                {
                    hub_->leave_rtsp(shared_from_this());
                    m_streamname.clear();
                }
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());

        shared_const_buffer_flv& ptagflvbuf = write_msgs_.front();
        boost::asio::async_write(socket_,//当前session的socket
            ptagflvbuf,
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                if (!m_streamname.empty())
                {
                    hub_->leave_rtsp(shared_from_this());
                    m_streamname.clear();
                }
            }
        });
    }
    std::shared_ptr<stream_hub> hub_;    
    std::string m_streamname;
    tcp::socket socket_;	
    stream_message_queue write_msgs_;
    boost::asio::streambuf m_readstreambuf;
    
    bool m_bfirstkeycoming;
    char m_szendpoint[32];
};


#endif
