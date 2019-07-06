#ifndef EPOLL_H	
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <string.h>
#include <unordered_map>

#include "Channel.h"

namespace net
{

class EventLoop;

const int kInitEventListSize = 5;

class Epoll 
{
	public:	
		Epoll(EventLoop *loop)
		:epollfd_(epoll_create1(EPOLL_CLOEXEC)),
	         loop_(loop),
	  	 events_(kInitEventListSize)
		{}

		void addChannel(Channel *channel); 
		void modifyChannel(Channel *channel);
		void deleteChannel(Channel *channel);

		//close the epoll fd
		~Epoll();		
		
		std::vector<Channel *> poll();
	private:
		typedef std::unordered_map<int, Channel *> ChannelMap;
		typedef std::vector<struct epoll_event> EventList;
		typedef std::vector<Channel *> ChannelList;
		
		//close the fd when destroy
		int epollfd_;
		EventLoop *loop_;
		//fd-channel map
		ChannelMap channelMap_;

		EventList events_;
};

}
#endif
