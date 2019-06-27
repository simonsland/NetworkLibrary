#ifndef THREAD_POOL_H
#define THREAD_POOL_H	

#include <functional>
#include <future>
#include <type_traits>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <thread>

namespace base 
{

class ThreadPool 
{
	public:
		ThreadPool(int thread_cnt);
		
		template<typename F, typename... Args> 
		auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
		{
			using return_type = typename std::result_of<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

			std::future<return_type> res = task->get_future();

			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				if(stop)
					throw std::runtime_error("enqueue on stopped ThreadPool");
				tasks.push([task]{(*task)();});
			}

			condition.notify_one();

			return res;
		}

		~ThreadPool();
	private:
		//worker thread
		std::vector<std::thread> workers;
		//task queue
		std::queue<std::function<void()>> tasks;
		
		std::mutex queue_mutex;
		std::condition_variable condition;

		//pool status
		bool stop;
};

}
#endif
