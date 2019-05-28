///////////////////////////////////////////////////////////////////////
// TestUtilitiesDemo.cpp - facilities for single-user test harness   //
// ver 2.2                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include <memory>
#include "TestUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../StringUtilities/StringUtilities.h"
#include "../FileSystem/FileSystem.h"

std::ostream& out = std::cout;

#ifdef TEST_TESTUTILITIES

namespace Utilities
{
  ///////////////////////////////////////////////////////////////////////
  // define demo tests - testing the tester
  // - destructors in each test simply demonstrate that
  //   std::shared_ptr<ITest> is doing its job.

  class TestPasses : public ITest
  {
  public:
    bool test() { return true; }
    virtual ~TestPasses() { out << "\n  -- deleting TestPasses"; }
  };

  class TestFails : public ITest
  {
  public:
    bool test() { return false; }
    ~TestFails() { out << "\n  -- deleting TestFails"; }
  };

  class TestThrows : public ITest
  {
  public:
    bool test()
    {
      std::exception ex("\n  -- this test always throws -- ");
      throw ex;
    }
    ~TestThrows() { out << "\n  -- deleting TestThrows"; }
  };

  // define a test that tests other code

  class CodeToTest
  {
  public:
    CodeToTest(const std::string& msg) : msg_(msg) {}
    std::string result()
    {
      return "result" + msg_;
    }
    ~CodeToTest() { out << "\n  -- deleting CodeToTest\n"; }
  private:
    std::string msg_;
  };

  class TestCode : public ITest
  {
  public:
    TestCode(const std::string& msg) : msg_(msg) {}
    bool test()
    {
      out << "\n  test input = \"" + msg_ << "\"";     // logging test input

      CodeToTest ctt(msg_);
      std::string result = ctt.result();

      out << "\n  test output = \"" + result << "\"";  //logging test output
      return (result == "result" + msg_);
    }
    ~TestCode() { out << "\n  -- deleting TestCode"; }
  private:
    std::string msg_;
  };
}

using namespace Utilities;

int main()
{
  Cosmetic cos;

  std::cout << "\n  Testing TestUtilities Package";
  std::cout << "\n ===============================\n";

  TestExecutive ex;

  // define a sequence of test items
  // - each test item holds a std::shared_ptr<ITest> to a test instance created on native heap
  //   and a string test name

  TestExecutive::TestItem ti1;
  ti1.pTest = std::shared_ptr<ITest>(new TestPasses());
  ti1.testName = "test_always_passes";

  TestExecutive::TestItem ti2 = { std::shared_ptr<ITest>(new TestFails), "test_always_fails" };
  TestExecutive::TestItem ti3 = { std::shared_ptr<ITest>(new TestThrows), "test_always_throws" };
  TestExecutive::TestItem ti4 = { std::shared_ptr<ITest>(new TestCode("#1")), "testing CodeToTest" };
  TestExecutive::TestItem ti5 = { std::shared_ptr<ITest>(new TestCode("#2")), "testing CodeToTest" };

  // register test sequence for execution
  // - each registration makes a 

  ex.registerTest(ti1);
  ex.registerTest(ti2);
  ex.registerTest(ti3);
  ex.registerTest(ti4);
  ex.registerTest(ti5);

  // execute all registered tests and report status

  bool result = ex.doTests();

  if (result == true)
    std::cout << "\n  all tests passed";
  else
    std::cout << "\n  at least one test failed";

  std::cout << "\n";
  return 0;
}
#endif
