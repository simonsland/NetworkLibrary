#include "InetAddress.h"

#include <arpa/inet.h>
#include <iostream>

using namespace net;

InetAddress::InetAddress(const std::string &ip, const std::string &port)
{
	struct in_addr ip_addr;
	if(inet_aton(ip.c_str(), &ip_addr) < 0)
	{
		std::cout << "invalid ip address. " << std::endl;
	       	exit(1);
	}
	addr_.sin_addr = ip_addr;
	addr_.sin_port = htons(static_cast<short>(atoi(port.c_str())));
	addr_.sin_family = AF_INET;	
}
