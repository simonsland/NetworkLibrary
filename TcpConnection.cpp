#include "TcpConnection.h"
#include "EventLoop.h"

using namespace net;

TcpConnection::TcpConnection(EventLoop *loop, Socket connSock)
:loop_(loop),
connSock_(std::move(connSock)),
connChannel_(loop, connSock_.fd())
{
	connChannel_.enableRead();
	connChannel_.setReadCallBack(std::bind(&TcpConnection::HandleRead, this));
	loop->addChannel(&connChannel_);	
}

void TcpConnection::HandleRead() 
{
	connSock_.read(&inputBuffer_);
	if(inputBuffer_.getSize() > 0) 
	{
		messageCallback_(this);
	}
}
