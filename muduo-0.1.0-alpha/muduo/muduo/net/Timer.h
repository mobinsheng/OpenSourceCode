// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <map>

#include <boost/noncopyable.hpp>

#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>

namespace muduo
{
namespace net
{
///
/// Internal class for timer event.
///
// 计时器/定时器
class Timer : boost::noncopyable
{
 public:
  Timer(const TimerCallback& cb, Timestamp when, double interval)
    : callback_(cb),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0)
  { }

  void run() const
  {
    callback_();
  }

  // 终止
  Timestamp expiration() const  { return expiration_; }
  
  // 是否重复
  bool repeat() const { return repeat_; }

  // 重新开始
  void restart(Timestamp now);

 private:
  // 计时器到时的回调函数
  const TimerCallback callback_;
  // 终止的时间戳
  Timestamp expiration_;
  // 间隔
  const double interval_;
  // 是否重复
  const bool repeat_;
};
}
}
#endif  // MUDUO_NET_TIMER_H
