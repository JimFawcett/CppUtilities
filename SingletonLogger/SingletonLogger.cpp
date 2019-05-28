/////////////////////////////////////////////////////////////////////
// SingletonLogger.cpp - provides logging to multiple streams      //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////

#include "ISingletonLogger.h"
#include "SingletonLoggerFactory.h"
#include "SingletonLogger.h"
#include "../FileUtilities/FileUtilities.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef TEST_SINGLETONLOGGER

using namespace Utilities;

void testThrow()
{
  try {
    Lock<1> lck;
    lck.lock();
    throw std::exception("in testThrow");
    lck.unlock();
  }
  catch (std::exception&)
  {
    std::cout << "\n  exception caugth in testThrow()\n";
  }
}

int main()
{
  std::cout << "\n  Demonstrating Singleton Logger";
  std::cout << "\n ================================";

  //testThrow();

  using namespace CodeUtilities;

  using ILogger = ILogger<1, Lock>;
  using Logger = Logger<1, Lock>;
  using LoggerFactory = SingletonLoggerFactory<1, Lock>;

  Logger* pLogger = Logger::getInstance();
  ILogger* pTest = LoggerFactory::getInstance();
  if (pTest == pLogger)
    std::cout << "\n  Observed singleton behavior\n";
  else
    std::cout << "\n  Not behaving as singleton\n";

  // Logger has &std::cout as default stream.
  // Uncomment the statement below to remove that default.
  //pLogger->removeStream(&std::cout);

  std::string fileSpec = "..\\LogFile.txt";

  std::ofstream outfileStrm(fileSpec, std::ios::out);
  if (outfileStrm.good())
    pLogger->addStream(&outfileStrm);  // adding pointer to std::ofstream instance

  std::cout << "\n  logging to console and " << fileSpec << "\n";

  std::string head = "\n  Demonstration Log";
  pLogger->writeHead(head);
  pLogger->write("  Hi from main");
  pLogger->write("  hi again");
  pLogger->writeTail();

  outfileStrm.close();
  std::string msg = "displaying contents of " + fileSpec;

  // Demonstrates writing file contents to std::ostringstream.
  // If you remove out, will display directly to console inside
  // function.

  std::ostringstream out;
  Utilities::displayFileContents(msg, fileSpec, out);
  std::cout << out.str();
  std::cout << "\n";

  pLogger->addStream(&out);
  std::cout << "\n  attempting to remove std::ostringstream out";
  if (pLogger->usingStream(&out))
    std::cout << "\n  Logger has std::ostringstream out";
  else
    std::cout << "\n  Logger does not have std::ostringstream out";
  std::cout << "\n  removing std::ostringstream out";
  pLogger->removeStream(&out);
  std::cout << "\n  attempting to remove std::ostringstream out";
  if (pLogger->usingStream(&out))
    std::cout << "\n  Logger has std::ostringstream out";
  else
    std::cout << "\n  Logger does not have std::ostringstream out";

  std::cout << "\n\n";
  return 0;
}

#endif
