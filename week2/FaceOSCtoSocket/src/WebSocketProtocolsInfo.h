#ifndef _WEBSOCKET_PROTOCOLS_INFO_
#define _WEBSOCKET_PROTOCOLS_INFO_

enum demo_protocols 
{
	//always first 
	PROTOCOL_HTTP = 0,
	
	PROTOCOL_DUMB_INCREMENT,
	PROTOCOL_LWS_MIRROR,

	//always last
	PROTOCOL_COUNT
};

struct per_session_data__dumb_increment
{
	int number;
};


struct per_session_data__lws_mirror
{
	struct libwebsocket *wsi;
	int ringbuffer_tail;
};

struct a_message
{
	void *payload;
	size_t len;
};

#define MAX_MESSAGE_QUEUE 64
static struct a_message ringbuffer[MAX_MESSAGE_QUEUE];
static int ringbuffer_head;

#endif