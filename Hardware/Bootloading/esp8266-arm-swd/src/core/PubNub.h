/***************************************************************************
* 
* File              : pubnub.h
* Author			: Kurt E. Clothier
* Date				: June 17, 2015
* Modified			: June 25, 2015
*
* Description       : Pubnub header file
*
* Compiler			: Xtensa Tools - GCC
* Hardware			: ESP8266
*
* Preprocessor Info	: http://gcc.gnu.org/onlinedocs/gcc-2.95.3/cpp_1.html
* More Information	: http://www.pubnub.com/
*					: http://www.esp8266.com/
*
****************************************************************************/
//
// The original program is written by Kurt E. Clothier.
// https://github.com/Kurt-E-Clothier/pubnub-esp8266/tree/master/esp8266-hellworld-demo/pubnub
//
// revised by TANABE Makoto (portion of the "DSAS" identifier)
//
// - Corresponding to "Arduino IDE for ESP8266"
//   https://github.com/esp8266/Arduino/
//
// - Enable to specify the callback function for completion of pubnub_publish()
//
// - Corresponding to HTTPS
//
// 2015 KLab Inc.
//

#ifndef _PUBNUB_H_
#define _PUBNUB_H_

#define DSAS
/**************************************************************************
	Included Files
***************************************************************************/
#ifdef DSAS
#include <ESP8266WiFi.h>
#else
#include "esp8266.h"
#endif
#include "string.h"

#ifdef DSAS
//#define USE_SSL
#define SSL_BUFFER_SIZE  8192

extern "C" {
#include "c_types.h"
#include "ip_addr.h"
#include "os_type.h"
#include "user_interface.h"
#include "osapi.h"
#include "espconn.h"
#include "mem.h"
}

#define IFA ICACHE_FLASH_ATTR

// Define to print debug statements over UART using DEBUG_PRINT()
//#define DEBUG_PRINT_	1

// Only prints lines for debugging purposes
#ifdef DEBUG_PRINT_
#	define DEBUG_PRINT(S)	Serial.printf(S)
//#	define DEBUG_PRINT	Serial.printf
#else
#	define DEBUG_PRINT
#endif

#define sprintf os_sprintf
#define espconn_send espconn_sent
#define espconn_secure_send espconn_secure_sent
#endif // DSAS

/**************************************************************************
	Definitions
***************************************************************************/
#define PUBNUB_ORIGIN	"pubsub.pubnub.com"
#if defined(DSAS) && defined(USE_SSL)
#define PUBNUB_DOMAIN	"https://pubsub.pubnub.com"
#define PUBNUB_ORIGIN_PORT	443
#else
#define PUBNUB_DOMAIN	"http://pubsub.pubnub.com"
#define PUBNUB_ORIGIN_PORT	80
#endif

#define PUBNUB_CHANNEL_MAXLEN	128
#define PUBNUB_BUF_MAXLEN		256
#define PUBNUB_REPLY_MAXLEN		512

#define PUBNUB_CTX_MAX	2

#define PUBNUB_TIMER_VALUE	100


/**************************************************************************
	Custom Types and Structures
***************************************************************************/

typedef void (*Pubnub_subscribeCB)(char *message);
#ifdef DSAS
typedef void (*Pubnub_publishCB)(char *message);
#endif

// Espconn connection type
typedef struct espconn Espconn;

// Pubnub Transaction States
enum pubnub_trans {
	PBTT_NONE,		// No Transactions at all
	PBTT_INIT_SUB,	// Initial subscribe to get timetoken
	PBTT_SUBSCRIBE,	// Subscribe Transaction
	PBTT_PUBLISH,	// Publish Transaction
	PBTT_LEAVE,		// Leave (channel(s)) transaction
};

// Pubnub States
enum pubnub_state {
	PS_IDLE,			// Line is Idle
	PS_WAIT_DNS,		// Waiting for DNS 
	PS_WAIT_CONNECT,	// Connected
	PS_WAIT_SEND,		// Waiting for message to send
	PS_WAIT_RECV,		// Waiting for message to be received
	PS_RECV,			//
};

