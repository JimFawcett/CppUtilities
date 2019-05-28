/////////////////////////////////////////////////////////////////////
// DateTime.cpp - represents clock time                            //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "DateTime.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <unordered_map>

#pragma warning(disable : 4267)  // disable warning about loss of significance

using namespace Utilities;

//----< replaces std::ctime using ctime_s >--------------------------

char* DateTime::ctime(const std::time_t* pTime)
{
  const rsize_t buffSize = 26;
  static char buffer[buffSize];
  errno_t err = ctime_s(buffer, buffSize, pTime);
  return buffer;
}
//----< replaces std::localtime using localtime_s >------------------

std::tm* DateTime::localtime(const std::time_t* pTime)
{
  static std::tm result;
  errno_t err = localtime_s(&result, pTime);
  return &result;
}
//----< construct DateTime instance with current system time >-------

DateTime::DateTime()
{
  tp_ = SysClock::now();
}
//----< construct DateTime from formatted time string >--------------
/* 
 * Helper function that reads each part of the formatted string 
 */
int readDateTimePart(int& part, std::istringstream& in)
{
  if (in.peek() == ':')
    in.get();
  in >> part;
  //std::cout << "\n  -- part = " << part << " --";
  if (in.good())
    return part;
  return -1;
}
//----< makes a DateTime instance from a formatted string >----------
/*
*  Throws exception if string is an invalid DateTime string
*/
DateTime::DateTime(std::string dtStr)
{
  static std::unordered_map<std::string, size_t> months = {
    { "Jan", 1 }, { "Feb", 2 }, { "Mar", 3 }, { "Apr", 4 }, 
    { "May", 5 }, { "Jun", 6 }, { "Jul", 7 }, { "Aug", 8 }, 
    { "Sep", 9 }, { "Oct", 10 }, { "Nov", 11 }, { "Dec", 12 } 
  };
  std::istringstream in(dtStr);
  std::string day, month;
  in >> day;
  in >> month;
  if (!in.good())
    throw std::exception("invalid DateTime string");
  std::tm date;
  date.tm_mon = months[month] - 1;
  readDateTimePart(date.tm_mday, in);
  readDateTimePart(date.tm_hour, in);
  readDateTimePart(date.tm_min, in);
  readDateTimePart(date.tm_sec, in);
  readDateTimePart(date.tm_year, in);
  date.tm_year -= 1900;
  date.tm_isdst = -1;
  std::time_t time = std::mktime(&date);
  if (time == -1)
  {
    throw "invalid system time";
  }
  tp_ = SysClock::from_time_t(time);  
}
//----< cast operator converts to time formatted string >------------

DateTime::operator std::string()
{
  return time();
}
//----< construct DateTime instance from time point >----------------

DateTime::DateTime(const DateTime::TimePoint& tp) : tp_(tp) {}

//----< make a time from integral parts >----------------------------

DateTime::TimePoint DateTime::makeTime(
  size_t year, size_t mon, size_t day, size_t hour, size_t min, size_t sec
)
{
  struct std::tm t;
  t.tm_sec = sec;
  t.tm_min = min;
  t.tm_hour = hour;
  t.tm_mday = day;
  t.tm_mon = mon-1;
  t.tm_year = year - 1900;
  t.tm_isdst = -1;
  std::time_t time = std::mktime(&t);
  if (time == -1)
  {
    throw "invalid system time";
  }
  return SysClock::from_time_t(time);
}
//----< make duration from integral parts >--------------------------

DateTime::Duration DateTime::makeDuration(
  size_t hour, size_t min, size_t sec, size_t millisec
)
{
  Duration dur = 
    std::chrono::milliseconds(millisec) + 
    std::chrono::seconds(sec) + 
    std::chrono::minutes(min) + 
    std::chrono::hours(hour);
  return dur;
}
//----< set time to system time >------------------------------------

std::string DateTime::now()
{
  TimePoint tp = SysClock::now();
  std::time_t t = SysClock::to_time_t(tp);
  std::string ts = ctime(&t);
  ts.resize(ts.size() - 1);
  return ts;
}
//----< return internal time point >---------------------------------

DateTime::TimePoint DateTime::timepoint()
{
  return tp_;
}
//----< return seconds from Jan 1 1990 at midnight >-----------------

size_t DateTime::ticks()
{
  auto int_sec = std::chrono::duration_cast<std::chrono::seconds>(tp_.time_since_epoch());
  return static_cast<size_t>(int_sec.count());
}
//----< return formatted time string >-------------------------------

std::string DateTime::time()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::string ts = ctime(&t);
  ts.resize(ts.size() - 1);
  return ts;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator<(const DateTime& dt)
{
  return tp_ < dt.tp_;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator>(const DateTime& dt)
{
  return tp_ > dt.tp_;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator==(const DateTime& dt)
{
  return tp_ == dt.tp_;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator!=(const DateTime& dt)
{
  return tp_ != dt.tp_;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator<=(const DateTime& dt)
{
  return tp_ <= dt.tp_;
}
//----< compare DateTime instances >---------------------------------

bool DateTime::operator>=(const DateTime& dt)
{
  return tp_ >= dt.tp_;
}
//----< add a duration to DateTime instance >------------------------

DateTime DateTime::operator+=(const DateTime::Duration& dur)
{
  tp_ += dur;
  return *this;
}
//----< make DateTime from instance time plus duration >-------------

DateTime DateTime::operator+(const DateTime::Duration& dur)
{

  return DateTime(tp_ + dur);
}
//----< subtract a duration from instance >--------------------------

DateTime DateTime::operator-=(const DateTime::Duration& dur)
{
  tp_ -= dur;
  return *this;
}
//---- make DateTime from instance time minus duration >-------------

DateTime DateTime::operator-(const DateTime::Duration& dur)
{
  return DateTime(tp_ - dur);
}
//----< return year count >------------------------------------------

size_t DateTime::year()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_year;
}
//----< return month count >-----------------------------------------

size_t DateTime::month()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_mon;
}
//----< return day count >-------------------------------------------

size_t DateTime::day()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_mday;
}
//----< return hour count >------------------------------------------

size_t DateTime::hour()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_hour;
}
//----< return minutes count >---------------------------------------

size_t DateTime::minute()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_min;
}
//----< return seconds count >---------------------------------------

size_t DateTime::second()
{
  std::time_t t = SysClock::to_time_t(tp_);
  std::tm tm = *localtime(&t);
  return tm.tm_sec;
}

//----< test stub >--------------------------------------------------

#ifdef TEST_DATETIME

#include <iostream>
#include "../StringUtilities/StringUtilities.h"

int main()
{
  Utilities::Title("Testing DateTime class");
 
  try {
    DateTime dt;
    std::cout << "\n  " << dt.time();
    dt += DateTime::makeDuration(24, 0);
    std::cout << "\n  " << dt.time();
    dt += DateTime::makeDuration(1, 10);
    std::cout << "\n  " << dt.time();
    DateTime now;
    std::cout << "\n  using cast operator: " << static_cast<std::string>(now);

    if (now < dt)
      std::cout << "\n  " << now.time() << " is less than " << dt.time();
    else
      std::cout << "\n  " << now.time() << " is not less than " << dt.time();
    std::cout << "\n  now.ticks() = " << now.ticks();
    std::cout << "\n  constructing DateTime from formated DateTime string";
    DateTime newDt(dt.time());
    std::cout << "\n  " << newDt.time();
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n  -- " << ex.what() << " --";
  }
  std::cout << "\n\n";
}
#endif
