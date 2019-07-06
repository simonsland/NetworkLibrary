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

TcpConnection::~TcpConnection()
{
	//优雅关闭
	while(outputBuffer_.getSize() > 0) connSock_.write(&outputBuffer_);	
}

void TcpConnection::HandleRead() 
{	
	int len = connSock_.read(&inputBuffer_);
	if(inputBuffer_.getSize() > 0) 
	{
		if(messageCallback_) messageCallback_(shared_from_this());
	}
	
	if(len == -1) //客户端关闭连接 
	{
		if(closeCallback_) closeCallback_(shared_from_this());
	}
}


