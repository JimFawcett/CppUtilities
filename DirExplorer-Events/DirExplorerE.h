#pragma once
/////////////////////////////////////////////////////////////////////
// DirExplorerE.h - directory explorer uses events                 //
// ver 1.2                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* DirExplorerE provides a class, of the same name, that executes a
* depth first search of a directory tree rooted at a specified path.
* Each time it enters a directory, it invokes its member function
* notifyDir, and for every file in that directory, it invokes notifyFile.
* 
*
* We call this "Event Directory Explorer" because, when it carries
* out its assigned responsibilities, it uses event interfaces,
* IDirEvent and IFileEvent, to allow applications to decide what
* processing should occur when the explorer encounters a directory
* or file.  The application simply defines classes, derived from 
* the event Interfaces, to supply that processing.  They then
* create instances of those classes on the native heap, and pass
* the resulting pointers to explorer's dirSubcribe and fileSubscribe
* methods.
* 
* Another project in this solution does just that, in a different way.

* - DirExplorer-Template:
*     Applications provide template class parameters to define file
*     and directory processing.
*
* Required Files:
* ---------------
* DirExplorerE.h, DirExplorerE.cpp
* FileSystem.h, FileSystem.cpp      // Directory and Path classes
* StringUtilities.h                 // Title function
* CodeUtilities.h                   // ProcessCmdLine class
*
* Maintenance History:
* --------------------
* ver 1.2 : 19 Aug 2018
* - moved method definitions to inlines below class declaration
* - made done a virtual method and added a virtual destructor
* ver 1.1 : 16 Aug 2018
* - made no recursion default, added option /s for recursion
* ver 1.0 : 13 Aug 2018
* - first release
*
*/
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "../FileSystem/FileSystem.h"
#include "../CodeUtilities/CodeUtilities.h"

namespace FileSystem
{
  ///////////////////////////////////////////////////////////////////
  // IFileEvent and IDirEvent interfaces
  // - hooks for application event handlers to implement

  struct IDirEvent
  {
    virtual void execute(const std::string& dirname) = 0;
    virtual ~IDirEvent() {}
  };

  struct IFileEvent
  {
    virtual void execute(const std::string& filename) = 0;
    virtual ~IFileEvent() {}
  };

  ///////////////////////////////////////////////////////////////////
  // DirExplorerE class
  // - defers application specific processing to application's
  //   event handlers

  class DirExplorerE
  {
  public:
    using patterns = std::vector<std::string>;

    static std::string version() { return "ver1.2"; }

    DirExplorerE(const std::string& path);
    virtual ~DirExplorerE() {}

    // provide hooks for handling events

    void dirSubScribe(IDirEvent* pDirSub);
    void notifyDir(const std::string& dirname);
    void fileSubScribe(IFileEvent* pFileSub);
    void notifyFile(const std::string& filename);
    
    // configure application processing

    void addPattern(const std::string& patt);
    void hideEmptyDirectories(bool hide);
    void maxItems(size_t numFiles);
    void showAllInCurrDir(bool showAllCurrDirFiles=true);
    bool showAllInCurrDir();
    void recurse(bool doRecurse = true);
    
    // navigation

    void search();
    virtual void find(const std::string& path);
    virtual bool done(bool reset = false);

    // display results
    virtual void showStats();

  private:
    std::vector<std::shared_ptr<IDirEvent>> dirSubscribers_;
    std::vector<std::shared_ptr<IFileEvent>> fileSubscribers_;
    std::string path_;
    patterns patterns_;
    bool hideEmptyDir_ = false;
    bool showAll_ = false;
    size_t maxItems_ = 0;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
  };

  //----< construct DirExplorerN instance with default pattern >-----

  inline DirExplorerE::DirExplorerE(const std::string& path) : path_(path)
  {
    patterns_.push_back("*.*");
  }
  //----< subscribe for dir events >---------------------------------
  /*
  *  pDirSub must be a pointer to DirSub created on heap
  */
  inline void DirExplorerE::dirSubScribe(IDirEvent* pDirSub)
  {
    dirSubscribers_.push_back(std::shared_ptr<IDirEvent>(pDirSub));
  }
  //----< notify dir event subscribers >------------------------------

  inline void DirExplorerE::notifyDir(const std::string& dirname)
  {
    ++dirCount_;
    for (auto sub : dirSubscribers_)
      sub->execute(dirname);
  }
  //----< subscribe for file events >---------------------------------
  /*
  *  pFileSub must be a pointer to FileSub created on heap
  */
  inline void DirExplorerE::fileSubScribe(IFileEvent* pFileSub)
  {
    fileSubscribers_.push_back(std::shared_ptr<IFileEvent>(pFileSub));
  }
  //----< notify file event subscribers >-----------------------------

  inline void DirExplorerE::notifyFile(const std::string& filename)
  {
    ++fileCount_;
    for (auto sub : fileSubscribers_)
    {
      if (showAll_ || maxItems_ == 0 || fileCount_ <= maxItems_)
      {
        sub->execute(filename);
      }
    }
  }
  //----< add specified patterns for selecting file names >----------

  inline void DirExplorerE::addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }
  //----< set option to hide empty directories >---------------------

  inline void DirExplorerE::hideEmptyDirectories(bool hide)
  {
    hideEmptyDir_ = hide;
  }
  //----< set the maximum number of files to display >---------------

  inline void DirExplorerE::maxItems(size_t numFiles)
  {
    maxItems_ = numFiles;
  }
  //----< stop display before showing all files in dir >-------------

  inline void DirExplorerE::showAllInCurrDir(bool showAllCurrDirFiles)
  {
    showAll_ = showAllCurrDirFiles;
  }
  //----< stop display before showing all files in dir? >------------

  inline bool DirExplorerE::showAllInCurrDir()
  {
    return showAll_;
  }
  //----< set option to recusively walk dir tree >-------------------

  inline void DirExplorerE::recurse(bool doRecurse)
  {
    recurse_ = doRecurse;
  }
  //----< start Depth First Search at path held in path_ >-----------

  inline void DirExplorerE::search()
  {
    find(path_);
  }
  //----< search for directories and their files >-------------------
  /*
    Recursively finds all the dirs and files on the specified path,
    executing notifyDir when entering a directory and notifyFile
    when finding a file.
  */
  inline void DirExplorerE::find(const std::string& path)
  {
    if (done())  // stop searching
      return;

    bool hasFiles = false;
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    if (!hideEmptyDir_)
    {
      notifyDir(fpath);
    }
    for (auto patt : patterns_)
    {
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      if (!hasFiles && hideEmptyDir_)
      {
        if (files.size() > 0)
        {
          notifyDir(fpath);
          hasFiles = true;
        }
      }
      for (auto f : files)
      {
        notifyFile(f);
      }
    }

    if (done())
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
        notifyDir(dpath);
      }
    }
  }
  //----< show final counts for files and dirs >---------------------

  inline void DirExplorerE::showStats()
  {
    std::cout << "\n\n  processed " << fileCount_ << " files in " << dirCount_ << " directories";
    if (done(true))
    {
      std::cout << "\n  stopped because max number of files exceeded";
    }
  }
  //----< define conditions for stopping navigation >----------------

  inline bool DirExplorerE::done(bool reset)
  {
    return (0 < maxItems_ && maxItems_ < fileCount_);
  }
}