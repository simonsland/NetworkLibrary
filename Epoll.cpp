
#include "Epoll.h"

#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace net;

void Epoll::addChannel(Channel *channel) { 
	channelMap_[channel->getFd()] = channel;	
	
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));	

	int fd = channel->getFd();
	ev.events = channel->getEvents();
	ev.data.fd = fd; 	

	if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) < 0) 
	{		
		std::cout << "file :" << __FILE__ << " at line : "<< __LINE__<< " in function "<< __FUNCTION__ << "() error " << errno << " : " << strerror(errno) << std::endl; 
	}	
}

void Epoll::modifyChannel(Channel *channel)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));	

	int fd = channel->getFd();
	ev.events = channel->getEvents();
	ev.data.fd = fd; 	

	if(epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev) < 0) 
	{		
		std::cout << "file :" << __FILE__ << " at line : "<< __LINE__<< " in function "<< __FUNCTION__ << "() error " << errno << " : " << strerror(errno) << std::endl; 
	}	
}

void Epoll::deleteChannel(Channel *channel)
{

}

Epoll::ChannelList Epoll::poll()
{
	int event_num = epoll_wait(epollfd_, &(*events_.begin()), events_.size(), -1);
	ChannelList activated;
	for(int i = 0; i < event_num; i++) 
	{
		Channel *channel = channelMap_[events_[i].data.fd];
		channel->setRevents(events_[i].events);
		activated.push_back(channel);
		

	}
	return activated;
}

Epoll::~Epoll()
{
	close(epollfd_);	
}
