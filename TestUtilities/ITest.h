#pragma once
///////////////////////////////////////////////////////////////////////
// ITest.h - Contract for tests in single-user test harness          //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

namespace Utilities
{
  struct ITest {
    virtual bool test() = 0;
    virtual ~ITest() {}
  };
}
