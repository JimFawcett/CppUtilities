/////////////////////////////////////////////////////////////////////
// DirExplorerE.cpp - directory explorer using events              //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Note:
*  This file simulates application code that uses DirExplorerE.
*/

#ifdef TEST_DIREXPLORERE  // only compile the following when defined

#include "DirExplorerE.h"
#include "../StringUtilities/StringUtilities.h"
#include "../CodeUtilities/CodeUtilities.h"
#include "../UtilitiesEnvironment/Environment.h"

using namespace Utilities;
using namespace FileSystem;

/////////////////////////////////////////////////////////////////////
// dirEventHandler class defines dir processing without modifying
// DirExplorerE code.

class dirEventHandler : public IDirEvent
{
public:
  void execute(const std::string& dirname)
  {
    std::cout << "\n  dir--->  " << dirname;
  }
};

/////////////////////////////////////////////////////////////////////
// fileEventHandler class defines file processing without modifying
// DirExplorerE code.

class fileEventHandler : public IFileEvent
{
public:
  void execute(const std::string& filename)
  {
    std::cout << "\n  file-->    " << filename;
  }
};

class AppDirExplorerE : public DirExplorerE
{
public:
  AppDirExplorerE(const std::string& path) : DirExplorerE(path) {}

  virtual bool done(bool reset = false)
  {
    static bool first = true;
  
    if (first || reset)
    {
      std::cout << "\n  Application modified done() invoked";
      first = false;
    }
    return DirExplorerE::done();
  }
};

std::string customUsage()
{
  std::string usage;
  usage += "\n  Command Line: path [/option]* [/pattern]*";
  usage += "\n    path is relative or absolute path where processing begins";
  usage += "\n    [/option]* are one or more options of the form:";
  usage += "\n      /s - walk directory recursively";
  usage += "\n      /h - hide empty directories";
  usage += "\n      /a - on stopping, show all files in current directory";
  usage += "\n    [pattern]* are one or more pattern strings of the form:";
  usage += "\n      *.h *.cpp *.cs *.txt or *.*";
  usage += "\n";
  return usage;
}

int main(int argc, char *argv[])
{
  Title("Demonstrate DirExplorer-Events, " + DirExplorerE::version());

  ProcessCmdLine pcl(argc, argv);
  customUsage();

  preface("Command Line: ");
  pcl.showCmdLine();
  putline();

  if (pcl.parseError())
  {
    pcl.usage();
    std::cout << "\n\n";
    return 1;
  }

  AppDirExplorerE de(pcl.path());

  //----< subscribe for dir and file events >------------------------

  de.dirSubScribe(new dirEventHandler);
  de.fileSubScribe(new fileEventHandler);

  for (auto patt : pcl.patterns())
  {
    de.addPattern(patt);
  }

  if (pcl.hasOption('s'))
  {
    de.recurse();
  }

  if (pcl.hasOption('a'))
  {
    de.showAllInCurrDir(true);
  }

  if (pcl.hasOption('h'))
  {
    de.hideEmptyDirectories(true);
  }

  if (pcl.maxItems() > 0)
  {
    de.maxItems(pcl.maxItems());
  }

  //----< start file system processing >-----------------------------

  de.search();
  de.showStats();

  std::cout << "\n\n";
  return 0;
}

#endif