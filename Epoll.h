#ifndef EPOLL_H	
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <memory>

#include "Channel.h"

namespace net
{

class EventLoop;

const int kInitEventListSize = 1000;

class Epoll 
{
	public:		
	       	Epoll(EventLoop *loop)
		:epollfd_(epoll_create1(EPOLL_CLOEXEC)),
	         loop_(loop),
	  	 events_(kInitEventListSize)
		{}

		void addChannel(Channel::ChannelWPtr &); 
		void modifyChannel(Channel::ChannelWPtr &);
		void removeChannel(Channel::ChannelWPtr &);

		//close the epoll fd
		~Epoll();		
		
		Channel::ChannelList poll();
	private:
		typedef std::unordered_map<int, Channel::ChannelWPtr> ChannelMap;
		typedef std::vector<struct epoll_event> EventList;
		
		//close the fd when destroy
		int epollfd_;
		EventLoop *loop_;
		//fd-channel map
		ChannelMap channelMap_;

		EventList events_;
};

}
#endif
