
#include "ThreadPool.h"
#include <mutex>
#include <utility>

using namespace base;

ThreadPool::ThreadPool(int thread_cnt) 
: stop(false)
{
	for(int i = 0; i < thread_cnt; i++) 
	{
		workers.emplace_back(
			[this]
			{	for(;;)
				{
					std::function<void()> task;
				
					{	
						std::unique_lock<std::mutex> lk(queue_mutex);
					
						this->condition.wait(lk, [this]{return this->stop || !this->tasks.empty();});
		
						if(this->stop && this->tasks.empty()) 
							return;
					
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}

					task();
				}
			}
		);
	}
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}

	this->condition.notify_all();

	for(std::thread &worker : workers)
	{
		worker.join();
	}
}



