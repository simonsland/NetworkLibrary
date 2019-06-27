#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <iostream>
#include <sys/epoll.h>

namespace net 
{

class EventLoop;

class Channel
{
	public:
		typedef int EpollEvent;
		//typedef std::function<void()> OnConnCallBack;
		typedef std::function<void()> OnReadCallBack;
		typedef std::function<void()> OnWriteCallBack;
		typedef std::function<void()> OnCloseCallBack;
		
		Channel(EventLoop *loop, int fd) : loop_(loop), fd_(fd) {}
		~Channel() = default;

		void setReadCallBack(OnReadCallBack callback) { readcallback_ = std::move(callback); }
		
		void setFd(int fd) { fd_ = fd; };
		void setRevents(EpollEvent revents) { revents_ = revents; }
		int getFd() { return fd_; }
		EpollEvent getEvents() { return events_; }

		void enableRead() { events_ |= (EPOLLIN | EPOLLPRI | EPOLLET); update(); }
		void enableWrite() { events_ |= (EPOLLOUT | EPOLLET); update(); }
		void disableRead() { events_ &= (~EPOLLIN | EPOLLPRI); update(); }
		void disableWrite() { events_ &= ~EPOLLOUT; update(); }	
		void disableAll() { events_ &= ~events_; update(); }		
		
		void handleEvent();
	private:
		void update();

		EventLoop *loop_;
		int fd_;
		//监听的事件
		EpollEvent events_; 
		//返回的事件
		EpollEvent revents_;

		//OnConnCallBack conncallback_;
		OnReadCallBack readcallback_;
		OnWriteCallBack writecallback_;
		OnCloseCallBack closecallback_;
};

}
#endif

