// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_SOCKET_H
#define MUDUO_NET_SOCKET_H

#include <boost/noncopyable.hpp>

namespace muduo
{
///
/// TCP networking.
///
namespace net
{

// 网络地址类
class InetAddress;

///
/// Wrapper of socket file descriptor.
///
/// It closes the sockfd when desctructs.
/// It's thread safe, all operations are delagated to OS.

// 套接字类
class Socket : boost::noncopyable
{
 public:
  explicit Socket(int sockfd)
    : sockfd_(sockfd)
  { }

  ~Socket();

  // 获取套接字描述符
  int fd() const { return sockfd_; }

  /// abort if address in use
  // 绑定地址
  void bindAddress(const InetAddress& localaddr);
  /// abort if address in use
  
  // 监听
  void listen();

  /// On success, returns a non-negative integer that is
  /// a descriptor for the accepted socket, which has been
  /// set to non-blocking and close-on-exec. *peeraddr is assigned.
  /// On error, -1 is returned, and *peeraddr is untouched.
  
  // 根据一个网络地址accept
  int accept(InetAddress* peeraddr);

  // 关闭写端，但是仍然可以读
  void shutdownWrite();

  ///
  /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
  ///
  // 设置非延时的TCP，即发送数据的时候，不会将数据放到缓冲区中
  // 直接发送出去
  void setTcpNoDelay(bool on);

  ///
  /// Enable/disable SO_REUSEADDR
  ///
  // 设置 地址 可以重复使用（复用）
  void setReuseAddr(bool on);

 private:
  const int sockfd_;
};

}
}
#endif  // MUDUO_NET_SOCKET_H
