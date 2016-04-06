// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_SINGLETON_H
#define MUDUO_BASE_SINGLETON_H

#include <boost/noncopyable.hpp>
#include <pthread.h>
#include <stdlib.h> // atexit

namespace muduo
{

// 单件类
template<typename T>
class Singleton : boost::noncopyable
{
 public:
  static T& instance()
  {
	// 需要且仅需要一次初始化，并且不会被多线程的执行打乱
    pthread_once(&ponce_, &Singleton::init);
    return *value_;
  }

 private:
  Singleton();
  ~Singleton();

  static void init()
  {
    value_ = new T();
	
	// 注册销毁函数
    ::atexit(destroy);
  }

  static void destroy()
  {
    delete value_;
  }

 private:
  // 需要且仅需要一次初始化
  static pthread_once_t ponce_;
  static T*             value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

}
#endif

