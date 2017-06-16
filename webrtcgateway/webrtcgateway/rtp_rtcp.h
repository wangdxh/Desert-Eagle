#ifndef _WXH_RTP_HEADER_H_
#define _WXH_RTP_HEADER_H_


#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#ifdef __MACH__
#include <machine/endian.h>
#else
#include <endian.h>
#endif
#endif

#include "inttypes.h"

#define RTP_HEADER_SIZE	12

#ifdef WIN32
#define __BYTE_ORDER  0x1234
#define __BIG_ENDIAN 0x3412
#define __LITTLE_ENDIAN 0x1234
#endif // WIN32


typedef struct rtp_header
{
#if __BYTE_ORDER == __BIG_ENDIAN
	uint16_t version:2;
	uint16_t padding:1;
	uint16_t extension:1;
	uint16_t csrccount:4;
	uint16_t markerbit:1;
	uint16_t type:7;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
	uint16_t csrccount:4;
	uint16_t extension:1;
	uint16_t padding:1;
	uint16_t version:2;
	uint16_t type:7;
	uint16_t markerbit:1;
#endif

	uint16_t seq_number;
	uint32_t timestamp;
	uint32_t ssrc;
	uint32_t csrc[16];
} rtp_header;

/*! \brief RTP packet */
typedef struct janus_rtp_packet {
	char *data;
	int32_t length;
	int64_t created;
	int64_t last_retransmit;
} janus_rtp_packet;

/*! \brief RTP extension */
typedef struct janus_rtp_header_extension {
	uint16_t type;
	uint16_t length;
} janus_rtp_header_extension;

//////////////////////////////////////////////////////////////////////////
typedef enum {
    RTCP_FIR = 192,
    RTCP_SR = 200,
    RTCP_RR = 201,
    RTCP_SDES = 202,
    RTCP_BYE = 203,
    RTCP_APP = 204,
    RTCP_RTPFB = 205,
    RTCP_PSFB = 206,
} rtcp_type;


/*! \brief RTCP Header (http://tools.ietf.org/html/rfc3550#section-6.1) */
typedef struct rtcp_header
{
#if __BYTE_ORDER == __BIG_ENDIAN
    uint16_t version:2;
    uint16_t padding:1;
    uint16_t rc:5;
    uint16_t type:8;
#elif __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t rc:5;
    uint16_t padding:1;
    uint16_t version:2;
    uint16_t type:8;
#endif
    uint16_t length:16;
} rtcp_header;

//////////////////////////////////////////////////////////////////////////
bool is_dtls(char *buf); 
bool is_rtp(char *buf);
bool is_rtcp(char *buf);

#endif