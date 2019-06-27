#include "TcpServer.h"
#include "TcpConnection.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <functional>

#include <iostream>

using namespace net;

const int kDefaultThreadCnt = 0;

TcpServer::TcpServer(EventLoop *loop, const net::InetAddress &addr)
:loop_(loop),
addr_(addr),
acceptor_(loop, addr),
pool_(loop, kDefaultThreadCnt),
start_(false)
{
	acceptor_.setNewConnCallBack(std::bind(&TcpServer::HandleNewConn, this, std::placeholders::_1));
}


void TcpServer::run()
{
	start_ = true;
	std::cout << "server run" << std::endl;	
	//loop_->loop();
}

//通过该connSock生成一个TcpConnection对象，加入TcpConnList中
//并将该connSock加入EventLoop中进行监听
//Tcperver管理TcpConnection的生命期
void TcpServer::HandleNewConn(Socket connSock)
{
	EventLoop *ioLoop = pool_.getNextLoop();
	std::unique_ptr<TcpConnection> conn(new TcpConnection(ioLoop, std::move(connSock))); 	
	conn->setMessageCallBack(OnMessageCallBack_);
	conns_.emplace_back(std::move(conn));	
}


