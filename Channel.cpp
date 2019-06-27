#include "Channel.h"
#include "EventLoop.h"

using namespace net;

void Channel::handleEvent()
{
	if(revents_ & EPOLLIN)
	{
		readcallback_();
	} 
	else if(revents_ & EPOLLOUT)
	{
		
	}	
}

void Channel::update()
{
	loop_->updateChannel(this);		
}
