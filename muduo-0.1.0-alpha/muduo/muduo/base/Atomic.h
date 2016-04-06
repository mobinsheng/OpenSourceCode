// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_ATOMIC_H
#define MUDUO_BASE_ATOMIC_H

#include <boost/noncopyable.hpp>
#include <stdint.h>

// muduo命名空间
namespace muduo
{

// detail命名空间
namespace detail
{
template<typename T>
// boost::noncopyable的适用于单例，不可复制，也不可赋值
// 原子整形（其实不一定是整形）
class AtomicIntegerT : boost::noncopyable
{
 public:
  AtomicIntegerT()
    : value_(0)
  {
  }

  // uncomment if you need copying and assignment
  //
  // AtomicIntegerT(const AtomicIntegerT& that)
  //   : value_(that.get())
  // {}
  //
  // AtomicIntegerT& operator=(const AtomicIntegerT& that)
  // {
  //   getAndSet(that.get());
  //   return *this;
  // }

  
  // __sync_val_compare_and_swap等函数是gcc提供的原子操作函数
  T get()
  {
	  // 提供了原子级别的比较和交换
    return __sync_val_compare_and_swap(&value_, 0, 0);
  }

  // 获取并加上x，先获取值，然后再原值的基础上加x
  T getAndAdd(T x)
  {
    return __sync_fetch_and_add(&value_, x);
  }

  // 先加x，然后获取值
  T addAndGet(T x)
  {
    return getAndAdd(x) + x;
  }

  // 先加1，然后获取值
  T incrementAndGet()
  {
    return addAndGet(1);
  }

  // 加上x
  void add(T x)
  {
    getAndAdd(x);
  }

  // 加1
  void increment()
  {
    incrementAndGet();
  }

  // 减1
  void decrement()
  {
    getAndAdd(-1);
  }

  // 获取它的值，然后把它设置为newValue
  T getAndSet(T newValue)
  {
    return __sync_lock_test_and_set(&value_, newValue);
  }

 private:
  // volatile主要用来修饰可能被不同线程访问和修改的变量，
  // 如果不加入volatile，基本上会导致这样的结果：要么无法编写多线程程序，
  // 要么编译器失去大量优化的机会
  // 优化器在用到这个变量时必须每次都小心地重新读取这个变量的值，而不是使用保存在寄存器里的备份
  volatile T value_;
};
}

// 原子型的int32
typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
// 原子型的int64
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;
}

#endif  // MUDUO_BASE_ATOMIC_H
