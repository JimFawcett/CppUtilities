#pragma once
/////////////////////////////////////////////////////////////////////
// SingletonLogger.h - Provides logging to multiple streams        //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines Logger<Category, Locker>:
*  - A singleton logger that supports one logger for each category (int)
*    and supports multiple std::iostreams, with each write sending logs
*    to each of its streams.  Logger will accept any stream derived from
*    std::iostream, e.g., ofstream and ostringstream.
*  - NoLock<0>:  Logger<0,NoLock> definded in ISingletonLogger.h
*    A class used to provide a locking policy for loggers.  It provides
*    empty lock and unlock methods, intended for use in a single-threaded
*    environment.  The empty methods will be optimized away by the compiler.
*  - Lock<0>: Logger<0,Lock> defined in ISingletonLogger.h
*    A class used to provide a locking policy for loggers.  It provides
*    lock and unlock methods that use a std::mutex to provide mutually
*    exclusive access to its instantiation method, getInstance.
*
*  Required Files:
*  ---------------
*  SingletonLogger.h, SingletonLogger.cpp (needed only for testing)
*  ISingletonLogger.h
*  DateTime.h, DateTime.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 07 Oct 2018
*  - added implementation of interface
*  ver 1.0 : 23 Sep 2018
*  - first release
*/
#include <iostream>
#include <fstream>
#include <sstream>   // defines std::getline(std::istream, std::string)
#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include "ISingletonLogger.h"
#include "../DateTime/DateTime.h"

namespace Utilities
{
  using Streams = std::vector<std::ostream*>;

  ///////////////////////////////////////////////////////////////////
  // Logger class
  // - Thread-safe singleton
  // - supports logging to multiple streams with a single write
  // - int template parameter supports multiple logs shared by
  //   the value of that parameter.
  // - Locker template parameter supports removing locking in
  //   single-threaded environments.
  // - uses template template parameter to ensure that
  //   Logger and Locker use the same Category

  using Streams = std::vector<std::ostream*>;
  using Terminator = std::string;

  template<int Category, template<int Category> class Locker>
  class Logger : public ILogger<Category, Locker>
  {
  public:
    Logger(const Logger<Category, Locker>&) = delete;
    Logger<Category, Locker>& operator=(const Logger<Category, Locker>&) = delete;

    void addStream(std::ostream* pStream) override;
    bool usingStream(std::ostream* pStream) override;
    bool removeStream(std::ostream* pStream) override;

    void setTerminator(const Terminator& term) override;
    void setAuthor(const std::string& name) override;

    void writeHead(const std::string& msg) override;
    void write(const std::string& text) override;
    void writeTail(const std::string& msg = "end of log") override;

    static Logger<Category, Locker>* getInstance();

  private:
    Logger()
    {
      addStream(&std::cout);
    }
    static Logger<Category, Locker>* instance_;
    static Locker<Category> locker_;
    Streams streams_;
    Terminator trm_ = "\n  ";  // default item terminator
    std::string author_ = "no author";
  };

  // define static data members
  
  template<int Category, template<int Category> class Locker>
  Logger<Category, Locker>* Logger<Category, Locker>::instance_;

  template<int Category, template<int Category> class Locker>
  Locker<Category> Logger<Category, Locker>::locker_;

  // define methods

  //----< register a stream - adds to default std::cout >------------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::addStream(std::ostream* pStream)
  {
    streams_.push_back(pStream);
  }
  //----< is pStream registered with logger? >-----------------------

  template<int Category, template<int Category> class Locker>
  bool Logger<Category, Locker>::usingStream(std::ostream* pStream)
  {
    Streams::iterator iter = streams_.begin();
    for (size_t i = 0; i < streams_.size(); ++i)
    {
      if (pStream == streams_[i])
      {
        return true;
      }
      ++iter;
    }
    return false;
  }
  //----< remove stream - can use to remove std::cout >--------------

  template<int Category, template<int Category> class Locker>
  bool Logger<Category, Locker>::removeStream(std::ostream* pStream)
  {
    Streams::iterator iter = streams_.begin();
    for (size_t i = 0; i < streams_.size(); ++i)
    {
      if (pStream == streams_[i])
      {
        streams_.erase(iter);
        return true;
      }
      ++iter;
    }
    return false;
  }
  //----< set log write terminator - always added to write >---------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::setTerminator(const Terminator& term) { trm_ = term; }
  
  //----< add log author's name >------------------------------------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::setAuthor(const std::string& name) { author_ = name; }

  //----< write first log entry with message >-----------------------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::writeHead(const std::string& msg)
  {
    std::string headerMsg = msg + " : " + author_ + trm_;
    headerMsg += DateTime().now() + trm_;
    for (auto pStrm_ : streams_)
    {
      *pStrm_ << headerMsg;
    }
  }
  //----< write log entry >------------------------------------------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::write(const std::string& text)
  {
    std::string logMsg = text + trm_;
    for (auto pStrm_ : streams_)
      *pStrm_ << logMsg;
  }
  //----< write last entry of log >----------------------------------

  template<int Category, template<int Category> class Locker>
  void Logger<Category, Locker>::writeTail(const std::string& msg)
  {
    for (auto pStrm_ : streams_)
      *pStrm_ << msg.c_str();
  }
  //----< get pointer to singleton logger >--------------------------
  //
  // Thread-safe singleton access:
  // - Does not attempt to improve performance by double-check locking
  // - That may fail occasionally, in C++, due to caching of instance_.
  // - Since accesses are rare, usually only a very few times per
  //   execution, performance degradation is very small.

  template<int Category, template<int Category> class Locker>
  Logger<Category, Locker>* Logger<Category, Locker>::getInstance()
  {
    Locker<Category>::lock();
    if (instance_ == nullptr)
    {
      instance_ = new Logger<Category, Locker>();
    }
    Locker<Category>::unlock();

    return instance_;
  }
}