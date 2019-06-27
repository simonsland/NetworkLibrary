#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

namespace net
{

class Buffer
{
	public:
		Buffer();
		void remove(int);
		void append(char *, int);					
		int getSize() { return size_; }
		char *buffer() { return &buffer_[head_]; }
	private:
		int capacity_;
		std::vector<char> buffer_;
		int size_;
		int head_;
		int tail_;
};

}

#endif
