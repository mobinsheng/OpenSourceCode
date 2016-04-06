// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_EVENTLOOPTHREAD_H
#define MUDUO_NET_EVENTLOOPTHREAD_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>

#include <boost/noncopyable.hpp>

namespace muduo
{

class Thread;

namespace net
{

class EventLoop;

// 事件循环的线程
class EventLoopThread : boost::noncopyable
{
 public:
  EventLoopThread();
  ~EventLoopThread();
  
  // 开始循环
  EventLoop* startLoop();

 private:
  // 线程函数
  void threadFunc();

  // 事件循环对象
  EventLoop* loop_;
  // 是否正在退出的标志
  bool exiting_;
  // 线程对象
  Thread thread_;
  // 互斥锁
  MutexLock mutex_;
  // 条件变量
  Condition cond_;
};

}
}

#endif  // MUDUO_NET_EVENTLOOPTHREAD_H

