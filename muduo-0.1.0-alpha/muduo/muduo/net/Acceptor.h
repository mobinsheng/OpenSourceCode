// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <muduo/net/Channel.h>
#include <muduo/net/Socket.h>

namespace muduo
{
namespace net
{

class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
// 接收器（接受者）
class Acceptor : boost::noncopyable
{
 public:
 
  // 客户端连接的时候的回调函数
  typedef boost::function<void (int sockfd, const InetAddress&)>
    NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr);
  // ~Acceptor();

  // 设置新连接到来的时候的回调函数
  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }
  
  // 是否正在监听
  bool listenning() const { return listenning_; }
  
  // 开始监听
  void listen();

 private:
  // 处理读数据
  void handleRead();
  
  // 事件循环
  EventLoop* loop_;
  
  // 接受者的套接字
  Socket acceptSocket_;
  Channel acceptChannel_;
  NewConnectionCallback newConnectionCallback_;
  bool listenning_;
};

}
}

#endif  // MUDUO_NET_ACCEPTOR_H
