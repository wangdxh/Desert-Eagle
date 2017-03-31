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
		m_strlocalip = socket_.local_endpoint().address().to_string();
		m_strremoteip = socket_.remote_endpoint().address().to_string();
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

                // reverse find
                int nPos = request_data.find_last_of("\r\n\r\n");
                m_readstreambuf.consume(nPos+1);
                buf2 = m_readstreambuf.data();

                std::map< std::string, std::string > mapitems;
                get_all_options_from_text(request_data , mapitems);
                
                bool bexists = true;                
                bool bjoin = false;
                if (m_streamname.empty())
                {
                    m_streamname = mapitems["deviceid"];
                     bexists = is_stream_hub_exists(m_streamname);
                    if (false == bexists) m_streamname.clear();
                }
                                
                std::string strresponse;
                std::string sss = mapitems["CSeq"];
                if (false == bexists)
                {
                    strresponse = "RTSP/1.0 404 notfounded\r\n"  
                                          "CSeq:"+mapitems["CSeq"]+"\r\n" 
                                          "\r\n";  
                }
                else
                {
                    std::string method = mapitems["methond"];
                    if (0 == method.compare("OPTIONS"))
                    {
                        strresponse = "RTSP/1.0 200 OK\r\n" 
                                              "CSeq:"+ mapitems["CSeq"] +"\r\n" 
                                              "Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE\r\n"
                                              "\r\n";
                    }
                    else if (0 == method.compare("DESCRIBE"))
                    {                                       
						std::stringstream sdpstream;
                        sdpstream << "v=0\r\n"
                            "o=- 1331092087436965 1 IN IP4 " << m_strlocalip << "\r\n"
                            "s=H.264 Video, streamed by Little_wang\r\n"
                            "t=0 0\r\n"
                            "m=video 0 RTP/AVP 96\r\n"
                            "c=IN IP4 0.0.0.0\r\n"
                            //"b=AS:500\r\n"
                            "a=rtpmap:96 H264/90000\r\n"
                            "a=fmtp:96 packetization-mode=1;profile-level-id=4D4033;sprop-parameter-sets=Z01AM5JUDAS0IAAAAwBAAAAM0eMGVA==,aO48gA==\r\n"
                            "a=control:track1"
                            ;
						std::string strsdp = sdpstream.str();
                        std::stringstream stream;
                        stream << "RTSP/1.0 200 OK\r\n"
                                              "CSeq:" << mapitems["CSeq"] <<"\r\n"
                                              "Content-Base:" << mapitems["baseurl"] << "\r\n"
                                              "Content-Type: application/sdp\r\n"
                                               "Content-Length:" << strsdp.length() << "\r\n"
                                               "\r\n" << strsdp;
                        strresponse = stream.str();
                        
                    }
                    else if (0 == method.compare("SETUP"))
                    {
						// should see is track1 or track2
						int track = get_url_track_num(mapitems["url"]);
						std::cout << "setup track " << track << std::endl;
                        std::stringstream stream;
                        stream << "RTSP/1.0 200 OK\r\n"
                                   "CSeq:" << mapitems["CSeq"] <<"\r\n"
                                   "Transport: RTP/AVP/TCP;unicast;destination=" << m_strremoteip  
                                            << ";source=" << m_strlocalip <<";interleaved=0-1\r\n"
                                   "Session: 289BFEAE\r\n"
                                   "\r\n";
                        strresponse = stream.str();
                    }
                    else if (0 == method.compare("PLAY"))
                    {
                        std::stringstream stream;
                        stream << "RTSP/1.0 200 OK\r\n"
                            "CSeq:" << mapitems["CSeq"] <<"\r\n"
                            "Range:" << mapitems["Range"] <<"\r\n"
                            "Session: 289BFEAE\r\n"
                            "RTP-Info: url=rtsp://" << m_strlocalip << "/realplay/track1;seq=1;rtptime=0\r\n"
                            "\r\n";
                        strresponse = stream.str();
                        bjoin = true;
                    }
                    else
                    {
                        strresponse = "RTSP/1.0 404 notfounded\r\n"  
                            "CSeq:"+mapitems["CSeq"]+"\r\n" 
                            "\r\n";  
                    }
                }
                

                shared_const_buffer_flv httpresponse(boost::asio::buffer(strresponse, strresponse.length()));                
                this->deliver(httpresponse);
                if (bjoin)
                {
                    hub_ = get_stream_hub(m_streamname);
                    hub_->join_rtsp(shared_from_this());
                }
                if (bexists)
                {
                    do_read_header();
                }
            }
            else
            {
                // read error
                if (hub_.get() != nullptr)
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
                if (hub_.get() != nullptr)
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
	std::string m_strlocalip;
	std::string m_strremoteip;
};


#endif
