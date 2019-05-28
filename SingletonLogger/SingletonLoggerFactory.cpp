/////////////////////////////////////////////////////////////////////
// SingletonLoggerFactory.cpp - Facility for creating loggers      //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  - This file includes the SingletonLogger implementation.
*  - Its header file does not include the implementation.
*  - Note that this code must be specific instantiations of
*    factories we expect to use.  That is due to the fact 
*    that client's won't include this file.
*/
#include "SingletonLoggerFactory.h"
#include "SingletonLogger.h"  

using namespace Utilities;

ILogger<0, Lock>* SingletonLoggerFactory<0, Lock>::getInstance()
{
  return Logger<0, Lock>::getInstance();
}

ILogger<0, NoLock>* SingletonLoggerFactory<0, NoLock>::getInstance()
{
  return Logger<0, NoLock>::getInstance();
}

ILogger<1, Lock>* SingletonLoggerFactory<1, Lock>::getInstance()
{
  return Logger<1, Lock>::getInstance();
}

ILogger<1, NoLock>* SingletonLoggerFactory<1, NoLock>::getInstance()
{
  return Logger<1, NoLock>::getInstance();
}
