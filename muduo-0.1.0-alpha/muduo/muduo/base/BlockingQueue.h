// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <deque>
#include <assert.h>

namespace muduo
{

// 阻塞队列
template<typename T>
class BlockingQueue : boost::noncopyable
{
 public:
  BlockingQueue()
    : mutex_(),
      cond_(mutex_),
      queue_()
  {
  }

  // 把一个元素压入队列
  void put(const T& x)
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(x);
	// 通知其他人，队列中有数据
    cond_.notify();
  }

  // 取走一个元素
  T take()
  {
    MutexLockGuard lock(mutex_);
	// 队列为空，则必须等待
    while (queue_.empty())
    {
      cond_.wait();
    }
    assert(!queue_.empty());
    T front(queue_.front());
    queue_.pop_front();
    return front;
  }

  size_t size() const
  {
    MutexLockGuard lock(mutex_);
    return queue_.size();
  }

 private:
  mutable MutexLock mutex_;
  Condition         cond_;
  std::deque<T>     queue_;
};

}

#endif  // MUDUO_BASE_BLOCKINGQUEUE_H
