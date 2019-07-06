#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"

#include <functional>
#include <vector>
#include <memory>

namespace net
{

class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection> 
{
	public:
		typedef Buffer::BufferData Data;
		typedef std::shared_ptr<TcpConnection> TcpConnPtr;

		typedef std::function<void(TcpConnPtr)> OnMessageCallBack;
		typedef std::function<void(TcpConnPtr)> OnCloseCallBack;

		TcpConnection(EventLoop *loop, Socket connSock);		
		~TcpConnection();

		Data recieve() { return inputBuffer_.getBufferData(); }		
		void send() { }

		void setMessageCallBack(OnMessageCallBack cb) { messageCallback_ = cb; }		
		void setCloseCallBack(OnCloseCallBack cb) { closeCallback_ = cb; }

	private:
		void HandleRead();

		EventLoop *loop_;
		Socket connSock_;	
		Channel connChannel_;
		
		Buffer inputBuffer_;
		Buffer outputBuffer_;
				
		OnMessageCallBack messageCallback_;
		OnCloseCallBack closeCallback_;
};

}

#endif
