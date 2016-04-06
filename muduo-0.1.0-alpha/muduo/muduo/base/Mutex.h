// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

// 使用mutex实现的锁
// 提供对pthread_mutex_t的包装
class MutexLock : boost::noncopyable
{
 public:
  MutexLock()
  {
	// 初始化
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MutexLock()
  {
	// 销毁
    pthread_mutex_destroy(&mutex_);
  }

  void lock()
  {
	// 加锁
    pthread_mutex_lock(&mutex_);
  }

  void unlock()
  {
	// 解锁
    pthread_mutex_unlock(&mutex_);
  }

  // 获取原始的pthread_mutex_t对象的指针
  pthread_mutex_t* getPthreadMutex() /* non-const */
  {
    return &mutex_;
  }

 private:
  // 基础类型是pthread_mutex_t
  pthread_mutex_t mutex_;
};

// 锁的守卫，提供了这个类之后就不用手动的加锁和解锁了，利用了RAII原则，自动加锁和解锁
class MutexLockGuard : boost::noncopyable
{
 public:
  explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
  {
    mutex_.lock();
  }

  ~MutexLockGuard()
  {
    mutex_.unlock();
  }

 private:

  MutexLock& mutex_;
};

}

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif  // MUDUO_BASE_MUTEX_H
