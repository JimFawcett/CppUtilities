#pragma once
/////////////////////////////////////////////////////////////////////
// ISingletonLogger.h - Interface for logging to multiple streams  //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines three classes:
*  - ILogger<Category, Locker>:
*    An Interface for singleton logger that supports one logger for each
*    category (int template argument).  SingletonLoggersupports multiple
*    std::iostreams, with each write sending log items to each of its 
*    streams.  Logger will accept any stream derived from std::iostream, 
*    e.g., ofstream and ostringstream.
*  - NoLock<Category>:
*    A class used to provide a locking policy for loggers.  It provides
*    empty lock and unlock methods, intended for use in a single-threaded
*    environment.  The empty methods will be optimized away by the compiler.
*  - Lock<Category>:
*    A class used to provide a locking policy for loggers.  It provides
*    lock and unlock methods that use a std::mutex to provide mutually
*    exclusive access to its instantiation method, getInstance.
*  - Category is an integer value, used to partition loggers into groups.
*    Each group, i.e. a specific interger value, shares the same logger.
*
*  Required Files:
*  ---------------
*  ISingletonLogger.h
*
*  Maintenance History:
*  --------------------
*  ver 1.2 : 07 Oct 2018
*  - modified comments
*  ver 1.1 : 27 Sep 2018
*  - extracted interface ILogger<int, Locker> as separate package
*  ver 1.0 : 23 Sep 2018
*  - first release
*/

#include <mutex>
#include <string>
#include <iostream>

namespace CodeUtilities
{
  ///////////////////////////////////////////////////////////////////
   // NoLock<Category> class
   // - Template parameter for single-threaded environment

  template<int Category>
  class NoLock
  {
  public:
    static void lock() {}
    static void unlock() {}
  };

  ///////////////////////////////////////////////////////////////////
  // Lock<Category> class
  // - Logger template parameter for multi-threaded environment
  // - Category is a template parameter used to define categories
  //   of loggers

  template<int Category>
  class Lock
  {
  public:
    Lock() {}
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;

    static void lock()
    {
      mtx_.lock();
    }
    static void unlock()
    {
      mtx_.unlock();
    }
  private:
    static std::recursive_mutex mtx_;
  };

  template<int Category>
  std::recursive_mutex Lock<Category>::mtx_;

  ///////////////////////////////////////////////////////////////////
  // ILogger Interface

  using Terminator = std::string;

  template<int Category, template<int Category> class Locker>
  class ILogger {
  public:
    virtual ~ILogger() {}

    virtual void addStream(std::ostream* pStream) = 0;
    virtual bool usingStream(std::ostream* pStream) = 0;
    virtual bool removeStream(std::ostream* pStream) = 0;

    virtual void setTerminator(const Terminator& term) = 0;
    virtual void setAuthor(const std::string& name) = 0;

    virtual void writeHead(const std::string& msg) = 0;
    virtual void write(const std::string& text) = 0;
    virtual void writeTail(const std::string& msg = "end of log") = 0;
  };
}
