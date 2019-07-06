#include "../TcpServer.h"
#include "../InetAddress.h"
#include "../EventLoop.h"
#include "../TcpConnection.h"

#include <string>
#include <iostream>

using namespace net;

void OnMessage(TcpConnection::TcpConnPtr conn)
{
	TcpConnection::Data data = conn->recieve();
	std::string request(data.begin(), data.end());	
	std::cout << "message : " << request << std::endl;
}	

int main() 
{
	EventLoop loop;
	InetAddress addr("8888");
	TcpServer server(&loop, addr);
	server.setMessageCallBack(std::bind(OnMessage, std::placeholders::_1));	
	server.setThreadNum(5);
	server.run();
	loop.loop();
	return 0;	
}
