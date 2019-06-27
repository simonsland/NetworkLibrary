#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../InetAddress.h"

int main()
{
	net::EventLoop loop;
	net::InetAddress addr("127.0.0.1", "8888");
	net::TcpServer server(&loop, addr); 	
	server.run();
	loop.loop();
}
