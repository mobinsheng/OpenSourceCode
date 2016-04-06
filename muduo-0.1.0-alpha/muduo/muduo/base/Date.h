// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_DATE_H
#define MUDUO_BASE_DATE_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>

struct tm;

namespace muduo
{

///
/// Date in Gregorian calendar.
///
/// This class is immutable.
/// It's recommended to pass it by value, since it's passed in register on x64.
///

// 日期类
class Date : public muduo::copyable
          // public boost::less_than_comparable<Date>,
          // public boost::equality_comparable<Date>
{
 public:

 // 年月日结构体
  struct YearMonthDay
  {
    int year; // [1900..2500]
    int month;  // [1..12]
    int day;  // [1..31]
  };

  // 每一周的天数。。。。
  static const int kDaysPerWeek = 7;

  ///
  /// Constucts an invalid Date.
  ///
  Date()
    : julianDayNumber_(0) // 朱利安时间
  {}

  ///
  /// Constucts a yyyy-mm-dd Date.
  ///
  /// 1 <= month <= 12
  Date(int year, int month, int day);

  ///
  /// Constucts a Date from Julian Day Number.
  ///
  explicit Date(int julianDayNum)
    : julianDayNumber_(julianDayNum)
  {}

  ///
  /// Constucts a Date from struct tm
  ///
  explicit Date(const struct tm&);

  // default copy/assignment/dtor are Okay

  // 时间交换
  void swap(Date& that)
  {
    std::swap(julianDayNumber_, that.julianDayNumber_);
  }

  // 判断时间是否有效
  bool valid() const { return julianDayNumber_ > 0; }

  ///
  /// Converts to yyyy-mm-dd format.
  ///
  string toIsoString() const;

  struct YearMonthDay yearMonthDay() const;

  // 获取年
  int year() const
  {
    return yearMonthDay().year;
  }

  // 获取月
  int month() const
  {
    return yearMonthDay().month;
  }

  // 获取 日
  int day() const
  {
    return yearMonthDay().day;
  }

  // [0, 1, ..., 6] => [Sunday, Monday, ..., Saturday ]
  // 获取今天属于一周中的第几天
  int weekDay() const
  {
    return (julianDayNumber_+1) % kDaysPerWeek;
  }

  int julianDayNumber() const { return julianDayNumber_; }

 private:
  int julianDayNumber_;
};

inline bool operator<(Date x, Date y)
{
  return x.julianDayNumber() < y.julianDayNumber();
}

// 判断两个时间是否相等
inline bool operator==(Date x, Date y)
{
  return x.julianDayNumber() == y.julianDayNumber();
}

}
#endif  // MUDUO_BASE_DATE_H
