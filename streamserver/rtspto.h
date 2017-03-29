#ifndef _WXH_RTSP_TO_H_
#define _WXH_RTSP_TO_H_

#include "flvbuffer.h"


class stream_rtsp_to:
    public stream_session,
    public std::enable_shared_from_this<stream_rtsp_to>
{
public:
    stream_rtsp_to(tcp::socket socket)
        : socket_(std::move(socket))
    {        
        m_dwtime = 0;        
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
            if (!msg.isflvheader() && false == m_bfirstkeycoming )
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
        }
        if (write_msgs_.size() > MAX_STREAM_BUFFER_NUMS)
        {
            //buffer is full, do not need p-frame,so wait the I-frame
            m_bfirstkeycoming = false;
            printf("rpst_to the buffer over the max number %d, %s\r\n", MAX_STREAM_BUFFER_NUMS, m_szendpoint);
            return;
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
                    hub_->join(shared_from_this());
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
            const char* pdata = boost::asio::buffer_cast<const char*>(*pbuffer);            

            int nLen;
            //memset(m_szchunkbuf, sizeof(m_szchunkbuf), 0);
            if (pdata[0] == 0x17 || pdata[0] == 0x27)
            {
                int ntaglen = nsize -4;
                nLen = sprintf(m_szchunkbuf, "%x\r\n", nsize+11);
                m_szchunkbuf[nLen+0] = 9; //video
                m_szchunkbuf[nLen+1] = (ntaglen >> 16) & 0xff;
                m_szchunkbuf[nLen+2] = (ntaglen >> 8) & 0xff;
                m_szchunkbuf[nLen+3] = ntaglen & 0xff;

                // nb timestamp
                m_szchunkbuf[nLen+4] = (m_dwtime>> 16) & 0xff;
                m_szchunkbuf[nLen+5] = (m_dwtime>> 8) & 0xff;
                m_szchunkbuf[nLen+6] = m_dwtime& 0xff;
                m_szchunkbuf[nLen+7] = (m_dwtime>> 24) & 0xff;
                m_szchunkbuf[nLen+8] = 0;
                m_szchunkbuf[nLen+9] = 0;
                m_szchunkbuf[nLen+10] = 0;             

                nLen += 11;
                m_dwtime += 40;
            }
            else
            {
                nLen = sprintf(m_szchunkbuf, "%x\r\n", nsize);               
            }
            ptagflvbuf.setchunk(m_szchunkbuf, nLen, m_szchunkend, 2);            
        }


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
                    hub_->leave(shared_from_this());
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

    uint32_t m_dwtime;
    bool m_bfirstkeycoming;

    char m_szendpoint[32];
};


#endif
