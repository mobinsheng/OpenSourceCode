// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <muduo/base/Mutex.h>
#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TimerId.h>

namespace muduo
{
namespace net
{

class Channel;
class Poller;
class TimerQueue;

///
/// Reactor, at most one per thread.
///
/// This is an interface class, so don't expose too much details.
// 事件循环类
class EventLoop : boost::noncopyable
{
 public:
  typedef boost::function<void()> Functor;

  EventLoop();
  ~EventLoop();  // force out-line dtor, for scoped_ptr members.

  ///
  /// Loops forever.
  ///
  /// Must be called in the same thread as creation of the object.
  ///
  void loop();

  void quit();

  ///
  /// Time when poll returns, usually means data arrivial.
  ///
  // 返回轮询的时间
  Timestamp pollReturnTime() const { return pollReturnTime_; }

  // timers

  /// Runs callback immediately in the loop thread.
  /// It wakes up the loop, and run the cb.
  /// If in the same loop thread, cb is run within the function.
  /// Safe to call from other threads.
  // 在循环中执行该函数对象
  void runInLoop(const Functor& cb);
  /// Queues callback in the loop thread.
  /// Runs after finish pooling.
  /// Safe to call from other threads.
  // 把函数对象加入到队列中
  void queueInLoop(const Functor& cb);
  ///
  // 在指定的时间运行某个回调函数
  TimerId runAt(const Timestamp& time, const TimerCallback& cb);
  ///
  /// Runs callback after @c delay seconds.
  /// Safe to call from other threads.
  // 经过指定延时之后运行某个回调函数
  TimerId runAfter(double delay, const TimerCallback& cb);
  ///
  /// Runs callback every @c interval seconds.
  /// Safe to call from other threads.
  // 每隔interval的时间就运行一次回调函数
  TimerId runEvery(double interval, const TimerCallback& cb);
  /// Cancels the timer.
  /// Safe to call from other threads.
  // void cancel(TimerId timerId);

  // internal usage
  // 唤醒
  void wakeup();
  // 更新信道
  void updateChannel(Channel* channel);
  // 移除信道
  void removeChannel(Channel* channel);

  pid_t threadId() const { return threadId_; }
  void assertInLoopThread()
  {
    if (!isInLoopThread())
    {
      abortNotInLoopThread();
    }
  }
  // 判断是否在循环线程中
  bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
  // bool callingPendingFunctors() const { return callingPendingFunctors_; }

 private:
  void abortNotInLoopThread();
  
  // 处理读
  void handleRead();  // waked up
  
  // 处理投递的函数对象
  void doPendingFunctors();

  // 打印活动的信道
  void printActiveChannels() const; // DEBUG

  // 信道列表
  typedef std::vector<Channel*> ChannelList;

  // 接下来这几个都是原子变量-begin
  // 循环标志
  bool looping_; /* atomic */
  // 退出标志
  bool quit_; /* atomic */
  // 事件正被处理标志
  bool eventHandling_; /* atomic */
  // 正在调用投递函数对象的标志
  bool callingPendingFunctors_; /* atomic */
  // 原子变量-end
  
  
  // 轮询返回的时间戳
  Timestamp pollReturnTime_;
  // 执行这个循环的线程的id
  const pid_t threadId_;
  // 轮询者
  boost::scoped_ptr<Poller> poller_;
  // 计时器队列
  boost::scoped_ptr<TimerQueue> timerQueue_;
  // 唤醒描述符
  int wakeupFd_;
  // unlike in TimerQueue, which is an internal class,
  // we don't expose Channel to client.
  // 已经激活的信道列表
  boost::scoped_ptr<Channel> wakeupChannel_;
  
  // 活跃的信道列表
  ChannelList activeChannels_;
  
  // 互斥锁
  MutexLock mutex_;
  
  // 投递函数对象的队列
  std::vector<Functor> pendingFunctors_; // @BuardedBy mutex_
};

}
}
#endif  // MUDUO_NET_EVENTLOOP_H
