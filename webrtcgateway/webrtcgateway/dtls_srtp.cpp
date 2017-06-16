#include "dtls_srtp.h"
#include "nice_agent.h"
#include <list>
#include <openssl/bio.h>

#include "rtp_rtcp.h"

#ifdef WIN32
#undef X509_NAME //sb windows
#endif 

int dtls_bio_filter_write(BIO *h, const char *buf,int num);
long dtls_bio_filter_ctrl(BIO *h, int cmd, long arg1, void *arg2);
int dtls_bio_filter_new(BIO *h);
int dtls_bio_filter_free(BIO *data);

static int mtu = 1472;
void dtls_bio_filter_set_mtu(int start_mtu) 
{
    if(start_mtu < 0) {
        printf("Invalid MTU...\n");
        return;
    }
    mtu = start_mtu;
    printf("Setting starting MTU in the DTLS BIO filter: %d\n", mtu);
}
static BIO_METHOD *dtls_bio_filter_methods = NULL;
int dtls_bio_filter_init(void) 
{
    dtls_bio_filter_methods = BIO_meth_new(BIO_TYPE_FILTER | BIO_get_new_index(), "janus filter");
    if(!dtls_bio_filter_methods)
        return -1;
    BIO_meth_set_write(dtls_bio_filter_methods, dtls_bio_filter_write);
    BIO_meth_set_ctrl(dtls_bio_filter_methods, dtls_bio_filter_ctrl);
    BIO_meth_set_create(dtls_bio_filter_methods, dtls_bio_filter_new);
    BIO_meth_set_destroy(dtls_bio_filter_methods, dtls_bio_filter_free);

    return 0;
}

BIO_METHOD *BIO_dtls_filter(void) 
{
    return dtls_bio_filter_methods;
}

/* Helper struct to keep the filter state */
class dtls_bio_filter 
{
public:
    dtls_bio_filter()
    {

    }
    ~dtls_bio_filter()
    {
        packets.clear();
    }
    //GList *packets;
    std::list < int > packets;    
    boost::mutex mutex;
} ;


int dtls_bio_filter_new(BIO *bio) 
{
    /* Create a filter state struct */
    dtls_bio_filter *filter = new dtls_bio_filter();//(dtls_bio_filter *)g_malloc0(sizeof(dtls_bio_filter));
    //filter->packets = NULL;
    //mutex_init(&filter->mutex);

    BIO_set_init(bio, 1);
    BIO_set_data(bio, filter);

    return 1;
}

int dtls_bio_filter_free(BIO *bio) 
{
    if(bio == NULL)
        return 0;
    
    dtls_bio_filter *filter = (dtls_bio_filter *)BIO_get_data(bio);

    if(filter != NULL) 
    {
        delete filter;
    }
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    bio->ptr = NULL;
    bio->init = 0;
    bio->flags = 0;
#else
    BIO_set_data(bio, NULL);
    BIO_set_init(bio, 0);
#endif
    return 1;
}

int dtls_bio_filter_write(BIO *bio, const char *in, int inl) {
    printf("dtls_bio_filter_write: %p, %d\n", in, inl);

    long ret = BIO_write(BIO_next(bio), in, inl);

    printf("  -- %ld\n", ret);

    /* Keep track of the packet, as we'll advertize them one by one after a pending check */
    dtls_bio_filter *filter = (dtls_bio_filter *)BIO_get_data(bio);

    if(filter != NULL)
    {
        boost::mutex::scoped_lock lock(filter->mutex);
        //filter->packets = g_list_append(filter->packets, GINT_TO_POINTER(ret));
        filter->packets.push_back(ret);
        printf("New list length: %d\n", filter->packets.size());
    }
    return ret;
}

