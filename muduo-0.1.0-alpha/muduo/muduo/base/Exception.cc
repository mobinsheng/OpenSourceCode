// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/base/Exception.h>

//#include <cxxabi.h>
#include <execinfo.h>
#include <stdlib.h>

using namespace muduo;

Exception::Exception(const char* msg)
  : message_(msg)
{
  // 填充异常栈
  fillStackTrace();
}

Exception::Exception(const string& msg)
  : message_(msg)
{
  fillStackTrace();
}

Exception::~Exception() throw ()
{
}

const char* Exception::what() const throw()
{
  return message_.c_str();
}

const char* Exception::stackTrace() const throw()
{
  return stack_.c_str();
}

void Exception::fillStackTrace()
{
  const int len = 200;
  // 堆栈（用数组模拟）
  void* buffer[len];
  // backtrace函数用于获取当前线程的调用堆栈,获取的信息将会被存放在buffer中
  int nptrs = ::backtrace(buffer, len);
  // backtrace_symbols将从backtrace函数获取的信息转化为一个字符串数组
  char** strings = ::backtrace_symbols(buffer, nptrs);
  
  // 将字符串数组的每个字符串压到stack_（字符串）中
  if (strings)
  {
    for (int i = 0; i < nptrs; ++i)
    {
      // TODO demangle funcion name with abi::__cxa_demangle
      stack_.append(strings[i]);
      stack_.push_back('\n');
    }
    free(strings);
  }
}

