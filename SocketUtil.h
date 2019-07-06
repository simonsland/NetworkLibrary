#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <sys/socket.h>

namespace net
{
	extern int creatNonBlockingSock();
	extern struct sockaddr* sockaddr_cast(const struct sockaddr_in *addr);
}

#endif