// Pubnub Last Result Codes
enum pubnub_res {
	PNR_OK,					// Success - transaction finished successfully
	PNR_TIMEOUT,			// Time out before the request has completed
	PNR_IO_ERROR,			// Communication error (network or HTTP response format)
	PNR_HTTP_ERROR,			// HTTP error
	PNR_FORMAT_ERROR,		// Undexpected input in received JSON
	PNR_CANCELLED,			// Request cancelled by user
	PNR_STARTED,			// Transaction started, await the outcome via process message
	PNR_IN_PROGRESS,		// Transaction (already) ongoing, can't start a new trans
	PNR_RX_BUFF_NOT_EMPTY,	// Recv buffer (from last trans) not read, new subscription no allowed
	PNR_TX_BUFF_TOO_SMALL	// The buffer is too small, need to increease #PUBNUB_BUF_MAXLEN
};

// Pubnub Structure - holds all relevant keys and attriutes
typedef struct pubnub {
	const char *publish_key, *subscribe_key;
	const char *uuid, *auth;
	const char *subscribe_channel;
	char timetoken[64];

	enum pubnub_res last_result;	// Result of last Pubnub transaction
	enum pubnub_trans trans;		// transaction state
	enum pubnub_state state;		// pubnub state

	// These buffers hold information about http GET messages and replies
	char http_msgOut[PUBNUB_BUF_MAXLEN];
	char http_reply[PUBNUB_REPLY_MAXLEN];
	char http_msgIn[PUBNUB_REPLY_MAXLEN];
	unsigned http_len;

	// These buffers hold data from the last attpemted publish
	char publish_message[PUBNUB_BUF_MAXLEN];
	char publish_channel[PUBNUB_CHANNEL_MAXLEN];

	Pubnub_subscribeCB	subscribeCB;
#ifdef DSAS
	Pubnub_publishCB publishCB;
#endif

	/* These in-string offsets are used for yielding messages received
     * by subscribe - the beginning of last yielded message and total
     * length of message buffer, and the same for channels.
     */
	unsigned short msg_ofs, msg_end, chan_ofs, chan_end;
} pubnub_t, Pubnub;


/**************************************************************************
	PubNub Client API
***************************************************************************/

/** 
 * Creates a connection to Pubnub
 * This should be called when a network connection is established!
 */
void IFA pubnub_connect(void);


/** 
 * Initialize the PubNub Connection
 *
 * @param	*publish_key		publish key to use in this connection
 * @param	*subscribe_key		subscribe_key key to use in this connection
 */
void IFA pubnub_init(const char *publish_key, const char *subscribe_key);


/** 
 * Publish message on channel using Pubnub.
 * JSON type message should be used, Unicode characters are injected automatically...
 * Example:
 *	sprintf(buffer, "\"TEXT\"");
 *		-- OR --
 *	sprintf(buffer, "{\"TEXT\":[[\"TEXT\", \"%d\"]]}", variable);
 *		-- THEN --
 *	pubnub_publish(channel, buffer);
 *
 * @param	*channel		pubnub channel where message is published
 * @param	*message		message to be published
 * @return					<tt>true</tt> if no errors encountered
 */
#ifdef DSAS
bool IFA pubnub_publish(const char *channel, const char *message, Pubnub_publishCB pubCB);
#else
bool IFA pubnub_publish(const char *channel, const char *message);
#endif

/** 
 * Subscribe to a Pubnub channel
 *	- typedef void (*Pubnub_subscribeCB)(char *message);
 *
 * Due to the limitations of single-threading, this function
 *	must be called manually each time a subscribe is desired.
 *
 * The library can be altered to automatically handle the subscriptions,
 *	but you lose the ability to publish at will.
 *
 * @param	*channel			pubnub channel to listen on
 * @param	Pubnub_subscribeCB	callback for subscribe events
 *			
 * @return		<tt>true</tt> on successful subscribe
 */
bool IFA pubnub_subscribe(const char *channel, Pubnub_subscribeCB subCB);


/** 
 * Unsubscribe to a Pubnub channel
 *
 * @param	*channel		pubnub channel to stop listening on
 * @return					<tt>false</tt> if channel not found
 */
bool IFA pubnub_unsubscribe(const char *channel);


#endif	//_PUBNUB_H_