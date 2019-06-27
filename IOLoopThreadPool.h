#ifndef IO_LOOP_THREAD_POOL_H
#define IO_LOOP_THREAD_POOL_H	

#include <functional>
#include <vector>
#include <memory>
#include <thread>

namespace net 
{

class EventLoop;

class IOLoopThreadPool 
{
	public:
		IOLoopThreadPool(EventLoop *loop, int thread_cnt);

		void start();
		void stop();
		
		EventLoop *getNextLoop();

		void setThreadNums(int thread_cnt) { threadNums_ = thread_cnt; }

		~IOLoopThreadPool();
	private:
		EventLoop *baseLoop_;
		//io loop threads
		std::vector<std::thread> threads_;
		
		std::vector<std::shared_ptr<EventLoop>> loops_;	
		//当前IO loop的编号
		int idx_;

		int threadNums_;

		//pool status
		bool stop_;
};

}
#endif
