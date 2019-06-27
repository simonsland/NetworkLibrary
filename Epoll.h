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

		void addChannel(Channel *channel) { 
			channels_[channel->getFd()] = channel;	
			update(EPOLL_CTL_ADD, channel);
		}

		void updateChannel(Channel *channel) { update(EPOLL_CTL_MOD, channel); }
		
		//close the epoll fd
		~Epoll();		
		
		std::vector<Channel *> poll();
	private:
		typedef std::unordered_map<int, Channel *> ChannelMap;
		typedef std::vector<struct epoll_event> EventList;
		typedef std::vector<Channel *> ChannelList;

		void update(int operation, Channel *channel);
		
		//close the fd when destroy
		int epollfd_;
		EventLoop *loop_;
		//fd-channel map
		ChannelMap channels_;

		EventList events_;
};

}
#endif
