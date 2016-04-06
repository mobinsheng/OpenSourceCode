// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include <muduo/base/Atomic.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

// 线程类，对pthread_t的访问进行了包装
class Thread : boost::noncopyable
{
 public:
 
  // 函数对象类型
  typedef boost::function<void ()> ThreadFunc;

  // 构造一个线程，具体执行线程的函数由传递进来的函数对象指定
  Thread(const ThreadFunc&, const string& name = string());
  ~Thread();

  // 启动线程
  void start();
  // 等待线程结束
  void join();

  // 判断线程是否已经启动
  bool started() const { return started_; }
  // pthread_t pthreadId() const { return pthreadId_; }
  // 获取线程的id
  pid_t tid() const { return tid_; }
  
  // 获取线程的名字
  const string& name() const { return name_; }

  // 这个函数的意思是：获取创先的线程个数？？？
  static int numCreated() { return numCreated_.get(); }

 private:

  // 辅助函数，启动线程
  static void* startThread(void* thread);
  
  // 辅助函数，运行线程
  void runInThread();

  // 是否已经启动
  bool       started_;
  // 线程对象
  pthread_t  pthreadId_;
  // 线程id
  pid_t      tid_;
  // 执行具体线程任务的函数对象
  ThreadFunc func_;
  // 线程的名字
  string     name_;

  static AtomicInt32 numCreated_;
};

// 命名空间，当前的线程
namespace CurrentThread
{
  pid_t tid();
  const char* name();
  bool isMainThread();
}

}
#endif
