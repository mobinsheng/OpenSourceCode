// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_POLLER_H
#define MUDUO_NET_POLLER_H

#include <vector>
#include <boost/noncopyable.hpp>

#include <muduo/base/Timestamp.h>

namespace muduo
{
namespace net
{

class Channel;
class EventLoop;

///
/// Base class for IO Multiplexing
///
/// This class doesn't own the Channel objects.
// 轮询器
class Poller : boost::noncopyable
{
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  virtual ~Poller();

  /// Polls the I/O events.
  /// Must be called in the loop thread.
  // 轮询事件
  virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

  /// Changes the interested I/O events.
  /// Must be called in the loop thread.
  // 更新信道
  virtual void updateChannel(Channel* channel) = 0;

  /// Remove the channel, when it destructs.
  /// Must be called in the loop thread.
  // 移除信道
  virtual void removeChannel(Channel* channel) = 0;

  // 创建一个默认的轮询器
  static Poller* newDefaultPoller(EventLoop* loop);

  void assertInLoopThread();

 private:
  EventLoop* ownerLoop_;
};

}
}
#endif  // MUDUO_NET_POLLER_H
