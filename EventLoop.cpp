#include "EventLoop.h"
#include <iostream>

using namespace net;

EventLoop::EventLoop()
:stop_(true),
poller_(new Epoll(this))
{

}

void EventLoop::addChannel(Channel::ChannelWPtr channel)
{
	poller_->addChannel(channel);
}

void EventLoop::modifyChannel(Channel::ChannelWPtr channel)
{
	poller_->modifyChannel(channel);
}

void EventLoop::removeChannel(Channel::ChannelWPtr channel)
{
	poller_->removeChannel(channel);
}	

void EventLoop::loop()
{
	//debug
	stop_ = false;
	threadId_ = std::this_thread::get_id();
	std::cout << "loop start at thread : " << threadId_ <<std::endl;	
	while(!stop_) 
	{
		Channel::ChannelList activeated = poller_->poll();
		std::cout << "events comes at " << "thread : "<< threadId_ << std::endl;
		for(auto channel_w : activeated)
		{
			Channel::ChannelPtr channel(channel_w.lock());
			if(channel)
			{	
				channel->handleEvent();
			}
		}
	}		
}
