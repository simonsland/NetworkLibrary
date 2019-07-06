#include "Socket.h"
#include "InetAddress.h"
#include "Buffer.h"
#include "SocketUtil.h"

#include <sys/socket.h>
#include <iostream>
#include <errno.h>
#include <string.h>
using namespace net;

const int kStackBufferSize = 65536;

Socket::Socket(Socket &&sock) noexcept
: sockfd_(sock.sockfd_)
{
	sock.sockfd_ = -1;
}


Socket & Socket::operator=(Socket &&sock) noexcept
{
	if(sockfd_ == sock.sockfd_) return *this;
	::close(sockfd_);
	sock.sockfd_ = sock.sockfd_;
	sock.sockfd_ = -1;
	return *this;
}

void Socket::bindAddress(const InetAddress &addr)
{ 
	if(::bind(sockfd_, addr.getInetSocketAddress(), addr.getAddrSize()) < 0)
	{
		std::cout << "net::socket bind error" << std::endl;
	}	
}

void Socket::listen() 
{
	if(::listen(sockfd_, 5) < 0)
	{
		std::cout << "net::socket listen error" << std::endl;	
	}	
}

int Socket::accept()
{
	std::cout << __FILE__ << " at line: " << __LINE__ << " in functio: " << __FUNCTION__ << std::endl;
	struct sockaddr_in clientAddr;
	socklen_t clilen;
	int connfd = ::accept(sockfd_, reinterpret_cast<struct sockaddr*>(&clientAddr), &clilen);
	if(connfd < 0)
	{
		std::cout << "accpet error : " << strerror(errno) << std::endl;
		//accept返回-1的情况主要包括慢系统调用被中断,此时errno等于EINTR
		//客户端在accept之前关闭，发送了一个RST至服务端，此时errno等于ECONNABORTED，软件引起的连接终止
		//可以根据errno来确定是否需要重启accept及相应的处理流程		
	}
	return connfd;		
}

//ET模式下的非阻塞读
int Socket::read(Buffer *inputBuffer)
{
	char buffer[kStackBufferSize];
	int read_t = 0;
	while(true)
	{
		int len = recv(sockfd_, buffer, kStackBufferSize, 0); 
		if(len < 0) 
		{
			std::cout << "read " << read_t << " bytes from socket" << std::endl;

			if(errno == EAGAIN) //缓存区数据读取完成
			{
				break;				
			} 
			else if(errno == EINTR) //慢系统调用被中断 
			{
				continue;
			}
			else break;  //其他错误如ECONNRST网络连接出错 	
		}
		else if(len == 0) //对端数据发送完毕并关闭连接
		{
			return -1; 
		}
		//将读取到的数据放入inputBuffer,并循环读取缓冲区(ET模式)
		inputBuffer->append(buffer,len);
		read_t += len;
		std::cout << "read " << read_t << " bytes from socket" << std::endl;
	}
	return read_t;
}

//ET模式下的非阻塞写
void Socket::write(Buffer *outputBuffer)
{
	int remain = outputBuffer->getSize();
	char *buffer = outputBuffer->buffer();	
	while(remain > 0) 
	{
		int len = send(sockfd_, buffer, remain, 0);
		if(len < 0) 
		{
			if(errno == EAGAIN) //写缓存冲区满 
			{
				break;	
			} 
			else if(errno == EINTR) 
			{
				continue;
			}
			else break; //其他错误如EPIPE，对方已经关闭连接
		}
		buffer += len;
		remain -= len;
	}
	outputBuffer->remove(outputBuffer->getSize() - remain);
}

