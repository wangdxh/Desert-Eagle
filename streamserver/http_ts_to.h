#ifndef _WXH_HTTP_TS_H_
#define _WXH_HTTP_TS_H_

#include "flvbuffer.h"


class stream_http_ts_to:
    public stream_session,
    public std::enable_shared_from_this<stream_http_ts_to>
{
public:
    stream_http_ts_to(tcp::socket socket)
        : socket_(std::move(socket))
    {                        
        m_szchunkend= "\r\n";
        m_bfirstkeycoming = false;
        sprintf(m_szendpoint, "ip:%s port:%d", socket_.remote_endpoint().address().to_string().c_str(),
            socket_.remote_endpoint().port());
        printf("http ts new client info : %s\r\n", m_szendpoint);
    }
    ~stream_http_ts_to()
    {
        //this->close();
        printf("http ts client leave info : %s\r\n", m_szendpoint);
    }
    void start()
    {	        
        do_read_header();
    }

    // ts do not need flv header
    void deliver(const shared_const_buffer_flv& msg)
    {		
        if (msg.isflvheader()) return; // do not need header        
        
        if (msg.isflvstream())
        {
            // all flv info need chunked
            if (false == m_bfirstkeycoming )
            {
                if (!msg.iskeyframe())
                {
                    printf("flvdata keyframe is not coming %s  \r\n", m_szendpoint);
                    return;
                }
                else
                {
                    m_bfirstkeycoming = true;
                }				            
            }
            // just drop the stream data but not the head and protocol
            if (write_msgs_.size() > MAX_STREAM_BUFFER_NUMS)
            {
                //buffer is full, do not need p-frame,so wait the I-frame
                m_bfirstkeycoming = false;
                printf("the buffer over the max number %d, %s\r\n", MAX_STREAM_BUFFER_NUMS, m_szendpoint);
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
                        "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: video/mp2t\r\nTransfer-Encoding: chunked\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
                }
                else
                {
                    strresponse = "HTTP/1.1 404 notfounded \r\n\r\n";   
                }

                shared_const_buffer_flv httpresponse(boost::asio::buffer(strresponse));
                this->deliver(httpresponse);

                if (bexists)
                {
                    room_ = get_stream_hub(m_streamname);
                    room_->join_http_ts(shared_from_this());
                }
            }
        });
    }

    void do_write()
    {
        auto self(shared_from_this());

        shared_const_buffer_flv& ptagflvbuf = write_msgs_.front();
        if (ptagflvbuf.isflvstream())
        {
            const boost::asio::const_buffer* pbuffer = ptagflvbuf.getstreamdata();
            int nsize = boost::asio::buffer_size(*pbuffer);
            int nLen = sprintf(m_szchunkbuf, "%x\r\n", nsize);
            ptagflvbuf.setchunk(m_szchunkbuf, nLen, m_szchunkend, 2);            
        }

        boost::asio::async_write(socket_,//当前session的socket
            ptagflvbuf,
            [this, self](boost::system::error_code ec, std::size_t length/*length*/)
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
                    room_->leave_http_ts(shared_from_this());
                    m_streamname.clear();
                }
            }
        });
    }
    std::shared_ptr<stream_hub> room_;    
    std::string m_streamname;
    tcp::socket socket_;	
    stream_message_queue write_msgs_;
    boost::asio::streambuf m_readstreambuf;
    
    char* m_szchunkend;
    char m_szchunkbuf[32];
    bool m_bfirstkeycoming;

    char m_szendpoint[32];
};//seesion


#endif