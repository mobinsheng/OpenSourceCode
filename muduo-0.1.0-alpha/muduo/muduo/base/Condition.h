// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>
#include <pthread.h>


namespace muduo
{
// 条件变量
// 也是内核对象
// 包装了对pthread_cond_t的访问，使之更加简单
class Condition : boost::noncopyable
{
 public:
 
  // 初始化条件变量
  Condition(MutexLock& mutex) : mutex_(mutex)
  {
    pthread_cond_init(&pcond_, NULL);
  }

  // 销毁条件变量
  ~Condition()
  {
    pthread_cond_destroy(&pcond_);
  }

  // 等待条件变量为真
  void wait()
  {
    pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
  }

  // 使用完毕条件变量之后，通知所有正在等待该条件变量的对象（单个）
  void notify()
  {
    pthread_cond_signal(&pcond_);
  }

  // 使用完毕条件变量之后，通知所有正在等待该条件变量的对象（广播）
  void notifyAll()
  {
    pthread_cond_broadcast(&pcond_);
  }

 private:
  // 条件变量必须加锁才能进行访问
  MutexLock& mutex_;
  pthread_cond_t pcond_;
};

}
#endif  // MUDUO_BASE_CONDITION_H
