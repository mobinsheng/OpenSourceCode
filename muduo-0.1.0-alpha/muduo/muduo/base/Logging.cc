#include <muduo/base/Logging.h>

#include <muduo/base/Thread.h>
#include <muduo/base/Timestamp.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <sstream>

namespace muduo
{
// 日志池类
class LoggerImpl
{
 public:
  typedef Logger::LogLevel LogLevel;
  LoggerImpl(LogLevel level, int old_errno, const char* file, int line);
  void finish();

  // 时间戳
  Timestamp time_;
  // 字符串输出流
  std::ostringstream stream_;
  // 级别
  LogLevel level_;
  // 全名
  const char* fullname_;
  // 行
  int line_;
  // 基础名
  const char* basename_;
  // 函数名
  const char* function_;

  static const char* LogLevelName[];
};

// 错误编码的缓存
__thread char t_errnobuf[512];

const char* strerror_tl(int savedErrno)
{
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

// 日志级别初始化
Logger::LogLevel initLogLevel()
{
  if (::getenv("MUDUO_LOG_TRACE"))
    return Logger::TRACE;
  else
	// 默认是调试级别
    return Logger::DEBUG;
}

// 全局的日志级别
Logger::LogLevel g_logLevel = initLogLevel();

}

using namespace muduo;

const char* LoggerImpl::LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "TRACE",
  "DEBUG",
  "INFO",
  "WARN",
  "ERROR",
  "FATAL",
};

// 日志实现
// 记录日志，利用了LoggerImpl临时对象的构造和释放
LoggerImpl::LoggerImpl(LogLevel level, int savedErrno, const char* file, int line)
  : time_(Timestamp::now()),
    stream_(),
    level_(level),
    fullname_(file),
    line_(line),
    basename_(NULL),
    function_(NULL)
{
  // 分隔符
  const char* path_sep_pos = strrchr(fullname_, '/');
  // 基础名
  basename_ = (path_sep_pos != NULL) ? path_sep_pos + 1 : fullname_;
  // 消息头
  char message_head[512];
  // 消息头初始化
  snprintf(message_head, sizeof(message_head), "%s %5d %s ",
      time_.toFormattedString().c_str(), CurrentThread::tid(),
      LogLevelName[level]);
  // 存放到字符串输出流中
  stream_ << message_head;
  // 是否保存错误编号
  if (savedErrno != 0)
  {
    stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
  }
}

// 结束
void LoggerImpl::finish()
{
  stream_ << " - " << basename_ << ":" << line_ << '\n';
}

std::ostream& Logger::stream()
{
  return impl_->stream_;
}

Logger::Logger(const char* file, int line)
  : impl_(new LoggerImpl(INFO, 0, file, line))
{
}

Logger::Logger(const char* file, int line, LogLevel level, const char* func)
  : impl_(new LoggerImpl(level, 0, file, line))
{
  impl_->stream_ << func << ' ';
}

Logger::Logger(const char* file, int line, LogLevel level)
  : impl_(new LoggerImpl(level, 0, file, line))
{
}

Logger::Logger(const char* file, int line, bool toAbort)
  : impl_(new LoggerImpl(toAbort?FATAL:ERROR, errno, file, line))
{
}

Logger::~Logger()
{
  impl_->finish();
  std::string buf(impl_->stream_.str());
  ssize_t n = ::write(1, buf.data(), buf.size());
  //FIXME check n
  (void)n;
  if (impl_->level_ == FATAL)
  {
	// 如果遇到致命错误，直接退出程序
    abort();
  }
}

Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}

void Logger::setLogLevel(Logger::LogLevel level)
{
  g_logLevel = level;
}

