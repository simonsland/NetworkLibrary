#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "Epoll.h"

#include <thread>
#include <memory>
#include <vector>

namespace net
{

class EventLoop 
{
	public:
		EventLoop();
		~EventLoop() { }

		void addChannel(Channel::ChannelWPtr);
		void modifyChannel(Channel::ChannelWPtr);
		void removeChannel(Channel::ChannelWPtr);

		void loop();
	private:
		EventLoop(const EventLoop &) = delete;
		EventLoop& operator=(const EventLoop &) = delete;

		bool stop_;			
		std::unique_ptr<Epoll> poller_;
		std::thread::id threadId_;
};

}
#endif
