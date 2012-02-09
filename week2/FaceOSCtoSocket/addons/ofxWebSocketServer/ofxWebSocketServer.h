#pragma once

#include "ofThread.h"
#include "ofEvents.h"
#include "WebSocketProtocolsInfo.h"

extern "C"
{
	#include <libwebsockets.h>
}

struct session_t {
	session_t()
	{
		skipNextBroadcast   = false;
		lastBroadcast       = NULL;
		lastBroadcastLength = 0;
	}
	bool skipNextBroadcast;
	char* lastBroadcast;
	unsigned int lastBroadcastLength;
};

class ofxWebSocketServer : public ofThread
{
	public:
		ofxWebSocketServer();
		~ofxWebSocketServer();
		libwebsocket_protocols myProtocols[PROTOCOL_COUNT];
		void setup(libwebsocket_protocols _protocols[PROTOCOL_COUNT]);
		void exit();

		void threadedFunction();
		bool allowClient(const char* const _name, const char* const _ip);

		std::map<std::string, bool> allowRules;

		void _onopen   (libwebsocket* const ws,
					  session_t* const session,
					  const char* const _message) const;
		void _onclose  (libwebsocket* const ws,
					  session_t* const session,
					  const char* const _message) const;
		void _onmessage(libwebsocket* const ws,
					  session_t* const session,
					  const char* const _message) const;

		void broadcast(int protocolID, const std::string& message,
					 const bool needsEncoding = true);
		void broadcast(int protocolID, const char* const message,
					 unsigned int len,
					 const bool needsEncoding = true);
		
		void send(libwebsocket* const ws,
				const std::string& message);
		void send(libwebsocket* const ws,
				const char* const message,
				unsigned int len,
				const bool needsEncoding = true);

		void close(libwebsocket* const ws) const;

		void execute();
	class WebSocketEventArgs
	{
		public:
			libwebsocket* ws;
			session_t* session;
			std::string message;
	};

	static ofEvent<WebSocketEventArgs> onopen;
	static ofEvent<WebSocketEventArgs> onmessage;
	static ofEvent<WebSocketEventArgs> onclose;
	static ofEvent<WebSocketEventArgs> onexecute;
	
	std::vector<unsigned char> buf;
	struct libwebsocket_context *context;
	
	protected:
		
		short port;
		std::string interface;
		bool binary;

		bool useSSL;
		std::string sslCertFilename;
		std::string sslKeyFilename;

		

		bool defaultAllowPolicy;

	private:
		void encode(const char* const message,
				  const unsigned int len,
				  const char** encoded,
				  unsigned int* const encoded_len) const;

		void decode(const char* const message,
				  const unsigned int len,
				  const char** decoded,
				  unsigned int* const decoded_len) const;

		const libwebsocket* skip_ws;
};

static ofxWebSocketServer _websocket_server;

