// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREADPOOL_H
#define MUDUO_BASE_THREADPOOL_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Types.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <deque>

namespace muduo
{

// 线程池
class ThreadPool : boost::noncopyable
{
 public:
  // 任务就是一个函数对象
  typedef boost::function<void ()> Task;

  explicit ThreadPool(const string& name = string());
  ~ThreadPool();

  // 启动指定数量的线程
  void start(int numThreads);
  void stop();

  // 运行任务，该任务由线程池中的线程执行
  void run(const Task& f);

 private:
  void runInThread();
  
  // 从任务队列中获取一个任务
  Task take();

  // 互斥锁
  MutexLock mutex_;
  // 条件变量（上面的互斥锁用于保护此条件变量）
  Condition cond_;
  // 线程池的名字
  string name_;
  // 线程集合
  boost::ptr_vector<muduo::Thread> threads_;
  // 任务队列（任务按照先到先服务的原则进行处理）
  std::deque<Task> queue_;
  // 线程池是否在运行的标志
  bool running_;
};

}

#endif
