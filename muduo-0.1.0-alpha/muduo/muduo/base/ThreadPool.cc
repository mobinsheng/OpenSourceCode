// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/base/ThreadPool.h>

#include <muduo/base/Exception.h>

#include <boost/bind.hpp>
#include <assert.h>
#include <stdio.h>

using namespace muduo;

ThreadPool::ThreadPool(const string& name)
  : mutex_(),
    cond_(mutex_),
    name_(name),
    running_(false)
{
}

ThreadPool::~ThreadPool()
{
}

// 启动numThreads个线程
void ThreadPool::start(int numThreads)
{
  // 调用此函数时，线程集合必须是空的，否则出错，这防止了start函数被重复调用
  assert(threads_.empty());
  running_ = true;
  threads_.reserve(numThreads);
  
  // 循环创建线程
  for (int i = 0; i < numThreads; ++i)
  {
	// 自定义的线程id
    char id[32];
    snprintf(id, sizeof id, "%d", i);
	// 创建一个线程，并将此线程压入线程集合中
	// 每一个线程执行的实际函数就是ThreadPool::runInThread
	// 该函数是一个无限循环，会将任务从队列中取出，然后执行
    threads_.push_back(new muduo::Thread(
          boost::bind(&ThreadPool::runInThread, this), name_+id));
	// 启动此线程
    threads_[i].start();
  }
}

// 停止线程集合中的所有线程
void ThreadPool::stop()
{
  running_ = false;
  cond_.notifyAll();
  for_each(threads_.begin(),
           threads_.end(),
           boost::bind(&muduo::Thread::join, _1));
}

// 运行一个任务
void ThreadPool::run(const Task& task)
{
  // 如果线程池中没有线程，那么按照同步的方式运行这个任务
  if (threads_.empty())
  {
    task();
  }
  else
  {
	// 把这个任务添加到任务队列中
    MutexLockGuard lock(mutex_);
    queue_.push_back(task);
    cond_.notify();
  }
}

// 从队列中取出一个任务
ThreadPool::Task ThreadPool::take()
{
  MutexLockGuard lock(mutex_);
  while (queue_.empty() && running_)
  {
    cond_.wait();
  }
  Task task;
  if(!queue_.empty())
  {
    task = queue_.front();
    queue_.pop_front();
  }
  return task;
}

// 运行一个任务
void ThreadPool::runInThread()
{
  try
  {
    while (running_)
    {
	  // 取出一个任务，并执行
      Task task(take());
      if (task)
      {
        task();
      }
    }
  }
  catch (const Exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
    abort();
  }
  catch (const std::exception ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    abort();
  }
  catch (...)
  {
    fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
    abort();
  }
}

