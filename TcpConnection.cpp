#include "TcpConnection.h"
#include "EventLoop.h"

using namespace net;

TcpConnection::TcpConnection(EventLoop *loop, Socket connSock)
:closed_(false),
loop_(loop),
connSock_(std::move(connSock)),
connChannel_(std::make_shared<Channel>(loop, connSock_.fd()))
{

}

TcpConnection::~TcpConnection()
{
	connChannel_->disableAll();
	loop_->removeChannel(connChannel_);
	std::cout << "TcpConnection : " << connSock_.fd() << " destroy at thread" << std::endl;
}

void TcpConnection::runInLoop()
{
	connChannel_->enableRead();
	connChannel_->enableWrite();
	connChannel_->enableErr();
	TcpConnWeakPtr weak_this(shared_from_this());
	connChannel_->setReadCallBack(std::bind(&TcpConnection::HandleRead, weak_this));
	connChannel_->setWriteCallBack(std::bind(&TcpConnection::HandleWrite, weak_this));
	connChannel_->setCloseCallBack(std::bind(&TcpConnection::HandleClose, weak_this));
		
	loop_->addChannel(connChannel_);	
}

//这里还有一个待修复的问题在于
//如果客户端一直不读缓冲区，服务端无法收到EPOLLOUT事件，则会累积数据并且不关闭连接
void TcpConnection::HandleRead(TcpConnWeakPtr &weakConn) 
{
	TcpConnPtr conn(weakConn.lock());
	if(!conn) return;	
	int len = conn->connSock_.read(&conn->inputBuffer_);
	if(conn->inputBuffer_.getSize() > 0) 
	{
		if(conn->messageCallback_) conn->messageCallback_(conn);
	}
	
	if(len == 0) //客户端数据发送完成并关闭或半关闭连接 
	{
		//客户端关闭连接后，如果服务端outputBUffer中还有数据未发送
		//则持续发送数据直至数据发送完毕
		//同时还需要时刻客户端的状态，如果客户端此时异常中断，则直接关闭
		conn->closed_ = true; //预关闭
		if(conn->outputBuffer_.getSize() == 0) 
		{
			conn->closeCallback_(conn);
		}
	}
	else if(len == -1) //客户端关闭或发生错误
	{
		std::cout << "close completely.." << std::endl;
		conn->closed_ = true;
		conn->closeCallback_(conn);
	}
}

void TcpConnection::HandleWrite(TcpConnWeakPtr &weakConn)
{
	TcpConnPtr conn(weakConn.lock());
	if(!conn) return;
	int written_t = conn->connSock_.write(&conn->outputBuffer_);	
	if((conn->closed_ && written_t == 0) || written_t == -1) //缓冲区数据发送完毕或者写异常,关闭socket 
	{
		conn->closeCallback_(conn);	
	}
}

void TcpConnection::HandleClose(TcpConnWeakPtr &weakConn)
{
	TcpConnPtr conn(weakConn.lock());
	if(!conn) return;
	conn->closeCallback_(conn);
}


