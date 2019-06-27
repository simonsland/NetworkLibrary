#include "TcpServer.h"

#include <iostream>

void onConnection() 
{
	std::cout << "---connection comes---" << std::endl;
}

int main() 
{
	TcpServer server;
	server.setConnectionCallBack(std::bind(onConnection));
	server.listenAndServer(8888);
	return 0;
}
