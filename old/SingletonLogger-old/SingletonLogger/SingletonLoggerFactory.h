#pragma once
/////////////////////////////////////////////////////////////////////
// SingletonLoggerFactory.h - Facility for creating loggers        //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package defines SingletonLoggerFactory<Category, Locker>:
*  - Creates a singleton logger that supports one logger for each 
*    Category (int) and supports multiple std::iostreams, with each
*    write sending logs to each of its streams.
*  - Uses Locker template parameter which may be either of:
*    - NoLock<0>:  SingletonLoggerFactory<0,NoLock> definded in 
*      ISingletonLogger.h
*    - Lock<0>: SingletonLoggerFactory<0,Lock> defined in 
*      ISingletonLogger.h
*  - Note that this file does not include any implementation details
*    of the singleton logger, so clients that use it are not dependent
*    on those details.
*
*  Required Files:
*  ---------------
*  SingletonLoggerFactory.h, SingletonLoggerFactory.cpp
*  ISingletonLogger.h
*  Static Library for SingletonLogger
*    or
*  SingletonLogger.h, SingletonLogger.cpp (needed only for testing)
*  DateTime.h, DateTime.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 07 Oct 2018
*  - first release
*/

#include "ISingletonLogger.h"

namespace CodeUtilities
{
  template<int Category, template<int Category> class Locker>
  struct SingletonLoggerFactory
  {
    static ILogger<Category, Locker>* getInstance();
  };
}
