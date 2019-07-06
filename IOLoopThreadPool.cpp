#include "IOLoopThreadPool.h"
#include <mutex>
#include <utility>
#include "EventLoop.h"

using namespace net;

IOLoopThreadPool::IOLoopThreadPool(EventLoop *loop, int thread_cnt) 
:baseLoop_(loop),
idx_(0),
threadNums_(thread_cnt),
stop_(true)
{
	for(int i = 0; i < thread_cnt; i++) 
	{
		std::shared_ptr<EventLoop> ioLoop = std::make_shared<EventLoop>();
		loops_.push_back(ioLoop);		
	}
}

IOLoopThreadPool::~IOLoopThreadPool()
{
	stop_ = true;

	for(std::thread &t : threads_)
	{
		t.join();
	}
}

void IOLoopThreadPool::setThreadNums(int thread_cnt)
{
	for(int i = threadNums_; i < thread_cnt; i++)
	{
		std::shared_ptr<EventLoop> ioLoop = std::make_shared<EventLoop>();
		loops_.push_back(ioLoop);
	}
	threadNums_ = thread_cnt;
}

void IOLoopThreadPool::start()
{
	stop_ = false;
	for(int i = 0; i < threadNums_; i++) 
	{
		threads_.emplace_back(
			[this,i]() {
				(this->loops_[i])->loop();			
			});
	}
}

void IOLoopThreadPool::stop()
{
	
}

EventLoop *IOLoopThreadPool::getNextLoop()
{
	if(threadNums_ == 0) return baseLoop_;
	EventLoop *ioLoop = loops_[idx_].get();
	idx_ = (idx_ + 1) % threadNums_;
	return ioLoop; 
}
