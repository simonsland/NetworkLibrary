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
	pool_.start();
	std::cout << "server run..." << std::endl;	
	//loop_->loop();
}

//设置IO线程个数，分离accpet和conntion处理，提高并发
void TcpServer::setThreadNum(int threadNum)
{
	if(threadNum < 0) threadNum = 0;
	pool_.setThreadNums(threadNum);	
}

//通过该connSock生成一个TcpConnection对象，加入TcpConnList中
//并将该connSock加入EventLoop中进行监听
//Tcperver管理TcpConnection的生命期
void TcpServer::HandleNewConn(Socket connSock)
{
	EventLoop *ioLoop = pool_.getNextLoop();
	
	std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(ioLoop, std::move(connSock));	
	conn->setMessageCallBack(OnMessageCallBack_);
	conn->setCloseCallBack(std::bind(&TcpServer::HandleClose, this, std::placeholders::_1));
	conns_.insert(conn);	
	std::cout << "online user count : " << conns_.size() << std::endl;
}

void TcpServer::HandleClose(std::shared_ptr<TcpConnection> conn)
{
	std::cout << "conn close, online user : " << conns_.size() << std::endl;
	conns_.erase(conn);
}


