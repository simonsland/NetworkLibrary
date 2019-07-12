#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Socket.h"
#include "Channel.h"
#include "InetAddress.h"

#include <functional>
#include <memory>

namespace net
{

class EventLoop;
class InetAddress;

class Acceptor
{
	public:
		typedef std::function<void(Socket)> NewConnCallBack;

		Acceptor(EventLoop *loop, const InetAddress &listenAddr);
		
		void setNewConnCallBack(NewConnCallBack callback) { newConnCallback_ = callback; }

		void listen();	
	private:
		void HandleNewConn();

		EventLoop *loop_;
		Socket listenSock_;
		Channel::ChannelPtr listenChannel_;		

		bool listening;

		NewConnCallBack newConnCallback_;		
};

}

#endif
