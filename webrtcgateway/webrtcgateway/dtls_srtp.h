#ifndef _WXH_DTLS_SRTP_H_
#define _WXH_DTLS_SRTP_H_

#include <glib.h>
#include <srtp/srtp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/asn1.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

#pragma comment(lib, "../opensslnew/libssl.lib")
#pragma comment(lib, "../opensslnew/libcrypto.lib")
#pragma comment(lib, "../srtp/srtp.lib")

typedef enum dtls_role 
{
    DTLS_ROLE_ACTPASS = -1,
    DTLS_ROLE_SERVER,
    DTLS_ROLE_CLIENT,
} dtls_role;

typedef enum dtls_state 
{
    DTLS_STATE_FAILED = -1,
    DTLS_STATE_CREATED,
    DTLS_STATE_TRYING,
    DTLS_STATE_CONNECTED,
} dtls_state;

class nice_agent;

class dtls_srtp
{
public:
    static int init(const char* server_pem, const char* server_key);
    static char* get_local_fingerprint();
    static void release();

    dtls_srtp();
    ~dtls_srtp();
    int create(nice_agent* niceagent, int streamid, int componetid, dtls_role role);
    void destroy();
    void handshake();
    void fd_bridge();
    void incoming_msg(char *buf, uint16_t len);
    bool srtp_unprotect_rtp_buf(void *srtp_hdr, int *len_ptr);
    bool srtp_unprotect_rtcp_buf(void *srtp_hdr, int *len_ptr);

public:
    /*! \brief DTLS role of the gateway for this stream: 1=client, 0=server */
    dtls_role m_dtls_role;
    /*! \brief DTLS state of this component: -1=failed, 0=nothing, 1=trying, 2=connected */
    dtls_state m_dtls_state;
    /*! \brief Monotonic time of when the DTLS state has switched to connected */
    gint64 dtls_connected;
    /*! \brief SSL context used for DTLS for this component */
    SSL *ssl;
    /*! \brief Read BIO (incoming DTLS data) */
    BIO *read_bio;
    /*! \brief Write BIO (outgoing DTLS data) */
    BIO *write_bio;
    /*! \brief Filter BIO (fix MTU fragmentation on outgoing DTLS data, if required) */
    BIO *filter_bio;
    /*! \brief Whether SRTP has been correctly set up for this component or not */
    gint srtp_valid;
    /*! \brief libsrtp context for incoming SRTP packets */
    srtp_t srtp_in;
    /*! \brief libsrtp context for outgoing SRTP packets */
    srtp_t srtp_out;
    /*! \brief libsrtp policy for incoming SRTP packets */
    srtp_policy_t remote_policy;
    /*! \brief libsrtp policy for outgoing SRTP packets */
    srtp_policy_t local_policy;
    /*! \brief Mutex to lock/unlock this libsrtp context */
    boost::mutex srtp_mutex;
    /*! \brief Whether this DTLS stack is now ready to be used for messages as well (e.g., SCTP encapsulation) */
    int ready;
    nice_agent* agent;
    int stream_id;
    int component_id;
    

    static SSL_CTX* ssl_ctx;
    static X509* ssl_cert;
    static EVP_PKEY* ssl_key;    
};


#endif