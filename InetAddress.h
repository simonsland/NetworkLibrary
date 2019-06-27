#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>

#include "SocketUtil.h"

namespace net 
{

class InetAddress 
{
	public:
	//only ipv4 is supported
	InetAddress(const std::string &ip, const std::string &port);

	int getAddrSize() const { return sizeof(addr_); }
	const struct sockaddr* getInetSocketAddress() const { return sockaddr_cast(&addr_); }
	private:
		struct sockaddr_in addr_;
};

}

#endif
