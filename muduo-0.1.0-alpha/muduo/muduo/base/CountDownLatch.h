// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include <muduo/base/Condition.h>
#include <muduo/base/Mutex.h>

#include <boost/noncopyable.hpp>

namespace muduo
{
// 倒数计时器
class CountDownLatch : boost::noncopyable
{
 public:

  CountDownLatch(int count);

  // 等待计数变为0
  void wait();

  // 计数减一
  void countDown();

  // 获取当前剩余的计时
  int getCount() const;

 private:
  // 互斥锁
  // mutable关键字的说明：
  // 如果类的成员函数不会改变对象的状态，
  // 那么这个成员函数一般会声明成const的。
  // 但是，有些时候，我们需要在const的函数里面修改一些跟类状态无关的数据成员，
  // 那么这个数据成员就应该被mutalbe来修饰
  mutable MutexLock mutex_;
  // 条件变量
  Condition condition_;
  
  // 计时
  int count_;
};

}
#endif  // MUDUO_BASE_COUNTDOWNLATCH_H
