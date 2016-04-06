// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_EXCEPTION_H
#define MUDUO_BASE_EXCEPTION_H

#include <muduo/base/Types.h>
#include <exception>

namespace muduo
{

// 异常类，继承自标准库的异常类
class Exception : public std::exception
{
 public:
  explicit Exception(const char* what);
  explicit Exception(const string& what);
  virtual ~Exception() throw();
  
  // 获取异常信息
  virtual const char* what() const throw();
  
  // 堆栈跟踪
  const char* stackTrace() const throw();

 private:
  void fillStackTrace();

  // 异常信息
  string message_;
  
  // 异常堆栈
  string stack_;
};

}

#endif  // MUDUO_BASE_EXCEPTION_H
