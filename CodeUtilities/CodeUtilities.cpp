///////////////////////////////////////////////////////////////////////
// CodeUtilities.cpp - small, generally useful, helper classes       //
// ver 1.3                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include "CodeUtilities.h"
#include "../StringUtilities/StringUtilities.h"
#include <string>
#include <iostream>
#include <functional>

#ifdef TEST_CODEUTILITIES

using namespace Utilities;

int main(int argc, char* argv[])
{
  Title("Testing CodeUtilities");
  putline();

  title("Testing ProcessCmdLine");
  ProcessCmdLine pcl(argc, argv);
  pcl.usage();

  preface("Command Line: "); pcl.showCmdLine(argc, argv);
  preface("path:     "); pcl.showPath();
  preface("Options:  "); pcl.showOptions();
  preface("Patterns: "); pcl.showPatterns();
  preface("MaxItems: "); pcl.showMaxItems();
  std::cout << "\n";

  preface("changing path: ");
  pcl.path("c:\\su");
  pcl.showPath();

  preface("adding option: ");
  pcl.option('/q');
  pcl.showOptions();

  preface("adding pattern: ");
  pcl.pattern("*.htm");
  pcl.showPatterns();

  preface("changing maxItems: ");
  pcl.maxItems(15);
  pcl.showMaxItems();
  putline();
  preface("new command line: ");
  pcl.showCmdLine();
  putline();

  title("Testing Box and PersistFactory");
  Box<double> boxedDouble = 3.1415927;
  std::cout << "\n  boxedDouble = " << boxedDouble;
  boxedDouble = 0.33333;
  std::cout << "\n  boxedDouble = " << boxedDouble;

  PersistFactory<std::string> name = std::string("Jim");
  name.append(" Fawcett");
  std::cout << "\n  " << name.toXml("name");

  PersistFactory<Box<double>> value = Box<double>(9.87654);
  std::cout << "\n  " << value.toXml("value");
  value += 3.14159;
  std::cout << "\n  " << value.toXml("value");
  std::cout << "\n\n";

  title("test std::string Converter<T>::toString(T)");

  std::string conv1 = Converter<double>::toString(3.1415927);
  std::string conv2 = Converter<int>::toString(73);
  std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

  std::cout << "\n  Converting from values to strings: ";
  std::cout << conv1 << ", " << conv2 << ", " << conv3;
  putline();

  title("test T Converter<T>::toValue(std::string)");

  std::cout << "\n  Converting from strings to values: ";
  std::cout << Converter<double>::toValue(conv1) << ", ";
  std::cout << Converter<int>::toValue(conv2) << ", ";
  std::cout << Converter<std::string>::toValue(conv3);

  putline(2);
  return 0;
}
#endif
