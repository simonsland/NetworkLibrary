#include "../TcpServer.h"
#include "../InetAddress.h"
#include "../EventLoop.h"
#include "../TcpConnection.h"

#include <string>
#include <iostream>

using namespace net;

void OnMessage(TcpConnection::TcpConnPtr &conn)
{
	TcpConnection::Data data = conn->recieve();
	std::string request(data.begin(), data.end());	
	//std::cout << "message : " << request << std::endl;
	conn->send(data);
}	

int main() 
{
	EventLoop loop;
	InetAddress addr("8888");
	TcpServer server(&loop, addr);
	server.setMessageCallBack(std::bind(OnMessage, std::placeholders::_1));	
	server.setThreadNum(4);
	server.run();
	std::cout << "main loop at thread : " << std::this_thread::get_id() << std::endl;
	loop.loop();
	return 0;	
}