long dtls_bio_filter_ctrl(BIO *bio, int cmd, long num, void *ptr) {
    switch(cmd) 
    {
    case BIO_CTRL_FLUSH:
        /* The OpenSSL library needs this */
        return 1;
    case BIO_CTRL_DGRAM_QUERY_MTU:
        /* Let's force the MTU that was configured */
        printf("Advertizing MTU: %d\n", mtu);
        return mtu;
    case BIO_CTRL_WPENDING:
        return 0L;
    case BIO_CTRL_PENDING: 
        {
            /* We only advertize one packet at a time, as they may be fragmented */
            dtls_bio_filter *filter = (dtls_bio_filter *)BIO_get_data(bio);

            if(filter == NULL) {return 0;}

            boost::mutex::scoped_lock lock(filter->mutex);

            //mutex_lock(&filter->mutex);
            if(0 == filter->packets.size()) {return 0;}            
            
            /*GList *first = g_list_first(filter->packets);
            filter->packets = g_list_remove_link(filter->packets, first);
            int pending = GPOINTER_TO_INT(first->data);*/
            int pending = filter->packets.front();
            filter->packets.pop_front();
            /*g_list_free(first);
            mutex_unlock(&filter->mutex);*/

            /* We return its size so that only part of the buffer is read from the write BIO */
            return pending;
        }
    default:
        printf("dtls_bio_filter_ctrl: %d\n", cmd);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
static const char *srtp_error[] =
{
    "err_status_ok",
    "err_status_fail",
    "err_status_bad_param",
    "err_status_alloc_fail",
    "err_status_dealloc_fail",
    "err_status_init_fail",
    "err_status_terminus",
    "err_status_auth_fail",
    "err_status_cipher_fail",
    "err_status_replay_fail",
    "err_status_replay_old",
    "err_status_algo_fail",
    "err_status_no_such_op",
    "err_status_no_ctx",
    "err_status_cant_check",
    "err_status_key_expired",
    "err_status_socket_err",
    "err_status_signal_err",
    "err_status_nonce_bad",
    "err_status_read_fail",
    "err_status_write_fail",
    "err_status_parse_err",
    "err_status_encode_err",
    "err_status_semaphore_err",
    "err_status_pfkey_err",
};
const gchar *get_srtp_error(int error) {
    if(error < 0 || error > 24)
        return NULL;
    return srtp_error[error];
}


const gchar *get_dtls_srtp_state(dtls_state state) {
    switch(state) {
    case DTLS_STATE_CREATED:
        return "created";
    case DTLS_STATE_TRYING:
        return "trying";
    case DTLS_STATE_CONNECTED:
        return "connected";
    case DTLS_STATE_FAILED:
        return "failed";
    default:
        return NULL;
    }
    return NULL;
}




/* DTLS stuff */
#define DTLS_CIPHERS	"ALL:NULL:eNULL:aNULL"
/* Duration for the self-generated certs: 1 year */
#define DTLS_AUTOCERT_DURATION	60*60*24*365

/* SRTP stuff (http://tools.ietf.org/html/rfc3711) */
#define SRTP_MASTER_KEY_LENGTH	16
#define SRTP_MASTER_SALT_LENGTH	14
#define SRTP_MASTER_LENGTH (SRTP_MASTER_KEY_LENGTH + SRTP_MASTER_SALT_LENGTH)

static gchar local_fingerprint[160];
gchar *dtls_get_local_fingerprint(void) 
{
    return (gchar *)local_fingerprint;
}

int dtls_verify_callback(int preverify_ok, X509_STORE_CTX *ctx) 
{
    /* We just use the verify_callback to request a certificate from the client */
    return 1;
}

static int dtls_generate_keys(X509** certificate, EVP_PKEY** private_key) 
{
    static const int num_bits = 2048;
    BIGNUM* bne = NULL;
    RSA* rsa_key = NULL;
    X509_NAME* cert_name = NULL;

    printf("Generating DTLS key / cert\n");

    /* Create a big number object. */
    bne = BN_new();
    if (!bne) 
    {
        printf("BN_new() failed\n");
        goto error;
    }

    if (!BN_set_word(bne, RSA_F4)) {  /* RSA_F4 == 65537 */
        printf("BN_set_word() failed\n");
        goto error;
    }

    /* Generate a RSA key. */
    rsa_key = RSA_new();
    if (!rsa_key) {
        printf("RSA_new() failed\n");
        goto error;
    }

    /* This takes some time. */
    if (!RSA_generate_key_ex(rsa_key, num_bits, bne, NULL)) 
    {
        printf("RSA_generate_key_ex() failed\n");
        goto error;
    }

    /* Create a private key object (needed to hold the RSA key). */
    *private_key = EVP_PKEY_new();
    if (!*private_key) 
    {
        printf("EVP_PKEY_new() failed\n");
        goto error;
    }

    if (!EVP_PKEY_assign_RSA(*private_key, rsa_key)) 
    {
        printf("EVP_PKEY_assign_RSA() failed\n");
        goto error;
    }
    /* The RSA key now belongs to the private key, so don't clean it up separately. */
    rsa_key = NULL;

    /* Create the X509 certificate. */
    *certificate = X509_new();
    if (!*certificate) 
    {
        printf("X509_new() failed\n");
        goto error;
    }

    /* Set version 3 (note that 0 means version 1). */
    X509_set_version(*certificate, 2);

    /* Set serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(*certificate), (long)g_random_int());

    /* Set valid period. */
    X509_gmtime_adj(X509_get_notBefore(*certificate), -1 * DTLS_AUTOCERT_DURATION);  /* -1 year */
    X509_gmtime_adj(X509_get_notAfter(*certificate), DTLS_AUTOCERT_DURATION);  /* 1 year */

    /* Set the public key for the certificate using the key. */
    if (!X509_set_pubkey(*certificate, *private_key)) 
    {
        printf("X509_set_pubkey() failed\n");
        goto error;
    }

    /* Set certificate fields. */
    cert_name = X509_get_subject_name(*certificate);
    if (!cert_name) 
    {
        printf("X509_get_subject_name() failed\n");
        goto error;
    }
    X509_NAME_add_entry_by_txt(cert_name, "O", MBSTRING_ASC, (const unsigned char*)"Janus", -1, -1, 0);
    X509_NAME_add_entry_by_txt(cert_name, "CN", MBSTRING_ASC, (const unsigned char*)"Janus", -1, -1, 0);

    /* It is self-signed so set the issuer name to be the same as the subject. */
    if (!X509_set_issuer_name(*certificate, cert_name)) 
    {
        printf("X509_set_issuer_name() failed\n");
        goto error;
    }

    /* Sign the certificate with the private key. */
    if (!X509_sign(*certificate, *private_key, EVP_sha1())) 
    {
        printf("X509_sign() failed\n");
        goto error;
    }

    /* Free stuff and resurn. */
    BN_free(bne);
    return 0;

error:
    if (bne)
        BN_free(bne);
    if (rsa_key && !*private_key)
        RSA_free(rsa_key);
    if (*private_key)
        EVP_PKEY_free(*private_key);  /* This also frees the RSA key. */
    if (*certificate)
        X509_free(*certificate);
    return -1;
}


static int dtls_load_keys(const char* server_pem, const char* server_key, X509** certificate, EVP_PKEY** private_key) 
{
    X509* cert = X509_new();
    BIO* bio_cert = BIO_new_file(server_pem, "rb");
    PEM_read_bio_X509(bio_cert, &cert, NULL, NULL);
    BIO_free(bio_cert);        
    *certificate = cert;

    EVP_PKEY* key = EVP_PKEY_new();
    BIO* bio_key = BIO_new_file(server_key, "rb");
    PEM_read_bio_PrivateKey(bio_key, &key, NULL, NULL);
    BIO_free(bio_key);
    *private_key = key;

    return 0;

error:
    if (*certificate) {
        X509_free(*certificate);
        *certificate = NULL;
    }
    if (*private_key) {
        EVP_PKEY_free(*private_key);
        *private_key = NULL;
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////
SSL_CTX* dtls_srtp::ssl_ctx = NULL;
X509* dtls_srtp::ssl_cert = NULL;
EVP_PKEY* dtls_srtp::ssl_key = NULL;

int dtls_srtp::init(const char* server_pem, const char* server_key)
{
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    printf(" error ... OpenSSL pre-1.1.0\n");    
#else
    printf("OpenSSL >= 1.1.0\n");
#endif
    
    ssl_ctx = SSL_CTX_new(DTLS_method());

    if(!ssl_ctx) 
    {
        printf("Ops, error creating DTLS context?\n");
        return -1;
    }
    SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, dtls_verify_callback);
    SSL_CTX_set_tlsext_use_srtp(ssl_ctx, "SRTP_AES128_CM_SHA1_80");	/* FIXME Should we support something else as well? */

    if (!server_pem && !server_key) 
    {
        printf("No cert/key specified, autogenerating some...\n");
        if (dtls_generate_keys(&ssl_cert, &ssl_key) != 0) {
            printf("Error generating DTLS key/certificate\n");
            return -2;
        }
    }
    else if (!server_pem || !server_key) 
    {
        printf("DTLS certificate and key must be specified\n");
        return -2;
    }
    else if (dtls_load_keys(server_pem, server_key, &ssl_cert, &ssl_key) != 0) 
    {
        return -3;
    }

    if(!SSL_CTX_use_certificate(ssl_ctx, ssl_cert)) 
    {
        printf("Certificate error (%s)\n", ERR_reason_error_string(ERR_get_error()));
        return -4;
    }
    if(!SSL_CTX_use_PrivateKey(ssl_ctx, ssl_key)) 
    {
        printf("Certificate key error (%s)\n", ERR_reason_error_string(ERR_get_error()));
        return -5;
    }
    if(!SSL_CTX_check_private_key(ssl_ctx)) 
    {
        printf("Certificate check error (%s)\n", ERR_reason_error_string(ERR_get_error()));
        return -6;
    }
    SSL_CTX_set_read_ahead(ssl_ctx,1);

    unsigned int size;
    unsigned char fingerprint[EVP_MAX_MD_SIZE];
    if(X509_digest(ssl_cert, EVP_sha256(), (unsigned char *)fingerprint, &size) == 0) 
    {
        printf("Error converting X509 structure (%s)\n", ERR_reason_error_string(ERR_get_error()));
        return -7;
    }
    char *lfp = (char *)&local_fingerprint;
    unsigned int i = 0;
    for(i = 0; i < size; i++) 
    {
        g_snprintf(lfp, 4, "%.2X:", fingerprint[i]);
        lfp += 3;
    }
    *(lfp-1) = 0;

    printf("Fingerprint of our certificate: %s\n", local_fingerprint);
    SSL_CTX_set_cipher_list(ssl_ctx, DTLS_CIPHERS);

    if(dtls_bio_filter_init() < 0) 
    {
        printf("Error initializing BIO filter\n");
        return -8;
    }

    /* Initialize libsrtp */
    if(srtp_init() != err_status_ok) 
    {
        printf("Ops, error setting up libsrtp?\n");
        return 5;
    }
    return 0;
}

char* dtls_srtp::get_local_fingerprint()
{
    return local_fingerprint;
}

void dtls_srtp::release()
{
    if (ssl_cert != NULL) {
        X509_free(ssl_cert);
        ssl_cert = NULL;
    }
    if (ssl_key != NULL) {
        EVP_PKEY_free(ssl_key);
        ssl_key = NULL;
    }
    if (ssl_ctx != NULL) {
        SSL_CTX_free(ssl_ctx);
        ssl_ctx = NULL;
    }    
}


void dtls_callback(const SSL *ssl, int where, int ret) 
{
    /* We only care about alerts */
    if (!(where & SSL_CB_ALERT)) 
    {
        return;
    }
    printf("......................................dtls error from callback.........................\r\n");
}

dtls_srtp::dtls_srtp()
{
    memset(&remote_policy, 0x0, sizeof(remote_policy));
    memset(&local_policy, 0x0, sizeof(local_policy));
    agent = nullptr;

}

dtls_srtp::~dtls_srtp()
{
    destroy();
}

int dtls_srtp::create(nice_agent* niceagent, int streamid, int componetid, dtls_role role) 
{	
    this->agent = niceagent;
    stream_id = streamid;
    component_id = componetid;

	/* Create SSL context, at last */
	srtp_valid = 0;
	ssl = SSL_new(ssl_ctx);
	if(!ssl) 
    {
		printf("[%I64u]     Error creating DTLS session! (%s)\n", ERR_reason_error_string(ERR_get_error()));		
		return NULL;
	}
	SSL_set_ex_data(ssl, 0, this);
	SSL_set_info_callback(ssl, dtls_callback);
	read_bio = BIO_new(BIO_s_mem());
	if(!read_bio) 
    {
		printf(" Error creating read BIO! (%s)\n",  ERR_reason_error_string(ERR_get_error()));		
		return NULL;
	}
	BIO_set_mem_eof_return(read_bio, -1);
	write_bio = BIO_new(BIO_s_mem());
	if(!write_bio) 
    {
		printf("Error creating write BIO! (%s)\n", ERR_reason_error_string(ERR_get_error()));		
		return NULL;
	}
	BIO_set_mem_eof_return(write_bio, -1);
	/* The write BIO needs our custom filter, or fragmentation won't work */
	filter_bio = BIO_new(BIO_dtls_filter());
	if(!filter_bio) 
    {
		printf("Error creating filter BIO! (%s)\n", ERR_reason_error_string(ERR_get_error()));		
		return NULL;
	}
	/* Chain filter and write BIOs */
	BIO_push(filter_bio, write_bio);
	/* Set the filter as the BIO to use for outgoing data */
	SSL_set_bio(ssl, read_bio, filter_bio);
	m_dtls_role = role;
	if(m_dtls_role == DTLS_ROLE_CLIENT) 
    {
		printf("Setting connect state (DTLS client)\n");
		SSL_set_connect_state(ssl);
	}
    else 
    {
		printf("Setting accept state (DTLS server)\n");
		SSL_set_accept_state(ssl);
	}
	/* https://code.google.com/p/chromium/issues/detail?id=406458
	 * Specify an ECDH group for ECDHE ciphers, otherwise they cannot be
	 * negotiated when acting as the server. Use NIST's P-256 which is
	 * commonly supported.
	 */
	EC_KEY* ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
	if(ecdh == NULL) 
    {
		printf("Error creating ECDH group! (%s)\n", ERR_reason_error_string(ERR_get_error()));		
		return NULL;
	}
	SSL_set_options(ssl, SSL_OP_SINGLE_ECDH_USE);
	SSL_set_tmp_ecdh(ssl, ecdh);
	EC_KEY_free(ecdh);

	ready = 0;

	/* Done */
	dtls_connected = 0;	
	return TRUE;
}

void dtls_srtp::destroy()
{    
    ready = 0;

    /* Destroy DTLS stack and free resources */
    
    if(ssl != NULL) 
    {
        SSL_free(ssl);
        ssl = NULL;
    }
    /* BIOs are destroyed by SSL_free */
    read_bio = NULL;
    write_bio = NULL;
    filter_bio = NULL;
    if(srtp_valid) 
    {
        if(srtp_in) 
        {            
            srtp_dealloc(srtp_in);
            srtp_in = NULL;
        }
        if(srtp_out) 
        {
            srtp_dealloc(srtp_out);
            srtp_out = NULL;
        }
        /* FIXME What about dtls->remote_policy and dtls->local_policy? */
    }
}

void dtls_srtp::handshake() 
{
	if(ssl == NULL) {return;}
    
    if(DTLS_STATE_CREATED == m_dtls_state)
    {
        m_dtls_state = DTLS_STATE_TRYING;
    }
	SSL_do_handshake(ssl);
	fd_bridge();
}

void dtls_srtp::fd_bridge() 
{
	if(!write_bio) 
    {
		printf("No handle/agent/bio, no DTLS bridge...\n");
		return;
	}
	int pending = BIO_ctrl_pending(filter_bio);
	while(pending > 0) 
    {
		printf("Going to send DTLS data: %d bytes\n", pending);
		char* outgoing = new char[pending];
        int noutgoinglen = pending;
		int out = BIO_read(write_bio, outgoing, noutgoinglen);
		printf("Read %d bytes from the write_BIO...\n", out);
		if(out > 1500) 
        {
			/* FIXME Just a warning for now, this will need to be solved with proper fragmentation */
			printf("The DTLS stack is trying to send a packet of %d bytes, this may be larger than the MTU and get dropped!\n", out);
		}
		//int bytes = nice_agent_send(agent->agent, component->stream_id, component->component_id, out, outgoing);
        int bytes = agent->send_data(stream_id, component_id, out, outgoing);
		if(bytes < out) 
        {
			printf("Error sending DTLS message on component %d of stream %d (%d)\n", component_id, stream_id, bytes);
		} else {
			printf("[%I64u] >> >> ... and sent %d of those bytes on the socket\n", bytes);
		}
		/* Update stats (TODO Do the same for the last second window as well)
		 * FIXME: the Data stats includes the bytes used for the handshake */
		if(bytes > 0) 
        {
// 			component->out_stats.data_packets++;
// 			component->out_stats.data_bytes += bytes;
		}
		delete [] outgoing;
		/* Check if there's anything left to send (e.g., fragmented packets) */
		pending = BIO_ctrl_pending(filter_bio);
	}
}

void dtls_srtp::incoming_msg(char *buf, uint16_t len)
{        
    if(!ssl || !read_bio) 
    {
        printf("No DTLS stuff for component %d in stream %d??\n", component_id, stream_id);
        return;
    }
    fd_bridge();
    int written = BIO_write(read_bio, buf, len);
    if(written != len) 
    {
        printf("Only written %d/%d of those bytes on the read BIO...\n", written, len);
    } else {
        printf("Written %d bytes on the read BIO...\n", written);
    }
    fd_bridge();
    /* Try to read data */
    char data[1500];	/* FIXME */
    memset(&data, 0, 1500);
    int read = SSL_read(ssl, &data, 1500);
    printf("... and read %d of them from SSL...\n", read);
    if(read < 0) 
    {
        unsigned long err = SSL_get_error(ssl, read);
        if(err == SSL_ERROR_SSL) 
        {
            /* Ops, something went wrong with the DTLS handshake */
            char error[200];
            ERR_error_string_n(ERR_get_error(), error, 200);
            printf("Handshake error: %s\n", error);
            return;
        }
    }
    fd_bridge();
    
    if(!SSL_is_init_finished(ssl)) 
    {
        /* Nothing else to do for now */
        printf("Initialization not finished yet...\n");
        return;
    }
    if(ready) 
    {
        /* There's data to be read? */
        printf("Any data available?\n");

        if(read > 0) 
        {
            printf("Data available but Data Channels support disabled...\n");
        }
    }
    else
    {        
        /* Check the remote fingerprint */
        X509 *rcert = SSL_get_peer_certificate(ssl);
        if(!rcert) 
        {
            printf("No remote certificate?? (%s)\n", ERR_reason_error_string(ERR_get_error()));
        } 
        else 
        {
            unsigned int rsize;
            unsigned char rfingerprint[EVP_MAX_MD_SIZE];
            char remote_fingerprint[160];
            char *rfp = (char *)&remote_fingerprint;
            //if(stream->remote_hashing && !strcasecmp(stream->remote_hashing, "sha-1")) {
            if(0)
            {
                printf("Computing sha-1 fingerprint of remote certificate...\n");
                X509_digest(rcert, EVP_sha1(), (unsigned char *)rfingerprint, &rsize);
            }
            else 
            {
                printf("Computing sha-256 fingerprint of remote certificate...\n");
                X509_digest(rcert, EVP_sha256(), (unsigned char *)rfingerprint, &rsize);
            }
            X509_free(rcert);
            rcert = NULL;
            unsigned int i = 0;
            for(i = 0; i < rsize; i++) {
                g_snprintf(rfp, 4, "%.2X:", rfingerprint[i]);
                rfp += 3;
            }
            *(rfp-1) = 0;

            printf("Remote fingerprint (%s) of the client is %s\n", "sha-256", remote_fingerprint);
            //if(!strcasecmp(remote_fingerprint, stream->remote_fingerprint ? stream->remote_fingerprint : "(none)")) 
            if(1)
            {
                printf("Fingerprint is a match!\n");
                m_dtls_state = DTLS_STATE_CONNECTED;
                dtls_connected = 1;
            } else {
                /* FIXME NOT a match! MITM? */
                printf("Fingerprint is NOT a match! got %s, expected %s\n", remote_fingerprint, remote_fingerprint);
                m_dtls_state = DTLS_STATE_FAILED;
                goto done;
            }
            if(DTLS_STATE_CONNECTED == m_dtls_state)
            {                
                {
                    /* Complete with SRTP setup */
                    unsigned char material[SRTP_MASTER_LENGTH*2];
                    unsigned char *local_key, *local_salt, *remote_key, *remote_salt;
                    /* Export keying material for SRTP */
                    if (!SSL_export_keying_material(ssl, material, SRTP_MASTER_LENGTH*2, "EXTRACTOR-dtls_srtp", 19, NULL, 0, 0)) 
                    {
                        /* Oops... */
                        printf("Oops, couldn't extract SRTP keying material for component %d in stream %d?? (%s)\n", component_id, stream_id, 
                                  ERR_reason_error_string(ERR_get_error()));
                        goto done;
                    }
                    /* Key derivation (http://tools.ietf.org/html/rfc5764#section-4.2) */
                    if(m_dtls_role == DTLS_ROLE_CLIENT) 
                    {
                        local_key = material;
                        remote_key = local_key + SRTP_MASTER_KEY_LENGTH;
                        local_salt = remote_key + SRTP_MASTER_KEY_LENGTH;
                        remote_salt = local_salt + SRTP_MASTER_SALT_LENGTH;
                    }
                    else 
                    {
                        remote_key = material;
                        local_key = remote_key + SRTP_MASTER_KEY_LENGTH;
                        remote_salt = local_key + SRTP_MASTER_KEY_LENGTH;
                        local_salt = remote_salt + SRTP_MASTER_SALT_LENGTH;
                    }
                    /* Build master keys and set SRTP policies */
                    /* Remote (inbound) */                    
                    crypto_policy_set_rtp_default(&(remote_policy.rtp));
                    crypto_policy_set_rtcp_default(&(remote_policy.rtcp));
                    remote_policy.ssrc.type = ssrc_any_inbound;
                    unsigned char remote_policy_key[SRTP_MASTER_LENGTH];
                    remote_policy.key = (unsigned char *)&remote_policy_key;
                    memcpy(remote_policy.key, remote_key, SRTP_MASTER_KEY_LENGTH);
                    memcpy(remote_policy.key + SRTP_MASTER_KEY_LENGTH, remote_salt, SRTP_MASTER_SALT_LENGTH);
#if HAS_DTLS_WINDOW_SIZE
                    remote_policy.window_size = 128;
                    remote_policy.allow_repeat_tx = 0;
#endif
                    remote_policy.next = NULL;
                    /* Local (outbound) */
                    crypto_policy_set_rtp_default(&(local_policy.rtp));
                    crypto_policy_set_rtcp_default(&(local_policy.rtcp));
                    local_policy.ssrc.type = ssrc_any_outbound;
                    unsigned char local_policy_key[SRTP_MASTER_LENGTH];
                    local_policy.key = (unsigned char *)&local_policy_key;
                    memcpy(local_policy.key, local_key, SRTP_MASTER_KEY_LENGTH);
                    memcpy(local_policy.key + SRTP_MASTER_KEY_LENGTH, local_salt, SRTP_MASTER_SALT_LENGTH);
#if HAS_DTLS_WINDOW_SIZE
                    local_policy.window_size = 128;
                    local_policy.allow_repeat_tx = 0;
#endif
                    local_policy.next = NULL;
                    /* Create SRTP sessions */
                    err_status_t res = srtp_create(&srtp_in, &remote_policy);
                    if(res != err_status_ok) 
                    {
                        /* Something went wrong... */
                        printf("Oops, error creating inbound SRTP session for component %d in stream %d??\n", component_id, stream_id);
                        printf("  -- %d (%s)\n", res, get_srtp_error(res));
                        goto done;
                    }
                    printf("Created inbound SRTP session for component %d in stream %d\n", component_id, stream_id);
                    res = srtp_create(&(srtp_out), &(local_policy));
                    if(res != err_status_ok) 
                    {
                        /* Something went wrong... */
                        printf("Oops, error creating outbound SRTP session for component %d in stream %d??\n", component_id, stream_id);
                        printf("  -- %d (%s)\n", res, get_srtp_error(res));
                        goto done;
                    }
                    srtp_valid = 1;
                    printf("Created outbound SRTP session for component %d in stream %d\n", component_id, stream_id);
                }
                ready = 1;
            }
done:
            if (srtp_valid)
            {
                // ready u can send data
                printf("great.......................srtp................................it is ok!\r\n");
            }
            else
            {
                printf("sad................................srtp......................................it is wrong!\r\n");
                dtls_callback(ssl, SSL_CB_ALERT, 0);
            }            
        }
    }
}

bool dtls_srtp::srtp_unprotect_rtp_buf(void *srtp_hdr, int *len_ptr)
{
    if (0 == this->srtp_valid)
    {
        return false;
    }
    int nlen = *len_ptr;
    err_status_t res = srtp_unprotect(srtp_in, srtp_hdr, len_ptr);
    if (res != err_status_ok)
    {
        if(res != err_status_replay_fail && res != err_status_replay_old) 
        {
            printf("SRTP unprotect error: rtp \n");
        }
        return false;
    }
    rtp_header* pheader = (rtp_header*) srtp_hdr;
    printf("rtp info: type is %d beforelen %d afterlen %d \r\n", pheader->type, nlen, *len_ptr);
    return true;
}

bool dtls_srtp::srtp_unprotect_rtcp_buf(void *srtp_hdr, int *len_ptr)
{
    if (0 == this->srtp_valid)
    {
        return false;
    }
    int nlen = *len_ptr;
    err_status_t res = srtp_unprotect_rtcp(srtp_in, srtp_hdr, len_ptr);
    if (res != err_status_ok)
    {
        printf("SRTP unprotect error: rtcp\n");     
        return false;
    }
    rtcp_header* pheader = (rtcp_header*) srtp_hdr;
    printf("rtcp info: type is %d beforelen %d afterlen %d \r\n", pheader->type, nlen, *len_ptr);
    return true;
}

