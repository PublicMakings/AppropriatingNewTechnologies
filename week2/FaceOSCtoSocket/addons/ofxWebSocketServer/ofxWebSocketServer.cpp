#include "ofxWebSocketServer.h"


#include "ofUtils.h"

#include "encode.h"
#include "decode.h"

#include <sys/time.h>



ofEvent<ofxWebSocketServer::WebSocketEventArgs> ofxWebSocketServer::onopen;
ofEvent<ofxWebSocketServer::WebSocketEventArgs> ofxWebSocketServer::onmessage;
ofEvent<ofxWebSocketServer::WebSocketEventArgs> ofxWebSocketServer::onclose;
ofEvent<ofxWebSocketServer::WebSocketEventArgs> ofxWebSocketServer::onexecute;

//--------------------------------------------------------------
ofxWebSocketServer::ofxWebSocketServer()
:	port(7681)
, useSSL(false)
, context(NULL)
, buf(LWS_SEND_BUFFER_PRE_PADDING+1024+LWS_SEND_BUFFER_POST_PADDING)
, defaultAllowPolicy(true)
, binary(true)
{}

//--------------------------------------------------------------
ofxWebSocketServer::~ofxWebSocketServer()
{
  exit();
}

//--------------------------------------------------------------
void
ofxWebSocketServer::setup(libwebsocket_protocols _protocols[PROTOCOL_COUNT])
{
	for (int i=0; i<PROTOCOL_COUNT; i++)
	{
		myProtocols[i] = _protocols[i];
	}
	int opts = 0;
	cout << "PROTOCOL_DUMB_INCREMENT: " << myProtocols[PROTOCOL_DUMB_INCREMENT].name <<endl;
	sslCertFilename = ofToDataPath("libwebsockets-test-server.pem", true);
	sslKeyFilename = ofToDataPath("libwebsockets-test-server.key.pem", true);

	const char* _sslCertFilename  = NULL;
	const char* _sslKeyFilename   = NULL;
	if (useSSL)
	{
		_sslCertFilename = sslCertFilename.c_str();
		_sslKeyFilename = sslKeyFilename.c_str();
	}

	context = libwebsocket_create_context(port, interface.c_str(), myProtocols,
										libwebsocket_internal_extensions,
										_sslCertFilename, _sslKeyFilename,
										-1, -1, opts);
	if (context == NULL)
	{
		std::cerr << "libwebsocket init failed" << std::endl;
	}else 
	{
		startThread(true, false); // blocking, non-verbose
	}
	
}

//--------------------------------------------------------------
void
ofxWebSocketServer::exit()
{
  if (context != NULL)
  {
    libwebsocket_context_destroy(context);
    context = NULL;
  }
}

//--------------------------------------------------------------
void
ofxWebSocketServer::threadedFunction()
{
  while (isThreadRunning())
  {
    execute();
  }
}

//--------------------------------------------------------------
bool
ofxWebSocketServer::allowClient(const char* const _name, const char* const _ip)
{
  std::string name(_name);
  std::string ip(_ip);
  
  std::cout
  << "Received network connect from "
  << name << "(" << ip << ")"
  << std::endl;
  
  std::map<std::string, bool>::iterator allow_iter;
  
  allow_iter = allowRules.find(name);
  if (allow_iter != allowRules.end())
    return allow_iter->second;
  
  allow_iter = allowRules.find(ip);
  if (allow_iter != allowRules.end())
    return allow_iter->second;
  
  return defaultAllowPolicy;
}

