#pragma once
/////////////////////////////////////////////////////////////////////
// DirExplorerT.h - Template directory explorer                    //
// ver 1.2                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* DirExplorerT provides a class, of the same name, that executes a
* depth first search of a directory tree rooted at a specified path.
* Each time it enters a directory, it invokes its member function
* doDir, and for every file in that directory, it invokes doFile.
*
* We call this a "Template Directory Explorer" because, it carries
* out its assigned responsibilities well, and allows the using
* application to modify what the doDir and doFiles do, by using
* a template type parameter to supply those methods.
* This is an improvement over DirExplorerN because the application
* does not need to change any part of the DirExplorerT class.
*
* Two other projects in this solution also do that, in different
* ways.  We'll be using this solution to illustrate techniques for
* building flexible software.
*
* Required Files:
* ---------------
* DirExplorerT.h, DirExplorerT.cpp
* Application.h, Application.cpp    // provides defn's for doDir and doFile
* FileSystem.h, FileSystem.cpp      // Directory and Path classes
* StringUtilities.h                 // Title function
* CodeUtilities.h                   // ProcessCmdLine class
*
* Maintenance History:
* --------------------
* ver 1.1 : 16 Aug 2018
* - made no recursion default, added option /s for recursion
* ver 1.0 : 11 Aug 2018
* - first release
*
*/
#include <vector>
#include "../FileSystem/FileSystem.h"
//#include "../Application/Application.h"

namespace FileSystem
{
  template<typename App>
  class DirExplorerT
  {
  public:
    using patterns = std::vector<std::string>;

    static std::string version() { return "ver 1.2"; }

    DirExplorerT(const std::string& path);

    void addPattern(const std::string& patt);
    void hideEmptyDirectories(bool hide);
    void maxItems(size_t numFiles);
    void showAllInCurrDir(bool showAllCurrDirFiles);
    bool showAllInCurrDir();
    void recurse(bool doRecurse = true);
    
    void search();
    void find(const std::string& path);
    bool done();

    void showStats();
    size_t fileCount();
    size_t dirCount();

  private:
    Application app_;
    std::string path_;
    patterns patterns_;
    bool hideEmptyDir_ = false;
    bool showAll_ = false;      // show files in current dir even if maxItems_ has been exceeded
    size_t maxItems_ = 0;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
  };

  //----< construct DirExplorerN instance with default pattern >-----

  template<typename App>
  DirExplorerT<App>::DirExplorerT(const std::string& path) : path_(path)
  {
    patterns_.push_back("*.*");
  }
  //----< add specified patterns for selecting file names >----------

  template<typename App>
  void DirExplorerT<App>::addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  //----< set option to hide empty directories >---------------------

  template<typename App>
  void DirExplorerT<App>::hideEmptyDirectories(bool hide)
  {
    hideEmptyDir_ = hide;
  }
  //----< set the maximum number of files to display >---------------

  template<typename App>
  void DirExplorerT<App>::maxItems(size_t numFiles)
  {
    maxItems_ = numFiles;
    app_.maxItems(maxItems_);
  }
  //----< stop display before showing all files in dir >-------------

  template<typename App>
  void DirExplorerT<App>::showAllInCurrDir(bool showAllCurrDirFiles)
  {
    showAll_ = showAllCurrDirFiles;
  }
  //----< stop display before showing all files in dir? >------------

  template<typename App>
  bool DirExplorerT<App>::showAllInCurrDir()
  {
    return showAll_;
  }
  //----< set option to recusively walk dir tree >-------------------

  template<typename App>
  void DirExplorerT<App>::recurse(bool doRecurse)
  {
    recurse_ = doRecurse;
  }
  //----< start Depth First Search at path held in path_ >-----------

  template<typename App>
  void DirExplorerT<App>::search()
  {
    if (showAllInCurrDir())
      app_.showAllInCurrDir(true);

    find(path_);
  }
  //----< search for directories and their files >-------------------
  /*
    Recursively finds all the dirs and files on the specified path,
    executing doDir when entering a directory and doFile when finding a file
  */
  template<typename App>
  void DirExplorerT<App>::find(const std::string& path)
  {
    if (done())  // stop searching
      return;

    bool hasFiles = false;
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    if (!hideEmptyDir_)
      app_.doDir(fpath);

    for (auto patt : patterns_)
    {
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      if (!hasFiles && hideEmptyDir_)
      {
        if (files.size() > 0)
        {
          app_.doDir(fpath);
          hasFiles = true;
        }
      }
      for (auto f : files)
      {
        app_.doFile(f);
      }
    }

    if (done())  // stop recursion
      return;

    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
  
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;
      if (recurse_)
      {
        find(dpath);
      }
      else
      {
        app_.doDir(dpath);
      }
    }
  }
  //----< return number of files processed >-------------------------

  template<typename App>
  size_t DirExplorerT<App>::fileCount()
  {
    return App.fileCount();
  }
  //----< return number of directories processed >-------------------

  template<typename App>
  size_t DirExplorerT<App>::dirCount()
  {
    return App.dirCount();
  }
  //----< show final counts for files and dirs >---------------------

  template<typename App>
  void DirExplorerT<App>::showStats()
  {
    app_.showStats();
  }

  template<typename App>
  bool DirExplorerT<App>::done()
  {
    return app_.done();
  }
}