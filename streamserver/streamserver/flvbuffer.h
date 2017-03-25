#ifndef  _WXH_FLV_BUFFER_H_
#define _WXH_FLV_BUFFER_H_

#include <boost/asio.hpp>
const uint32_t FLV_ASIO_BUFFER = 3;
class shared_const_buffer_flv
{
public:
	// Construct from a std::string.
	explicit shared_const_buffer_flv(const boost::asio::const_buffer& buff)	
	{
		int nLen = boost::asio::buffer_size(buff);
		const uint8_t* pData = boost::asio::buffer_cast<const uint8_t*>(buff);				
		m_streamdata = std::make_shared<uint8_t>(nLen);
		printf("get ptr %x", m_streamdata.get());
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


#endif // ! _WXH_FLV_BUFFER_H_
