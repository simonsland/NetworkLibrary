#include "Channel.h"
#include "EventLoop.h"

#include <iostream>
#include <thread>
using namespace net;


void Channel::handleEvent()
{
	if(revents_ & EPOLLIN)
	{
		std::cout << __FILE__ << " " << __LINE__ << " on read call back..." << std::endl;
		readcallback_();
	} 

	//BUG: Channel的生命期由TcpConnection管理，如果在readcallback_中释放了channel，那么writecallback_()回调就会段错误	
	//修改：Channel的生命期通过智能指针由TcpConnection管理,其他地方通过weakPtr进行管理
	//weakPtr通过提升保证在调用Channel方法的时候Channel生命期有效
	if(revents_ & EPOLLOUT)
	{
		//if(writecallback_) std::cout << "write call back exist at thread : " << std::this_thread::get_id() << std::endl;
		std::cout << __FILE__ << " " << __LINE__ << " on write call back at thread : " << std::this_thread::get_id() << std::endl;
		writecallback_();				
	}
	 
	if(revents_ & EPOLLERR)
	{
		std::cout << __FILE__ << " " << __LINE__ << " on exceptional close..." << std::endl;
		closecallback_();		
	} 	
}

//void Channel::update() 
//{ 
//	loop_->updateChannel(this); 
//}

