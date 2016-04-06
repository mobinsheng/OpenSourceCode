// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_BASE_PROCESSINFO_H
#define MUDUO_BASE_PROCESSINFO_H

#include <muduo/base/Types.h>
#include <vector>

namespace muduo
{

// 进程信息
namespace ProcessInfo
{
  // 获取进程id
  pid_t pid();
  // 字符串类型的进程id
  string pidString();
  // 获取用户id
  uid_t uid();
  // 获取用户名
  string username();
  // 获取主机名
  string hostname();
  
  /// read /proc/self/status
  
  // 获取进程状态
  string procStatus();

  // 获取打开的文件的数量
  int openedFiles();

  // 该进程拥有的线程的集合
  std::vector<pid_t> threads();
}

}

#endif  // MUDUO_BASE_PROCESSINFO_H
