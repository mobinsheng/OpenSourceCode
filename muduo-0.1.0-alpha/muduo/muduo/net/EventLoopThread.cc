// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/net/EventLoopThread.h>

#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;


EventLoopThread::EventLoopThread()
  : loop_(NULL),
    exiting_(false),
	// 构造一个线程，调用的是threadFunc函数
    thread_(boost::bind(&EventLoopThread::threadFunc, this)),
    mutex_(),
    cond_(mutex_)
{
}

EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  // 循环退出
  loop_->quit();
  // 等待线程结束
  thread_.join();
}

// 启动循环
EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  
  // 启动线程
  thread_.start();

  // 在这里等待循环准备好
  {
    MutexLockGuard lock(mutex_);
	// 等待循环准备好
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop_;
}

// 线程函数
void EventLoopThread::threadFunc()
{
  EventLoop loop;

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
  //assert(exiting_);
}

