#ifndef  _WXH_FLV_BUFFER_H_
#define _WXH_FLV_BUFFER_H_


#include <boost/asio.hpp>
using boost::asio::ip::tcp;
#include "rtpoverrtsp.h"

class copyed_buffer
{
public:
     explicit copyed_buffer(const boost::asio::const_buffer& buff)
    {
        int nLen = boost::asio::buffer_size(buff);
        m_streamdata = std::shared_ptr<uint8_t>(new uint8_t[nLen], []( uint8_t *p ) { delete[] p; });        
        const uint8_t* pData = boost::asio::buffer_cast<const uint8_t*>(buff);
        memcpy(m_streamdata.get(), pData, nLen);
        m_buffer = boost::asio::mutable_buffer(m_streamdata.get(), nLen);
    }
     copyed_buffer()
     {

     }
    ~copyed_buffer()
    {

    }
    bool isnull()const
    {        
        return bool(nullptr == boost::asio::buffer_cast<const uint8_t*>(m_buffer));
    }
public:
    std::shared_ptr<uint8_t> m_streamdata;	
    boost::asio::mutable_buffer m_buffer;
};

const uint32_t FLV_ASIO_BUFFER = 3;
class shared_const_buffer_flv
{
public:
    enum em_buffertype
    {
        em_http_flv,
        em_rtsp,
        em_message,
    };
public:
	explicit shared_const_buffer_flv(const boost::asio::const_buffer& buff, em_buffertype etype = em_message)
	{
		// should use make_shared but in the debug, vs the pointer is no correct		
		const uint8_t* pData = boost::asio::buffer_cast<const uint8_t*>(buff);
        m_bisflvstream = false;
        m_bisheader = false;
        m_bkeyframe = false;
        if (0x17 == pData[0]) { m_bkeyframe = true; }
        
        int nLen = boost::asio::buffer_size(buff);
        if (em_http_flv == etype || em_message == etype)
        {
            // 发往每条httpflv客户端的时间戳，必须是从0开始的，然后连续地递增，所以在即将发送的时候，再重新把flv tag的头打上去
            m_streamdata = std::shared_ptr<uint8_t>(new uint8_t[boost::asio::buffer_size(buff)], []( uint8_t *p ) { delete[] p; });
            memcpy(m_streamdata.get(), pData, nLen);
        }        
        else
        {
            throw "type must be httpflv or message";
        }
		m_abuffer[1] = boost::asio::buffer(m_streamdata.get(), nLen);
        
	}
	explicit shared_const_buffer_flv(const boost::asio::const_buffer& buff, em_buffertype etype, uint64_t dwtimestamp, uint16_t& dwsequence)
    {
        const uint8_t* pData = boost::asio::buffer_cast<const uint8_t*>(buff);
        m_bisflvstream = false;
        m_bisheader = false;
        m_bkeyframe = false;
        if (0x17 == pData[0]) { m_bkeyframe = true; }
                
        int nLen = boost::asio::buffer_size(buff);      
		uint32_t dwtotallen = nLen;
        if (em_rtsp == etype)
        {
            // 发往每个rtsp客户端的rtp里面的时间戳，并不需要从0开始，而且h264转成rtp时，每个rtp的头，是掺杂在数据中间的，
            // 所以时间戳的递增，从hub这里开始，然后递增，客户端拿到什么rtp时间，就是什么开始，并不受影响
            if (0x17 == pData[0] || 0x27 == pData[0])
            {
                uint32_t dwnumnalus;
                get_rtsp_rtp_video_total_len(pData, nLen, dwtotallen, dwnumnalus);    
                m_streamdata = std::shared_ptr<uint8_t>(new uint8_t[dwtotallen], []( uint8_t *p ) { delete[] p; });                
                bool bret = generate_rtp_info_over_rtsp(pData, nLen, m_streamdata.get(), dwtotallen,
					                                    dwnumnalus, dwtimestamp, dwsequence);
                if (false == bret)
                {
                    throw "genereate rtp over rtsp len wrong totallen is %d, return %d";
                }
            }
        }
        else
        {
            throw "steam type must be rtst to ";
        }
        m_abuffer[1] = boost::asio::buffer(m_streamdata.get(), dwtotallen);
    }
	const boost::asio::const_buffer* getstreamdata() const
	{
		return &m_abuffer[1];
	}
	void setchunk(char* pHeaderChunk, uint32_t dwChunkLen, char* pChunkEnd, uint32_t dwChunkEndLen)
	{
		m_abuffer[0] = boost::asio::buffer(pHeaderChunk, dwChunkLen);
		m_abuffer[2] = boost::asio::buffer(pChunkEnd, dwChunkEndLen);
	}
    void setisflvstream(bool isflvsteam) { m_bisflvstream = isflvsteam;}
    bool isflvstream() const  { return m_bisflvstream;}    
    void setisflvheader(bool bisheader) { m_bisheader = bisheader; }
    bool isflvheader() const { return m_bisheader; }
    bool iskeyframe() const { return m_bkeyframe; }

