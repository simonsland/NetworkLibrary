#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "Epoll.h"
#include <memory>
#include <vector>

namespace net
{

class EventLoop 
{
	public:
		EventLoop();
		~EventLoop() { stop_ = true; }

		void addChannel(Channel *channel);
		void modifyChannel(Channel *channel);

		void loop();
	private:
		EventLoop(const EventLoop &) = delete;
		EventLoop& operator=(const EventLoop &) = delete;
		
		typedef std::vector<Channel *> ChannelList;	

		bool stop_;		
		std::unique_ptr<Epoll> poller_;
};

}
#endif
