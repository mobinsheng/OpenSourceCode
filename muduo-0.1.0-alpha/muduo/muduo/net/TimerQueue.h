// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <list>

#include <boost/noncopyable.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/Channel.h>

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
// 计时器队列
class TimerQueue : boost::noncopyable
{
 public:
  TimerQueue(EventLoop* loop);
  ~TimerQueue();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  // 调度
  TimerId schedule(const TimerCallback& cb,
                   Timestamp when,
                   double interval);

  // void cancel(TimerId timerId);

 private:
  // called when timerfd arms
  // 处理读
  void handleRead();
  // insert timer in sorted list.
  bool insertWithLockHold(Timer* timer);

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  // 计时器链表
  typedef std::list<Timer*> TimerList;

  EventLoop* loop_;
  const int timerfd_;
  // 计时器通道
  Channel timerfdChannel_;
  // 互斥锁
  MutexLock mutex_;
  // Timer list sorted by expiration, @GuardedBy mutex_
  TimerList timers_;
};

}
}
#endif  // MUDUO_NET_TIMERQUEUE_H
