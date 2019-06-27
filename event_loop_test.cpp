#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"

#include <thread>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>


void onConnection() 
{
	std::cout << "----new connection comes----" << std::endl;
}

int main() 
{
	EventLoop loop;

	//tcp listener
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(8888);	
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);
	
	std::cout << "server start..." << std::endl;

	Channel channel(&loop, listenfd);	
	channel.setEvents(EPOLLIN | EPOLLET);
	//channel.addObservationOnEvent(EPOLLIN || EPOLLET);
        channel.setCallBack(std::bind(onConnection));	
	loop.addChannel(&channel);
	
	loop.loop();
	return 0;
}
