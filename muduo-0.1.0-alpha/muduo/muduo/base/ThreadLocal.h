// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREADLOCAL_H
#define MUDUO_BASE_THREADLOCAL_H

#include <boost/noncopyable.hpp>
#include <pthread.h>

namespace muduo
{

// 线程中特有的线程存储
template<typename T>
class ThreadLocal : boost::noncopyable
{
 public:
  // 创建一个线程存储
  ThreadLocal()
  {
    pthread_key_create(&pkey_, &ThreadLocal::destructor);
  }

  // 销毁一个线程存储
  ~ThreadLocal()
  {
    pthread_key_delete(pkey_);
  }

  // 取值
  T& value()
  {
	// 取一个属于线程特有对象
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
	
	// 首先判断该线程特有的对象是否存在
    if (!perThreadValue) {
	  // 如果该线程对象不存在，那么就创建一个对象
      T* newObj = new T();
	  // 讲该对象设置为线程特有对象
      pthread_setspecific(pkey_, newObj);
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:

 // 销毁这个线程特有的对象
  static void destructor(void *x)
  {
    T* obj = static_cast<T*>(x);
    delete obj;
  }

 private:
  pthread_key_t pkey_;
};

}
#endif
