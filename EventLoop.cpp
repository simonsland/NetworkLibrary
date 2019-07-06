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

void EventLoop::modifyChannel(Channel *channel)
{
	poller_->modifyChannel(channel);
}

void EventLoop::loop()
{
	//debug
	std::cout << "start loop..." << std::endl;
	while(!stop_) 
	{
		std::cout << __FILE__ << " " << __LINE__ << " loop..." << std::endl;
		ChannelList activeated = poller_->poll();
		std::cout << "new events coming..." << std::endl;
		for(auto channel : activeated)
		{
			channel->handleEvent();

			//重新激活channel		
			//channel->resetRevents();				
			//this->modifyChannel(channel);
		}
	}		
}
