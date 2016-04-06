// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include <muduo/base/Thread.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>

namespace muduo
{
namespace CurrentThread
{
	/*
	__thread是GCC内置的线程局部存储设施，
	存取效率可以和全局变量相比。
	__thread变量每一个线程有一份独立实体，
	各个线程的值互不干扰。
	可以用来修饰那些带有全局性且值可能变，
	但是又不值得用全局变量保护的变量
	*/
  __thread const char* t_threadName = "unknown";
}
}

namespace
{
// 这一份属于被主线程维护，即进程id（并不一定是主线程（主进程）的id，还有可能是其他线程（Linux的线程用进程实现）的id）
__thread pid_t t_cachedTid = 0;

// 获取线程的真实id（由于线程在Linux下是由进行实现的，但是pthread只提供获取线程id的函数
// 有时候我们需要知道线程真实的进程id，所以使用该函数）
pid_t gettid()
{
  // ::syscall间接系统调用
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    muduo::CurrentThread::t_threadName = "main";
  }
};

// 定义一个全局变量，该全局变量在main函数执行之前被初始化，即构造函数被调用
// 因此当前的线程的名字被设置为main，因为接下来要进入的是main函数，即main线程
ThreadNameInitializer init;
}

using namespace muduo;

// 获取当前的线程id
pid_t CurrentThread::tid()
{
  if (t_cachedTid == 0)
  {
    t_cachedTid = gettid();
  }
  return t_cachedTid;
}

// 获取当前线程的名字
const char* CurrentThread::name()
{
  return t_threadName;
}

// 判断是否为主线程，主线程的id和进程id相同
bool CurrentThread::isMainThread()
{
  return tid() == ::getpid();
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
  : started_(false),
    pthreadId_(0),
    tid_(0),
    func_(func),
    name_(n)
{
  // 每创建一个线程，线程的创建次数就加一
  numCreated_.increment();
}

Thread::~Thread()
{
}

void Thread::start()
{
  // 该线程必须是未启动的，否则出错
  assert(!started_);
  started_ = true;
  // 创建线程
  pthread_create(&pthreadId_, NULL, &startThread, this);
}

void Thread::join()
{
  // 如果该线程没有启动，那么出错
  assert(started_);
  // 等待线程执行完毕
  pthread_join(pthreadId_, NULL);
}

void* Thread::startThread(void* obj)
{
  // 启动线程，实际调用的是thread的runThread函数
  Thread* thread = static_cast<Thread*>(obj);
  thread->runInThread();
  return NULL;
}

void Thread::runInThread()
{
  // 获取当前线程的id
  tid_ = CurrentThread::tid();
  // 获取当前线程的名字
  muduo::CurrentThread::t_threadName = name_.c_str();
  // 运行执行具体线程任务的函数
  func_();
  // 执行完成之后，名字被命名为finished
  muduo::CurrentThread::t_threadName = "finished";
}