//--------------------------------------------------------------
void
ofxWebSocketServer::_onopen(libwebsocket* const ws,
                           session_t* const session,
                           const char* const _message) const
{
  std::string message = (_message == NULL)? "" : _message;

  WebSocketEventArgs args;
  args.ws = ws;
  args.session = session;
  args.message = message;

  ofNotifyEvent(onopen, args);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::_onclose(libwebsocket* const ws,
                             session_t* const session,
                             const char* const _message) const
{
  std::string message = (_message == NULL)? "" : _message;
  
  WebSocketEventArgs args;
  args.ws = ws;
  args.session = session;
  args.message = message;
  
  ofNotifyEvent(onclose, args);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::_onmessage(libwebsocket* const ws,
                              session_t* const session,
                              const char* const _message) const
{
  std::string message = (_message == NULL)? "" : _message;

  if (binary)
  {
    unsigned int decoded_len;
    const char* decoded;
    decode(message.c_str(), message.size(), &decoded, &decoded_len);
    message = std::string(decoded, decoded_len);
  }

  WebSocketEventArgs args;
  args.ws = ws;
  args.session = session;
  args.message = message;
  
  ofNotifyEvent(onmessage, args);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::close(libwebsocket* const ws) const
{
  libwebsocket_close_and_free_session(context, ws, LWS_CLOSE_STATUS_NORMAL);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::broadcast(int protocolID, const std::string& message,
                              const bool needsEncoding)
{
  broadcast(protocolID, message.c_str(), message.size(), needsEncoding);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::broadcast(int protocolID, const char* const message,
                              unsigned int len,
                              const bool needsEncoding)
{
	unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];

	if (binary && needsEncoding)
	{
		unsigned int encoded_len;
		const char* encoded;
		encode(message, len, &encoded, &encoded_len);
		memcpy(p, encoded, encoded_len);
		len = encoded_len;
	}
	else {
		memcpy(p, message, len);
	}

	int n = libwebsockets_broadcast(&myProtocols[protocolID], p, len);
	if (n < 0)
	{
		fprintf(stderr, "ERROR writing to socket");
		return;
	}

	libwebsocket_service(context, 50);
 
}

//--------------------------------------------------------------
void
ofxWebSocketServer::send(libwebsocket* const ws, const std::string& message)
{
  send(ws, message.c_str(), message.size());
}

//--------------------------------------------------------------
void
ofxWebSocketServer::send(libwebsocket* const ws,
                         const char* const message,
                         unsigned int len,
                         const bool needsEncoding)
{
  unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];

  if (binary && needsEncoding)
  {
    unsigned int encoded_len;
    const char* encoded;
    encode(message, len, &encoded, &encoded_len);
    memcpy(p, encoded, encoded_len);
    len = encoded_len;
  }
  else {
    memcpy(p, message, len);
  }

  int n = libwebsocket_write(ws, p, len, LWS_WRITE_TEXT);
  if (n < 0)
  {
	std::cout << "ERROR writing to socket" << std::endl;  
  }
    
}

//--------------------------------------------------------------
void
ofxWebSocketServer::execute()
{
	
  WebSocketEventArgs args;
  ofNotifyEvent(onexecute, args);
}

//--------------------------------------------------------------
void
ofxWebSocketServer::encode(const char* const message,
                           const unsigned int len,
                           const char** encoded,
                           unsigned int* const encoded_len) const
{
  // TODO: this is super hacky, and base 64 encoding/decoding
  // is a waste of CPU, memory, and network traffic for both
  // client and server.
  // Eventually web sockets will support size-preamble frames
  // and these should be used instead.
  //    std::stringstream data_stream;
  std::stringstream ss, encoded_stream;
  base64::encoder enc;
  ss.write(message, len);
  enc.encode(ss, encoded_stream);
  
  const std::string encoded_str = encoded_stream.str();
  *encoded_len = encoded_str.size();
  *encoded = encoded_str.c_str();  
}

//--------------------------------------------------------------
void
ofxWebSocketServer::decode(const char* const message,
                           const unsigned int len,
                           const char** decoded,
                           unsigned int* decoded_len) const
{
  // TODO: this is super hacky, and base 64 encoding/decoding
  // is a waste of CPU, memory, and network traffic for both
  // client and server.
  // Eventually web sockets will support size-preamble frames
  // and these should be used instead.
  std::stringstream ss(message), decoded_stream;
  base64::decoder dec;
  dec.decode(ss, decoded_stream);
  
  const std::string decoded_str = decoded_stream.str();
  *decoded_len = decoded_str.size();
  *decoded = decoded_str.c_str();
}


