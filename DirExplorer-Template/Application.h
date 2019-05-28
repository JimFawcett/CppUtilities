#pragma once
///////////////////////////////////////////////////////////////////////
// Application.h - provides demonstration methods doFile and doDir   //
// ver 1.1                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package represents an application that needs to navigate directories
*  as part of its application.  It uses DirExploreT, with no modification
*  for the application's needs.
*
*  Required Files:
*  ---------------
*  Application.h, application.cpp
*  DirExplorerT.h, DirExplorerT.cpp  // DirExplorerT.cpp just for testing
*  FileSystem.h, ,FileSystem.cpp
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 19 Aug 2018
*  - Converted to inline methods declared below class declaration.
*  - Added all the applications processing to main.
*  ver 1.0 : 11 Aug 2018
*  - first release
*/
#include <iostream>
#include <string>

class Application
{
public:
  Application();
  
  // Application defines how to handle files and dirs, when to
  // quit, and how to display final results.
  // None of this requires alteration of DirExplorerT's code.

  void doFile(const std::string& filename);
  void doDir(const std::string& dirname);
  size_t fileCount();
  size_t dirCount();
  bool done();
  void showStats();

  // configure application options

  void showAllInCurrDir(bool showAllFilesInCurrDir);
  bool showAllInCurrDir();
  void maxItems(size_t maxItems);
  
private:
  size_t fileCount_ = 0;  // number of files processed
  size_t dirCount_ = 0;   // number of directories processed
  size_t maxItems_ = 0;   // upper bound on number of files to process
  bool showAll_ = false;  // if true, show empty directories
};

inline Application::Application()
{
  std::cout << "\n  Using Application methods doFile and doDir\n";
}
inline void Application::doFile(const std::string& filename)
{
  ++fileCount_;
  if(showAll_ || !done())
  {
    std::cout << "\n  file-->    " << filename;
  }
}
inline void Application::doDir(const std::string& dirname)
{
  ++dirCount_;
  std::cout << "\n  dir--->  " << dirname;
}
inline size_t Application::fileCount()
{
  return fileCount_;
}
inline size_t Application::dirCount()
{
  return dirCount_;
}
inline void Application::showAllInCurrDir(bool showAllFilesInCurrDir)
{
  showAll_ = showAllFilesInCurrDir;
}
inline bool Application::showAllInCurrDir()
{
  return showAll_;
}
inline void Application::maxItems(size_t maxItems)
{
  maxItems_ = maxItems;
}
//----< show final counts for files and dirs >---------------------

inline void Application::showStats()
{
  std::cout << "\n\n  processed " << fileCount_ << " files in " << dirCount_ << " directories";
  if(done())
  {
    std::cout << "\n  stopped because max number of files exceeded";
  }
}
inline bool Application::done()
{
  return (0 < maxItems_ && maxItems_ < fileCount_);
}
