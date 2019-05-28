///////////////////////////////////////////////////////////////////////
// StringUtilities.cpp - small, generally useful, helper classes     //
// ver 1.1                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include "StringUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"

#ifdef TEST_STRINGUTILITIES

using namespace Utilities;

int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test split(std::string, ',')");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;

  std::vector<std::string> result = split(test);
  showSplits(result);

  title("test split(std::string, ' ')");

  std::cout << "\n  test string = " << test;

  result = split(test, ' ');
  showSplits(result);

  putline(2);
  return 0;
}
#endif
