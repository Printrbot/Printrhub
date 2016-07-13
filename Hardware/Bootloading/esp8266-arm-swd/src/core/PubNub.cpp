/***************************************************************************
* 
* File              : pubnub.c
* Author			: Kurt E. Clothier
* Date				: June 17, 2015
* Modified			: June 25, 2015
*
* Description       : Pubnub Library
*
* Compiler			: Xtensa Tools - GCC
* Hardware			: ESP8266
*
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

/**************************************************************************
	Included Files
***************************************************************************/
#include "pubnub.h"

/**************************************************************************
	Global Variables
***************************************************************************/
static Pubnub *pb;
static Espconn *pb_conn;
static volatile bool subscribed;
static volatile bool publishRequest;

/**************************************************************************
	Local Function Prototypes
***************************************************************************/
static void IFA resetTimetoken(void);
static int IFA find_string_start(char const *buf, int len);
static bool IFA split_array(char *buf);
static int IFA parse_subscribe_response(void);
static bool IFA pubnub_autoSubscribe(void);

// Network Callbacks
static void IFA pubnub_dnsFoundCB(const char *name, ip_addr_t *ip, void *arg);
static void IFA pubnub_conCB(void *arg);
static void IFA pubnub_disconCB(void *arg);
static void IFA pubnub_reconCB(void *arg, sint8 err);
static void IFA pubnub_recvCB(void *arg, char *data, unsigned short len);
static void IFA pubnub_sentCB(void *arg);
static void IFA pubnub_httpGet(const char *m);

#ifdef DSAS
static Espconn espconn;
static Pubnub pubnub;
#endif

/**************************************************************************
	Network Callback Functions
***************************************************************************/

/** 
 * Creates a connection to Pubnub
 * This should be called when a network connection is established!
 */
void IFA pubnub_connect(void)
{
#ifdef DSAS
	pb_conn = &espconn;
#else
	pb_conn = (Espconn *)os_zalloc(sizeof(Espconn));
#endif
	static ip_addr_t ip;
	DEBUG_PRINT(("Looking up Pubnub server...\n"));
	espconn_gethostbyname(pb_conn, PUBNUB_ORIGIN, &ip, pubnub_dnsFoundCB);
}

/**
 * Callback : DNS Resolved
 *
 * @param	*name		resolved host name
 * @param	*ip			ip address resolved from hostname
 * @param	*arg		espconn connection type
 */
static void IFA pubnub_dnsFoundCB(const char *name, ip_addr_t *ip, void *arg)
{
	static esp_tcp tcp;

	// Make sure this actually worked...
	if (ip == NULL) {
		DEBUG_PRINT(("Nslookup failed :/ Trying again...\n\r"));
		//network_init();
	}

		// Make sure this is coming from Pubnub connection
	else if( 0 != strcmp(name, PUBNUB_ORIGIN) ) {
		DEBUG_PRINT(("DNS Error: Non-PubNub!\n\r"));
	}

		// Everything is OK, so setup connection
	else {

		pb_conn = (struct espconn *)arg;
		DEBUG_PRINT(("Server found!\n\r"));

		// Espconn attributes
		pb_conn->type=ESPCONN_TCP;
		pb_conn->state=ESPCONN_NONE;
		pb_conn->proto.tcp=&tcp;
		pb_conn->proto.tcp->local_port=espconn_port();
		pb_conn->proto.tcp->remote_port = PUBNUB_ORIGIN_PORT;
		os_memcpy(pb_conn->proto.tcp->remote_ip, &ip->addr, 4);

		// Register Callbacks
		espconn_regist_connectcb(pb_conn, pubnub_conCB);
		espconn_regist_disconcb(pb_conn, pubnub_disconCB);
		espconn_regist_reconcb(pb_conn, pubnub_reconCB);
		espconn_regist_recvcb(pb_conn, pubnub_recvCB);
		espconn_regist_sentcb(pb_conn, pubnub_sentCB);
#if defined(DSAS) && defined(USE_SSL)
		espconn_secure_set_size(ESPCONN_CLIENT, 8192);
		espconn_secure_connect(pb_conn);
#else
		espconn_connect(pb_conn);
#endif
		pb->state = PS_WAIT_CONNECT;
	}
}

/**
 * Callback: Connection Established
 *
 * @param	*arg		espconn connection type
 */
static void IFA pubnub_conCB(void *arg)
{
	DEBUG_PRINT(("\n\rConnected to Pubnub\r\n"));
	pb->last_result = PNR_OK;
	pb->state = PS_WAIT_SEND;
	pubnub_httpGet("");
}

