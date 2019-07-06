#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

namespace net
{

class Buffer
{
	public:
		typedef std::vector<char> BufferData;

		Buffer();
		BufferData getBufferData();
		void remove(int);
		void append(char *, int);					
		int getSize() { return size_; }
		char *buffer() { return &buffer_[head_]; }
	private:
		int capacity_;
		BufferData buffer_;
		int size_;
		int head_;
		int tail_;
};

}

#endif
