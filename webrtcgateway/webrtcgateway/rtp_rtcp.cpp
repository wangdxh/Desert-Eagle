#include "rtp_rtcp.h"

bool is_dtls(char *buf) 
{
    return ((*buf >= 20) && (*buf <= 64));
}
bool is_rtp(char *buf) 
{
    rtp_header *header = (rtp_header *)buf;
    return ((header->type < 64) || (header->type >= 96));
}

bool is_rtcp(char *buf) 
{
    rtp_header *header = (rtp_header *)buf;
    return ((header->type >= 64) && (header->type < 96));
}


