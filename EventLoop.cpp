#include "EventLoop.h"
#include <iostream>

using namespace net;

EventLoop::EventLoop()
:stop_(false),
poller_(new Epoll(this))
{

}

void EventLoop::addChannel(Channel *channel)
{
	poller_->addChannel(channel);
}

void EventLoop::updateChannel(Channel *channel)
{
	poller_->updateChannel(channel);
}

void EventLoop::loop()
{
	//debug
	std::cout << "---start loop---" << std::endl;
	while(!stop_) 
	{
		ChannelList activeated = poller_->poll();
		for(auto channel : activeated)
		{
			channel->handleEvent();			
		}
	}		
}
