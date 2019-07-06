#include "SocketUtil.h"

#include <unistd.h>
#include <iostream>

using namespace net;

int creatNonBlockingSock()
{
	int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if(sockfd < 0) 
	{
		std::cout << "net::creatNonBlockingSock error" << std::endl;
		exit(1);	
	}
	return sockfd;
}

struct sockaddr* sockaddr_cast(const struct sockaddr_in *addr)
{
	return const_cast<struct sockaddr*>(reinterpret_cast<const struct sockaddr*>(addr));
}


