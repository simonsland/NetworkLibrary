
#include "Epoll.h"

#include <iostream>
#include <unistd.h>

using namespace net;

void Epoll::update(int operation, Channel *channel)
{
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));	
	
	int fd = channel->getFd();
	ev.events = channel->getEvents();
	ev.data.fd = fd; 	

	if(epoll_ctl(epollfd_, operation, fd, &ev) < 0) 
	{
		std::cout << "epoll_ctl operation fd = , error" << fd << std::endl;
	}
}

Epoll::ChannelList Epoll::poll()
{
	int event_num = epoll_wait(epollfd_, &(*events_.begin()), events_.size(), -1);
	ChannelList activated;
	for(int i = 0; i < event_num; i++) 
	{
		Channel *channel = channels_[events_[i].data.fd];
		channel->setRevents(events_[i].events);
		activated.push_back(channel);

		//在下一个循环持续监听事件
		update(EPOLL_CTL_MOD, channel);	
	}
	return activated;
}

Epoll::~Epoll()
{
	close(epollfd_);	
}