	// Implement the ConstBufferSequence requirements.
	typedef boost::asio::const_buffer value_type;
	typedef const boost::asio::const_buffer* const_iterator;
	const boost::asio::const_buffer* begin() const { return m_abuffer; }
	const boost::asio::const_buffer* end() const { return m_abuffer + FLV_ASIO_BUFFER; }

private:
	std::shared_ptr<uint8_t> m_streamdata;	
	boost::asio::const_buffer m_abuffer[FLV_ASIO_BUFFER];
    bool m_bisflvstream;
    bool m_bkeyframe;
    bool m_bisheader;
};


#include <deque>
#include <set>
typedef std::deque<shared_const_buffer_flv> stream_message_queue;

class stream_session
{
public:
	virtual ~stream_session() {}
	virtual void deliver(const shared_const_buffer_flv& msg) = 0; //participant should deliver message    
};

typedef std::shared_ptr<stream_session> stream_session_ptr;//shared ptr


class stream_hub
{
public:    
    explicit stream_hub(std::string& name)
    {
        m_strname = name;
        m_u64timestamp = 0;
        m_dwsequence = 1;
        printf("new stream hub create %s\r\n", name.c_str());
    }
    ~stream_hub()
    {
        printf("stream hub destroy %s\r\n", m_strname.c_str());
    }
	void join_http_flv(stream_session_ptr participant)
	{
		http_flv_sessions_.insert(participant);//add a client		
        if (!m_buf_header.isnull())
        {
            shared_const_buffer_flv flvheader(m_buf_header.m_buffer, shared_const_buffer_flv::em_http_flv);// send flv header
            flvheader.setisflvheader(true);
			flvheader.setisflvstream(true);
            participant->deliver(flvheader);
        }
	}
    
	void leave_http_flv(stream_session_ptr participant)
	{
		http_flv_sessions_.erase(participant);//remove a client
	}

    void join_rtsp(stream_session_ptr participant)
    {
        rtsp_sessions_.insert(participant);
        // rtsp do not need the flv header
    }
    void leave_rtsp(stream_session_ptr participant)
    {
        rtsp_sessions_.erase(participant);//remove a client
    }

	void deliver(const boost::asio::mutable_buffer& msg, bool isheader = false)
	{
		if (http_flv_sessions_.size() > 0)
		{
			shared_const_buffer_flv flvbuf(msg, shared_const_buffer_flv::em_http_flv);
            flvbuf.setisflvheader(isheader);
			flvbuf.setisflvstream(true);
			for (auto session: http_flv_sessions_)
				session->deliver(flvbuf);
		}
        if (rtsp_sessions_.size() > 0)
        {
            shared_const_buffer_flv flvbuf(msg, shared_const_buffer_flv::em_rtsp, m_u64timestamp, m_dwsequence);
            flvbuf.setisflvheader(isheader);
            flvbuf.setisflvstream(true);
            for (auto session: rtsp_sessions_)
                session->deliver(flvbuf);
            m_u64timestamp += (40*90);
        }
	}
    void setmetadata(const boost::asio::mutable_buffer& msg)
    {
        m_buf_header = copyed_buffer(msg);
        this->deliver(m_buf_header.m_buffer, true);
    }
	const boost::asio::mutable_buffer& getmetadata() const
	{
		return m_buf_header.m_buffer;
	}
    void eraseallsession()
    {
        http_flv_sessions_.clear();
        rtsp_sessions_.clear();        
    }
private:
	std::set<stream_session_ptr> http_flv_sessions_;//all client
    std::set<stream_session_ptr> rtsp_sessions_;//all client
	copyed_buffer m_buf_header;
	std::string m_strname;
    uint64_t m_u64timestamp;
    uint16_t m_dwsequence;
};

typedef std::shared_ptr<stream_hub> stream_hub_ptr;
std::map< std::string, stream_hub_ptr > g_map_stream_hubs;

stream_hub_ptr create_stream_hub(std::string& name)
{
    stream_hub_ptr room_;
    std::map< std::string, stream_hub_ptr >::iterator iter = g_map_stream_hubs.find(name);
    if (iter != g_map_stream_hubs.end())
    {
        printf("error error***************************** stream  had exitsed name : %s\r\n", name.c_str());
    }
    else
    {
        room_ = std::shared_ptr<stream_hub>(new stream_hub(name));
        g_map_stream_hubs[name] = room_;        
    }
    return room_;
}
bool is_stream_hub_exists(std::string& name)
{
    std::map< std::string, stream_hub_ptr >::iterator iter = g_map_stream_hubs.find(name);
    if (iter != g_map_stream_hubs.end())
    {
        return true;
    }
    return false;    
}

