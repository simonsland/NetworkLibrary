#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"

#include <functional>
#include <vector>

namespace net
{

class EventLoop;

class TcpConnection
{
	public:
		typedef std::function<void(TcpConnection *)> OnMessageCallBack;
		TcpConnection(EventLoop *loop, Socket connSock);		
		
		void setMessageCallBack(OnMessageCallBack cb) { messageCallback_ = cb; }		

	private:
		void HandleRead();

		EventLoop *loop_;
		Socket connSock_;	
		Channel connChannel_;
		
		Buffer inputBuffer_;
		Buffer outputBuffer_;
				
		OnMessageCallBack messageCallback_;
};

}

#endif
