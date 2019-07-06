#include <functional>
#include <memory>
#include <set>

#include "EventLoop.h"
#include "InetAddress.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "IOLoopThreadPool.h"

namespace net
{

class EventLoop;
class Socket;

class TcpServer 
{
	public:
		typedef std::function<void()> OnConnCallBack;
		typedef std::function<void(TcpConnection::TcpConnPtr)> OnMessageCallBack;	

		TcpServer(EventLoop *loop, const net::InetAddress &addr);
		
		void run();		
		void setThreadNum(int threadNum);

		void setConnectionCallBack(OnConnCallBack callback) { OnConnectionCallBack_ = callback; }
		void setMessageCallBack(OnMessageCallBack callback) { OnMessageCallBack_ = callback; }
		//void setCloseCallBack(CallBack callback) { OnCloseCallBack_ = callback; }
			
	private:
		void HandleNewConn(Socket connSock);
		void HandleMessage();
		void HandleClose(std::shared_ptr<TcpConnection> conn);

		typedef std::set<std::shared_ptr<TcpConnection>> TcpConnList;
		EventLoop *loop_;
		InetAddress addr_;	
		Acceptor acceptor_;

		IOLoopThreadPool pool_;			

		bool start_;

		OnConnCallBack OnConnectionCallBack_;
		OnMessageCallBack OnMessageCallBack_;

		TcpConnList conns_;	
};
 
}
