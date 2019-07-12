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
		typedef std::weak_ptr<TcpConnection> TcpConnWeakPtr;

		typedef std::function<void(TcpConnPtr &)> OnMessageCallBack;
		typedef std::function<void(TcpConnPtr &)> OnCloseCallBack;

		TcpConnection(EventLoop *loop, Socket connSock);		
		~TcpConnection();
		void runInLoop();

		Data recieve() { return inputBuffer_.getBufferData(); }		
		void send(Data data) { if(!closed_) outputBuffer_.appendBufferData(data); }

		void setMessageCallBack(OnMessageCallBack cb) { messageCallback_ = cb; }		
		void setCloseCallBack(OnCloseCallBack cb) { closeCallback_ = cb; }

	private:
		static void HandleRead(TcpConnWeakPtr &);
		static void HandleWrite(TcpConnWeakPtr &);
		static void HandleClose(TcpConnWeakPtr &);		

		bool closed_;

		EventLoop *loop_;
		Socket connSock_;	
		Channel::ChannelPtr connChannel_;
		
		Buffer inputBuffer_;
		Buffer outputBuffer_;
				
		OnMessageCallBack messageCallback_;
		OnCloseCallBack closeCallback_;
};

}

#endif