/**
 * Callback: Disconnected
 *
 * @param	*arg		espconn connection type
 */
static void IFA pubnub_disconCB(void *arg)
{
	DEBUG_PRINT(("\n\rDisconnected or connection error!\n\r"));
	pb->state = PS_IDLE;
	pb->last_result = PNR_IO_ERROR;
	// Attemp to reconnect...
	pubnub_connect();
}

/**
 * Callback: Reconnected
 *
 * @param	*arg		espconn connection type
 * @param	err			error
 */
static void IFA pubnub_reconCB(void *arg, sint8 err)
{
	DEBUG_PRINT(("\n\rReconnect\n\r"));
	pb->last_result = PNR_OK;
	pb->state = PS_WAIT_SEND;
	pubnub_httpGet("");
}

/**
 * Callback: Data sent
 *
 * @param	*arg		espconn connection type
 */
static void IFA pubnub_sentCB(void *arg)
{
	DEBUG_PRINT(("Data Sent\n\r"));

	if(pb->state == PS_WAIT_SEND) {
		pb->state = PS_WAIT_RECV;
		DEBUG_PRINT(("Waiting for reply...\n\r"));
	}
}

/**
 * Callback: Data received
 *
 * @param	*arg		espconn connection type
 * @param	*data		data received
 * @param	len			length of data received
 */
static void IFA pubnub_recvCB(void *arg, char *data, unsigned short len)
{
	DEBUG_PRINT(("Data Received:\n\r%s\n\r", data));
	pb->state = PS_IDLE;
	pb->last_result = PNR_OK;

	switch(pb->trans) {

		case PBTT_PUBLISH:
			pb->state = PS_IDLE;
			pb->last_result = PNR_OK;
#ifdef DSAS
			// User Callback Function
			pb->publishCB(pb->http_msgOut);
#endif
			break;

		case PBTT_INIT_SUB:
			memcpy(pb->http_reply, data, len);
			parse_subscribe_response();

			pb->last_result = PNR_OK;
			pb->state = PS_IDLE;

			// Subscribe again with valid timetoken
			pubnub_autoSubscribe();
			break;

		case PBTT_SUBSCRIBE:
			memcpy(pb->http_reply, data, len);
			parse_subscribe_response();

			pb->last_result =PNR_OK;
			pb->state = PS_IDLE;

			// User Callback Function
			pb->subscribeCB(pb->http_msgIn);
			break;

		case PBTT_NONE:
		case PBTT_LEAVE:
		default:
			break;
	}

	// Automatically republish last skipped message
	if(publishRequest)
#ifdef DSAS
		pubnub_publish(pb->publish_channel, pb->publish_message, pb->publishCB);
#else
	pubnub_publish(pb->publish_channel, pb->publish_message);
#endif
	// Uncomment the following to automatically subscribe to the channel!
	//else if (subscribed)
	//pubnub_autoSubscribe();
}

/**
 * Issue HTTP GET Request
 */
static void IFA pubnub_httpGet(const char *m)
{
	char buf[PUBNUB_BUF_MAXLEN] = { 0, };
	sprintf( buf,
			 "GET %s/%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: PubNub-ESP8266\r\nConnection: Keep-Alive\r\n\r\n",
			 PUBNUB_DOMAIN, m, PUBNUB_ORIGIN);
	DEBUG_PRINT(("\nSent: %s\n", buf));
#ifdef DSAS
#ifdef USE_SSL
	espconn_secure_send(pb_conn, (uint8*)buf, strlen(buf));
#else
	espconn_send(pb_conn, (uint8*)buf, strlen(buf));
#endif
#else
	espconn_send(pb_conn, buf, strlen(buf));
#endif
}


/**************************************************************************
	PUBNUB API
***************************************************************************/

/**
 * Initialize the PubNub Connection
 *
 * @param	*publish_key		publish key to use in this connection
 * @param	*subscribe_key		subscribe_key key to use in this connection
 */
void IFA pubnub_init(const char *publish_key, const char *subscribe_key)
{
	DEBUG_PRINT(("\n\nInitializing Pubnub\n"));
#ifdef DSAS
	pb = &pubnub;
#else
	pb = (Pubnub *)os_zalloc(sizeof(Pubnub));
#endif
	resetTimetoken();
	pb->publish_key = publish_key;
	pb->subscribe_key = subscribe_key;
	pb->subscribe_channel = NULL;
	pb->uuid = "ESP8266";
	pb->auth = NULL;
	pb->state = PS_WAIT_DNS;
	pb->last_result = PNR_IO_ERROR;
}

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
 * @param	*channel			pubnub channel where message is published
 * @param	*message			message to be published
 * @return						<tt>true</tt> if no errors encountered
 */