stream_hub_ptr get_stream_hub(std::string& name)
{    
    std::map< std::string, stream_hub_ptr >::iterator iter = g_map_stream_hubs.find(name);
    if (iter == g_map_stream_hubs.end())
    {
        printf("error error***************************** stream not exit name : %s\r\n", name.c_str());
    }
    stream_hub_ptr  room_ = iter->second;
    return room_;
}


const uint32_t MSG_HEADER_LEN_SIZE = 4;

class stream_flv_from : public std::enable_shared_from_this<stream_flv_from>
{
public:
	stream_flv_from(tcp::socket socket)
		: socket_(std::move(socket))
	{
        m_bget_stream_name = false;
        m_bget_flv_header = false;

        sprintf(m_szendpoint, "ip:%s port:%d", socket_.remote_endpoint().address().to_string().c_str(),
            socket_.remote_endpoint().port());
        printf("fromed new client info : %s\r\n", m_szendpoint);
	}
	~stream_flv_from()
	{
        printf("fromed client  leave info : %s\r\n", m_szendpoint);
		this->close();
	}
	void start()
	{	
		do_read_header();
	}
	void close()
    {
        if (!m_streamname.empty())
        {            
            room_->eraseallsession();
            g_map_stream_hubs.erase(m_streamname);
        }
    }
private:
	void do_read_header()
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(m_szMsgLen, MSG_HEADER_LEN_SIZE),//data,length
			[this, self](boost::system::error_code ec, std::size_t /*length*/)//lambada
		{
			if (!ec)
			{                
				uint32_t dwMsgLen = (m_szMsgLen[3]<<24) | (m_szMsgLen[2]<<16) |
                                                   (m_szMsgLen[1]<<8) | m_szMsgLen[0] ;									  
				do_read_body(dwMsgLen);// read body
			}
		});
	}

	void do_read_body(uint32_t dwMsgLen)
	{        
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(m_bufmsg, dwMsgLen),
			[this, self](boost::system::error_code ec, std::size_t  length)
		{
			if (!ec)
			{
                boost::asio::mutable_buffer steambuf (m_bufmsg, length);
                if (false == m_bget_stream_name)
                {
                    m_bget_stream_name = true;
                    m_bufmsg[length] = '\0';
                    m_streamname = (char*)m_bufmsg;                    
                    room_ = create_stream_hub(m_streamname);
                }
                else if(false == m_bget_flv_header)
                {
                    m_bget_flv_header = true;
                    room_->setmetadata(steambuf);                    
                }
                else
                {
                    room_->deliver(steambuf);//deliver将发送消息，刚连接进来的客户将收到历史消息    
                }
				
				do_read_header();//
			}
		});
	}
private:
	stream_hub_ptr room_;
	tcp::socket socket_;	
	
	uint8_t m_szMsgLen[MSG_HEADER_LEN_SIZE];
	uint8_t m_bufmsg[512*1024];
	bool m_bget_stream_name;
    std::string m_streamname;
    bool m_bget_flv_header;
	uint32_t m_dwTime;
    char m_szendpoint[32];
};//seesion


#define MAX_STREAM_BUFFER_NUMS 10
class stream_httpflv_to:
	public stream_session,
	public std::enable_shared_from_this<stream_httpflv_to>
{
public:
	stream_httpflv_to(tcp::socket socket)
		: socket_(std::move(socket))
	{        
        m_dwtime = 0;
        m_szchunkend= "\r\n";
        m_bfirstkeycoming = false;
        sprintf(m_szendpoint, "ip:%s port:%d", socket_.remote_endpoint().address().to_string().c_str(),
                                                                    socket_.remote_endpoint().port());
        printf("httpflv new client info : %s\r\n", m_szendpoint);
	}
	~stream_httpflv_to()
	{
		//this->close();
        printf("httpflv client leave info : %s\r\n", m_szendpoint);
	}
	void start()
	{	        
		do_read_header();
	}
    
	void deliver(const shared_const_buffer_flv& msg)
	{		
		if (msg.isflvstream() && !msg.isflvheader())
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
                        "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: video/x-flv\r\nTransfer-Encoding: chunked\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
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
                    room_->join_http_flv(shared_from_this());
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
                    room_->leave_http_flv(shared_from_this());
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
	
	uint32_t m_dwtime;
    char* m_szchunkend;
    char m_szchunkbuf[32];
    bool m_bfirstkeycoming;

    char m_szendpoint[32];
};//seesion

template <class T>
class tcp_server
{
public:
	//constructor
	tcp_server(boost::asio::io_service& io_service,
		const tcp::endpoint& endpoint)
		: acceptor_(io_service, endpoint),//listen fd
		socket_(io_service)//conn fd
	{
        do_accept();
	}

private:
	void do_accept()
	{
		//a new connection
		acceptor_.async_accept(socket_,
			[this](boost::system::error_code ec)
		{
			if (!ec)
			{				
				std::make_shared<T>(std::move(socket_))->start();//session
			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
};


#endif // ! _WXH_FLV_BUFFER_H_
   