///////////////////////////////////////////////////////////////////////
// TestUtilities.cpp - provides single-user test harness             //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include <functional>
#include "TestUtilities.h"
#include "../StringUtilities/StringUtilities.h"

#ifdef TEST_TESTUTILITIES

using namespace Utilities;

///////////////////////////////////////////////////////////////////////
// define demo tests

bool test_always_passes() { return true; }
bool test_always_fails() { return false; }
bool test_always_throws() {
  std::exception ex("exception\n         -- msg: this test always throws -- ");
  throw(ex);
}

int main()
{
  Title("Testing TestUtilities Package");
  putline();

  TestExecutive ex;

  TestExecutive::TestItem ti1{ test_always_passes, "test_always_passes" };
  TestExecutive::TestItem ti2{ test_always_fails, "test_always_fails" };
  TestExecutive::TestItem ti3{ test_always_throws, "test_always_throws" };
  
  ex.registerTest(ti1);
  ex.registerTest(ti2);
  ex.registerTest(ti3);

  bool result = ex.doTests();
  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  putline(2);
  return 0;
}
#endif
