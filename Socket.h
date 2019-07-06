#ifndef SOCKET_H
#define SOCKET_H

#include "base/Uncopyable.h"
#include <unistd.h>

namespace net
{

class InetAddress;
class Buffer;

//Socket类不可被拷贝，只提供移动语义
class Socket : private base::Uncopyable 
{
	public:
		explicit Socket(int sockfd) : sockfd_(sockfd) {} 
		
		//Socket是全局唯一的，因此在拷贝的过程中应该把资源管理权进行转移
		//并确保原Socket处于可析构状态
		//为了保证Socket资源的正确转移，可以有两种方式：
		//方式一：对Socket的管理统一使用unique_ptr进行，而不用普通指针。
		//方式二：定义Socket的移动拷贝和移动构造函数
		Socket(Socket &&) noexcept;
		Socket & operator=(Socket &&) noexcept;		

		~Socket() { if(sockfd_ >= 0) ::close(sockfd_); }		

		int fd() { return sockfd_; }

		void bindAddress(const InetAddress &addr);			
		void listen();
		int accept();

		int read(Buffer *);
		void write(Buffer *);

	private:
		int sockfd_;
};

}

#endif