#ifdef DSAS
bool IFA pubnub_publish(const char *channel, const char *message, Pubnub_publishCB pubCB)
{
	pb->publishCB = pubCB;
#else
	bool IFA pubnub_publish(const char *channel, const char *message)
{
#endif
	if(pb->state != PS_IDLE) {
		if (pb->trans == PBTT_SUBSCRIBE && pb->state == PS_WAIT_RECV) {
			DEBUG_PRINT(("\nWaiting on subscription(s)!\n"));
		}
		else {
			DEBUG_PRINT(("\nLine busy, Publish delayed...\n"));
		}
		publishRequest = true;
		sprintf(pb->publish_channel, "%s", channel);
		sprintf(pb->publish_message, "%s", message);
		return false;

	}

	publishRequest = false;

	pb->trans = PBTT_PUBLISH;
#ifdef DSAS
	pb->http_len = sprintf(pb->http_msgOut, "publish/%s/%s/0/%s/0/\"%s\"",
						   pb->publish_key, pb->subscribe_key, channel, message);
#else
	pb->http_len = sprintf(pb->http_msgOut, "publish/%s/%s/0/%s/0/",
								pb->publish_key, pb->subscribe_key, channel);

	const char *pmessage = message;

	while(pmessage[0]) {
		// RFC 3986 Unreserved characters plus few safe reserved ones.
		size_t okspan = strspn(pmessage,
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~" ",=:;@[]");
		if(okspan > 0) {
			if(okspan > sizeof(pb->http_msgOut)-1 - pb->http_len) {
				pb->http_len = 0;
				return false;
			}

			memcpy(pb->http_msgOut + pb->http_len, pmessage, okspan);
			pb->http_len += okspan;
			pb->http_msgOut[pb->http_len] = 0;
			pmessage += okspan;
		}

		if(pmessage[0]) {
			// %-encode a non-ok character.
			char enc[4] = {'%',};
			enc[1] = "0123456789ABCDEF"[pmessage[0] / 16];
			enc[2] = "0123456789ABCDEF"[pmessage[0] % 16];
			if(3 > sizeof(pb->http_msgOut) - 1 - pb->http_len) {
				pb->http_len = 0;
				return false;
			}
			memcpy(pb->http_msgOut + pb->http_len, enc, 4);
			pb->http_len += 3;
			++pmessage;
		}
	}
#endif
	if(pb->last_result == PNR_OK) {
		pb->state = PS_WAIT_SEND;
		pubnub_httpGet(pb->http_msgOut);
	}
	else {
		DEBUG_PRINT(("PB Last Result Error!\n\r"));
		return false;
	}

	return true;
};

/**
 * Automatically handle subsription
 *
 * @return			<tt>true</tt> if no errors encountered
 */
static bool IFA pubnub_autoSubscribe(void)
{
	if(pb->state != PS_IDLE) {
		return false;
	}

	// First, we have to get a valid timetoken
	if (pb->timetoken[1] == '\0')
		pb->trans = PBTT_INIT_SUB;
	else
		pb->trans = PBTT_SUBSCRIBE;

	memset(pb->http_reply, 0, PUBNUB_REPLY_MAXLEN);

	pb->http_len = sprintf(pb->http_msgOut,
						   "/subscribe/%s/%s/0/%s?" "%s%s" "%s%s%s" "&pnsdk=ESP8266%s%%2F%s",
						   pb->subscribe_key,
						   pb->subscribe_channel,
						   pb->timetoken,
						   pb->uuid ? "uuid=" : "",
						   pb->uuid ? pb->uuid : "",
						   pb->uuid && pb->auth ? "&" : "",
						   pb->uuid && pb->auth ? "auth=" : "",
						   pb->uuid && pb->auth ? pb->auth : "",
						   "", "0.1");

	if(pb->last_result == PNR_OK) {
		pb->state = PS_WAIT_SEND;
		pubnub_httpGet(pb->http_msgOut);
	}
	else {
		DEBUG_PRINT(("PB Last Result Error!\n\r"));
		return false;
	}

	return true;
}

/**
 * Subscribe to a Pubnub channel
 *	- typedef void (*Pubnub_subscribeCB)(char *message);
 *
 * @param	*channel			pubnub channel to listen on
 * @param	Pubnub_subscribeCB	callback for subscribe events
 *
 * @return		<tt>true</tt> on successful subscribe
 */
bool IFA pubnub_subscribe(const char *channel, Pubnub_subscribeCB subCB)
{
	pb->subscribe_channel = channel;
	pb->subscribeCB = subCB;
	subscribed = true;
	return pubnub_autoSubscribe();
}

/**
 * Unsubscribe to a Pubnub channel
 *
 * @param	*channel		pubnub channel to stop listening on
 * @return					<tt>false</tt> if channel not found
 */
bool IFA pubnub_unsubscribe(const char *channel)
{
	if(strcmp(channel, pb->subscribe_channel) == 0) {
		pb->subscribe_channel = NULL;
		pb->subscribeCB = NULL;
		subscribed = false;
		// should really issue a "leave"
		// DOMAIN/v2/presence/sub_key/demo/channel/my_channel/leave?uuid=MyUUID
		return true;
	}
	return false;
}


/**************************************************************************
	Local Utilities
***************************************************************************/

/**
 * Reset timetoken
 */
static void IFA resetTimetoken(void)
{
	pb->timetoken[0] = '0';
	pb->timetoken[1] = '\0';
}

/**
 * Find the beginning of a JSON string that comes
 *		after comma and ends at @c &buf[len].
 *
 * @return position (index) of the found start or -1 on error.
 */
static int IFA find_string_start(char const *buf, int len)
{
	int i;
	for (i = len-1; i > 0; i--) {
		if (buf[i] == '"') {
			return (buf[i-1] == ',') ? i : -1;
		}
	}
	return -1;
}


/**
 * Split @p buf string containing a JSON array (with arbitrary
 *		contents) to multiple NUL-terminated C strings, in-place.
 */
static bool IFA split_array(char *buf)
{
	bool escaped = false;
	bool in_string = false;
	int bracket_level = 0;

	for (; *buf != '\0'; ++buf) {
		if (escaped) {
			escaped = false;
		}
		else if ('"' == *buf) {
			in_string = !in_string;
		}
		else if (in_string) {
			escaped = ('\\' == *buf);
		}
		else {
			switch (*buf) {
				case '[': case '{': bracket_level++; break;
				case ']': case '}': bracket_level--; break;
					// if at root, split!
				case ',': if (bracket_level == 0) *buf = '\0'; break;
				default: break;
			}
		}
	}
	return !(escaped || in_string || (bracket_level > 0));
}

/**
 * Parse response from Subscribe call
 */
static int IFA parse_subscribe_response(void)
{
	char *reply = pb->http_reply;
	int replylen = strlen(reply);

	if (reply[replylen-1] != ']' && replylen > 2) {
		replylen -= 2;
	}
	if ((reply[replylen-1] != ']') || (reply[replylen-2] != '"')) {
		return -1;
	}

	// Extract the last argument.
	int i = find_string_start(reply, replylen-2);
	if (i < 0) {
		return -1;
	}

	reply[replylen - 2] = 0;

	// Now, the last argument may either be a timetoken or a channel list.
	if (reply[i-2] == '"') {
		int k;
		// It is a channel list, there is another string argument in front
		//		of us. Process the channel list ...
		pb->chan_ofs = i+1;
		pb->chan_end = replylen - 1;
		for (k = pb->chan_end - 1; k > pb->chan_ofs; --k) {
			if (reply[k] == ',') {
				reply[k] = 0;
			}
		}

		// ... and look for timetoken again.
		reply[i-2] = 0;
		i = find_string_start(reply, i-2);
		if (i < 0) {
			return -1;
		}
	}
	else {
		pb->chan_ofs = 0;
		pb->chan_end = 0;
	}

	// Now, i points at
	// [[1,2,3],"5678"]
	// [[1,2,3],"5678","a,b,c"]
	//          ^-- here

	//Extract message
	int j;
	for(j = 0; j < i; ++j) {
		if(reply[j] == '[') {
			if(reply[j+1] == '[') {
				// Found start of message
				j += 2;
				break;
			}
		}
	}
	memcpy(pb->http_msgIn, reply + j, (i - j - 2));
	pb->http_msgIn[(i-j-2)] = '\0';


	// Setup timetoken.
	if (replylen-2 - (i+1) >= sizeof pb->timetoken) {
		return -1;
	}
	strcpy(pb->timetoken, reply + i+1);
	reply[i-2] = 0; // terminate the [] message array (before the ]!)

	// Set up the message list - offset, length and NUL-characters splitting
	//		the messages.
	pb->msg_ofs = 2;
	pb->msg_end = i-2;

	return split_array(reply + pb->msg_ofs) ? 0 : -1;
}
