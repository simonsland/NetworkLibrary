#include "Buffer.h"

using namespace net;

const int kInitBufferSize = 4096;

Buffer::Buffer()
:capacity_(kInitBufferSize),
buffer_(kInitBufferSize),
size_(0),
head_(0),
tail_(0)
{

}

//每次读走一部分数据后，将数据整体前移
void Buffer::remove(int n)
{
	if(n > size_) n = size_;
	head_ += n;
	size_ -= n;
	for(int i = 0, j = head_; i < size_; i++, j++) 
	{
		buffer_[i] = buffer_[j];
	}
	head_ = 0;
	tail_ = head_ + size_;	
}

//如果buffer剩余空间满足写入需求，则直接覆盖对应位置的值
//否则通过resize将buffer大小变为两倍大小
void Buffer::append(char *data, int n) 
{
	int oldCapacity = capacity_;
	while(capacity_ - size_ < n)
	{
		capacity_ = capacity_ * 2;
	}	
	if(capacity_ != oldCapacity) buffer_.resize(capacity_);
	for(int i = tail_, j = 0; i < tail_ + n; i++, j++)
	{
		buffer_[i] = data[j];
	}
	size_ += n;
	tail_ += n;
}
