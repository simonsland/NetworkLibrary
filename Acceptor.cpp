#include "Acceptor.h"
#include "EventLoop.h"
#include "SocketUtil.h"

#include <fcntl.h>
#include <memory>

using namespace net; 

Acceptor::Acceptor(EventLoop *loop, const InetAddress &addr)
:loop_(loop),
listenSock_(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0)),
listenChannel_(loop, listenSock_.fd()),
listening(false)
{
	listenSock_.bindAddress(addr);	
	listenSock_.listen();
	listenChannel_.enableRead();
	listenChannel_.setReadCallBack(std::bind(&Acceptor::HandleNewConn, this));
	loop->addChannel(&listenChannel_);
}

//void Acceptor::listen()
//{	
//	listening = true;
//	listenSock_.listen();	
//}

//调用listenSock的accpet方法得到一个connfd, 并立刻放入ConnSocket中进行资源管理
//调用TcpServer的HandleNewConn方法,并将该ConnSocket作为参数传入
void Acceptor::HandleNewConn()
{
	int connfd = listenSock_.accept();
	if(connfd > 0)
	{
		std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;
		//将connfd设置为非阻塞状态
		if(fcntl(connfd, F_SETFL, O_NONBLOCK | fcntl(connfd, F_GETFL, 0)) < 0)
		{
			std::cout << __FILE__ << " " << __LINE__ << " " << " fcntl error" << std::endl;
		} 
		else 
		{
			//std::cout << __FILE__ << " " << __LINE__ << " " << " new connection callback... " << std::endl;
			newConnCallback_(Socket(connfd));		
		}
	}	
}

