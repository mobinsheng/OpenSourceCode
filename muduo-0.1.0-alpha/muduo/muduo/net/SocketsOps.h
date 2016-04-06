// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_SOCKETSOPS_H
#define MUDUO_NET_SOCKETSOPS_H

#include <arpa/inet.h>

namespace muduo
{
namespace net
{
// 套接字选项类
namespace sockets
{

// the inline assembler code makes type blur,
// so we disable warnings for a while.
#pragma GCC diagnostic ignored "-Wconversion"
// 将主机字节转换成网络的32位格式
inline uint32_t hostToNetwork32(uint32_t hostlong)
{
  return htonl(hostlong);
}

// 将主机字节转换为网络的16位格式
inline uint16_t hostToNetwork16(uint16_t hostshort)
{
  return htons(hostshort);
}

// 将网络字节转换为主机的32位
inline uint32_t networkToHost32(uint32_t netlong)
{
  return ntohl(netlong);
}

// 将网络字节转换为主机的16位
inline uint16_t networkToHost16(uint16_t netshort)
{
  return ntohs(netshort);
}
#pragma GCC diagnostic error "-Wconversion"

///
/// Creates a non-blocking socket file descriptor,
/// abort if any error.

// 创建一个非阻塞的套接字
int createNonblockingOrDie();

// 连接
int  connect(int sockfd, const struct sockaddr_in& addr);
// 绑定
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
// 监听
void listenOrDie(int sockfd);
// 接收
int  accept(int sockfd, struct sockaddr_in* addr);
// 关闭套接字
void close(int sockfd);

// 关闭写端套接字
void shutdownWrite(int sockfd);

// 将地址转换为主机和端口
void toHostPort(char* buf, size_t size,
                const struct sockaddr_in& addr);

// 将主机id和端口转换为地址
void fromHostPort(const char* ip, uint16_t port,
                  struct sockaddr_in* addr);

// 获取错误编码
int getSocketError(int sockfd);

// 获取本地地址
struct sockaddr_in getLocalAddr(int sockfd);

// 获取对方地址
struct sockaddr_in getPeerAddr(int sockfd);

// 是否为自己连接自己
bool isSelfConnect(int sockfd);

}
}
}

#endif  // MUDUO_NET_SOCKETSOPS_H
