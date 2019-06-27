#include "IOLoopThreadPool.h"
#include "EventLoop.h"
#include "Channel.h"

using namespace net;

int main() 
{
	EventLoop loop;
	IOLoopThreadPool pool(&loop, 5);
	//Channel channel(&loop);
	pool.start();	
	loop.loop();
	return 0;
}
