#include "Channel.h"
#include "EventLoop.h"

#include <iostream>

using namespace net;

void Channel::handleEvent()
{
	if(revents_ & EPOLLIN)
	{
		std::cout << __FILE__ << " " << __LINE__ << " on read call back..." << std::endl;
		readcallback_();
	} 
	else if(revents_ & EPOLLOUT)
	{
		
	}	
}

//void Channel::update() 
//{ 
//	loop_->updateChannel(this); 
//}

