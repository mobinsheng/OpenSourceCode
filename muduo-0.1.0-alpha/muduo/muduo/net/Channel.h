// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_CHANNEL_H
#define MUDUO_NET_CHANNEL_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <muduo/base/Timestamp.h>

namespace muduo
{
namespace net
{

class EventLoop;

///
/// A selectable I/O channel.
///
/// This class doesn't own the file descriptor.
/// The file descriptor could be a socket,
/// an eventfd, a timerfd, or a signalfd

// 信道
class Channel : boost::noncopyable
{
 public:
  typedef boost::function<void()> EventCallback;
  typedef boost::function<void(Timestamp)> ReadEventCallback;
  static const int kNoneEvent;

  Channel(EventLoop* loop, int fd);
  ~Channel();

  // 处理事件
  void handleEvent(Timestamp receiveTime);
  // 设置读的回调函数
  void setReadCallback(const ReadEventCallback& cb)
  { readCallback_ = cb; }
  // 设置写的回调函数
  void setWriteCallback(const EventCallback& cb)
  { writeCallback_ = cb; }
  // 设置关闭的回调函数
  void setCloseCallback(const EventCallback& cb)
  { closeCallback_ = cb; }
  // 设置处理错误的回调函数
  void setErrorCallback(const EventCallback& cb)
  { errorCallback_ = cb; }

  /// Tie this channel to the owner object managed by shared_ptr,
  /// prevent the owner object being destroyed in handleEvent.
  // 这个函数的作用？
  void tie(const boost::shared_ptr<void>&);

  // 获取文件描述符
  int fd() const { return fd_; }
  // 获取事件
  int events() const { return events_; }
  // 设置事件
  void set_revents(int revt) { revents_ = revt; }
  // int revents() const { return revents_; }

  // 使读可用
  void enableReading() { events_ |= kReadEvent; update(); }
  // 使写可用
  void enableWriting() { events_ |= kWriteEvent; update(); }
  // 禁止写
  void disableWriting() { events_ &= ~kWriteEvent; update(); }
  // 禁止读写
  void disableAll() { events_ = kNoneEvent; update(); }
  // 是否正在写
  bool isWriting() const { return events_ & kWriteEvent; }

  // for Poller
  // 获取索引
  int index() { return index_; }
  // 设置索引
  void set_index(int idx) { index_ = idx; }

  // for debug
  string reventsToString() const;

  EventLoop* getLoop() { return loop_; }

 private:
  // 更新
  void update();
  // 带锁的处理事件
  void handleEventWithGuard(Timestamp receiveTime);

  static const int kReadEvent;
  static const int kWriteEvent;

  // 所属的事件循环
  EventLoop* loop_;
  // 文件描述符，由外部传进来（应该是套接字）
  const int  fd_;
  // 事件
  int        events_;
  // 读事件
  int        revents_;
  // 该信道的索引？
  int        index_; // used by Poller.

  boost::weak_ptr<void> tie_;
  bool tied_;
  bool eventHandling_;
  ReadEventCallback readCallback_;
  EventCallback writeCallback_;
  EventCallback closeCallback_;
  EventCallback errorCallback_;
};

}
}
#endif  // MUDUO_NET_CHANNEL_H
