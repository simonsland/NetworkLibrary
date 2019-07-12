#include "Epoll.h"

#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace net;

void Epoll::addChannel(Channel::ChannelWPtr &weakChannel) {
	Channel::ChannelPtr channel(weakChannel.lock());	
	if(!channel) return;
	channelMap_[channel->getFd()] = weakChannel;	
	
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

void Epoll::modifyChannel(Channel::ChannelWPtr &weakChannel)
{
	Channel::ChannelPtr channel(weakChannel);
	if(!channel) return;

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

void Epoll::removeChannel(Channel::ChannelWPtr &weakChannel)
{	
	Channel::ChannelPtr channel(weakChannel);
	if(!channel) return;
	channelMap_.erase(channel->getFd());

	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));	

	int fd = channel->getFd();
	ev.events = channel->getEvents();
	ev.data.fd = fd; 	

	if(epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, &ev) < 0) 
	{		
		std::cout << "file :" << __FILE__ << " at line : "<< __LINE__<< " in function "<< __FUNCTION__ << "() error " << errno << " : " << strerror(errno) << std::endl; 
	}	
}

Channel::ChannelList Epoll::poll()
{
	int event_num = epoll_wait(epollfd_, &(*events_.begin()), events_.size(), -1);
	Channel::ChannelList activated;
	for(int i = 0; i < event_num; i++) 
	{
		int fd = events_[i].data.fd;
		if(channelMap_.find(fd) != channelMap_.end()) 
		{
			Channel::ChannelWPtr wptr = channelMap_[fd];
			Channel::ChannelPtr channel(wptr.lock());
			if(channel)
			{
				channel->setRevents(events_[i].events);
				activated.push_back(wptr);
			}
		}
	}
	return activated;
}

Epoll::~Epoll()
{
	close(epollfd_);	
}
