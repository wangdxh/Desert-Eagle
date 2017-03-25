#ifndef  _WXH_FLV_BUFFER_H_
#define _WXH_FLV_BUFFER_H_

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

const uint32_t FLV_ASIO_BUFFER = 3;
class shared_const_buffer_flv
{
public:
	explicit shared_const_buffer_flv(const boost::asio::const_buffer& buff):
		m_streamdata(new uint8_t[boost::asio::buffer_size(buff)], []( uint8_t *p ) { delete[] p; })
	{
		// should use make_shared but in the debug, vs the pointer is no correct
		int nLen = boost::asio::buffer_size(buff);
		const uint8_t* pData = boost::asio::buffer_cast<const uint8_t*>(buff);				
		memcpy(m_streamdata.get(), pData, nLen);
		m_abuffer[1] = boost::asio::buffer(m_streamdata.get(), nLen);
	}
	
	const boost::asio::const_buffer* getstreamdata()
	{
		return &m_abuffer[1];
	}
	void setchunk(char* pHeaderChunk, uint32_t dwChunkLen, char* pChunkEnd, uint32_t dwChunkEndLen)
	{
		m_abuffer[0] = boost::asio::buffer(pHeaderChunk, dwChunkLen);
		m_abuffer[2] = boost::asio::buffer(pChunkEnd, dwChunkEndLen);
	}


	// Implement the ConstBufferSequence requirements.
	typedef boost::asio::const_buffer value_type;
	typedef const boost::asio::const_buffer* const_iterator;
	const boost::asio::const_buffer* begin() const { return m_abuffer; }
	const boost::asio::const_buffer* end() const { return m_abuffer + FLV_ASIO_BUFFER; }

public:
	std::shared_ptr<uint8_t> m_streamdata;
	
	boost::asio::const_buffer m_abuffer[FLV_ASIO_BUFFER];
};



#include <deque>
#include <set>
typedef std::deque<shared_const_buffer_flv> chat_message_queue;

class chat_participant
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const shared_const_buffer_flv& msg) = 0; //participant should deliver message
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;//shared ptr

class chat_room
{
public:
	void join(chat_participant_ptr participant)
	{
		participants_.insert(participant);//add a client
		// send flv header
		shared_const_buffer_flv flvheader(m_bufHeader);
			participant->deliver(flvheader);
	}

	void leave(chat_participant_ptr participant)
	{
		participants_.erase(participant);//remove a client
	}

	void deliver(const boost::asio::mutable_buffer& msg)
	{
		shared_const_buffer_flv flvbuf(msg);

		for (auto participant: participants_)
			participant->deliver(flvbuf);
	}

private:
	std::set<chat_participant_ptr> participants_;//all client
	
	boost::asio::mutable_buffer m_bufHeader;
	boost::asio::mutable_buffer m_bufData;
};//chat room


const uint32_t MSG_HEADER_LEN_SIZE = 4;
class chat_session:
	public chat_participant,
	public std::enable_shared_from_this<chat_session>
{
public:
	chat_session(tcp::socket socket, chat_room& room)
		: socket_(std::move(socket)),
		room_(room)
	{
	}
	void start()
	{
		//收到连接之后首先将客户加入到room中
		//
		room_.join(shared_from_this());//add a participant

		do_read_header();//读消息
	}
	void deliver(const shared_const_buffer_flv& msg)
	{
		//
		bool write_in_progress = !write_msgs_.empty();//空
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
		boost::asio::async_read(socket_,
			boost::asio::buffer(m_szMsgLen, MSG_HEADER_LEN_SIZE),//data,length
			[this, self](boost::system::error_code ec, std::size_t /*length*/)//lambada
		{
			if (!ec)
			{
				uint32_t dwMsgLen = m_szMsgLen[0] | (m_szMsgLen[1]<<8) |
									 (m_szMsgLen[2]<<16) | (m_szMsgLen[3]<<24);
				do_read_body(dwMsgLen);// read body
			}
			else
			{
				auto ep_=socket_.remote_endpoint();
				
				room_.leave(shared_from_this());//read a eof
			}
		});
	}

	void do_read_body(uint32_t dwMsgLen)
	{
		auto self(shared_from_this());
		boost::asio::async_read(socket_,
			boost::asio::buffer(m_spMsg.get(), dwMsgLen),
			[this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				room_.deliver(read_msg_);//deliver将发送消息，刚连接进来的客户将收到历史消息
				do_read_header();//
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});
	}

	void do_write()
	{
		auto self(shared_from_this());
		if (!m_bSendedFlvHeader)
		{

		} 
		else
		{

			m_dwTime += 40;
		}



		boost::asio::async_write(socket_,//当前session的socket
			write_msgs_.front(),
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
				room_.leave(shared_from_this());
			}
		});
	}

	tcp::socket socket_;
	chat_room& room_;
	chat_message_queue write_msgs_;
	boost::asio::mutable_buffer read_msg_;
	uint8_t m_szMsgLen[MSG_HEADER_LEN_SIZE];
	std::shared_ptr<uint8_t> m_spMsg;
	bool m_bSendedFlvHeader;
	uint32_t m_dwTime;
};//seesion

class chat_server
{
public:
	//constructor
	chat_server(boost::asio::io_service& io_service,
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
				auto ep_=socket_.remote_endpoint();				
				std::make_shared<chat_session>(std::move(socket_), room_)->start();//session

			}

			do_accept();
		});
	}

	tcp::acceptor acceptor_;
	tcp::socket socket_;
	//server should keep all client connected
	chat_room room_;//chat_room class//保存所有的client
};





#endif // ! _WXH_FLV_BUFFER_H_
