#pragma once
///////////////////////////////////////////////////////////////////////
// CodeUtilities.h - small, generally useful, helper classes         //
// ver 1.3                                                           //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes:
* - ProcessCmdLine    extracts path, options, patterns, and a number from command line
* - Converter<T>      converts T to and from strings
* - Box<T>            converts primitive type to instance of a class
* - PersistFactory<T> adds toXml() method to T
*
* Build Process:
* --------------
* Required Files: 
*   CodeUtilities.h
*
* Maintenance History:
* --------------------
* ver 1.3 : 24 Jun 2019
* - modified options processing - now have:
*     default path = "."
*     default pattern = "*.*"
* - fixed test stub output
* ver 1.2 : 22 Jun 2019
* - redesigned handling of commandline args in ProcessCmdLine
* - now expects path to be preceded by /P
* - now expects pattern to be preceeded by /p and accepts *.h,*.cpp,.. with no spaces
* - now expects number to be preceeded by /n
* ver 1.1 : 10 Aug 2018
* - added ProcessCmdLine class
* ver 1.0 : 12 Jan 2018
* - first release
* - refactored from earlier Utilities.h
*
* Notes:
* ------
* - Designed to provide all functionality in header file. 
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "..//StringUtilities/StringUtilities.h"

namespace Utilities
{
  /////////////////////////////////////////////////////////////////////
  // preface function
  // - used to add a string preface to an output, e.g., "\n  "

  inline void preface(const std::string& msg = "", bool doReturn = true, std::ostream& out = std::cout, const std::string& prefix = "  ")
  {
    if (doReturn) out << "\n" << prefix << msg;
  }

  /////////////////////////////////////////////////////////////////////
  // ProcessCmdLine class
  // - extracts path, patterns, options, number

  class ProcessCmdLine
  {
  public:
    using Path = std::string;
    using Option = char;
    using Options = std::vector<Option>;
    using Pattern = std::string;
    using Patterns = std::vector<Pattern>;
    using Regex = std::string;
    using Number = long int;

    ProcessCmdLine(int argc, char** argv, std::ostream& out = std::cout);
    ProcessCmdLine() : pOut_(&std::cout) {};
    void processCmdLine(int argc, char** arv, std::ostream& out = std::cout);
    bool parseError();
    Path path();
    void path(const Path& path);
    Options options();
    void option(Option op);
    bool hasOption(Option op);
    Patterns patterns();
    void pattern(const Pattern& patt);
    Number maxItems();
    void maxItems(Number number);
    Regex regex();
    void regex(const Regex& rx);
    void usage(const std::string& msg = "");
    void showCmdLine(int argc, char** argv, bool showFirst = false);
    void showCmdLine();
    void showPath();
    void showOptions();
    void showPatterns();
    void showMaxItems();
    void showRegex();
    void setUsageMessage(const std::string& msg);
  private:
    void defaultUsageMessage();
    Path path_;
    Patterns patterns_;
    Options options_;
    int maxItems_ = 0;
    Regex regex_;
    bool parseError_ = false;
    std::ostream* pOut_;
    std::ostringstream msg_;
  };

  /*----< path operations >------------------------------------------*/

  inline void ProcessCmdLine::path(const Path& path) 
  { 
    path_ = path; 
  }

  inline ProcessCmdLine::Path ProcessCmdLine::path()
  {
    if (path_ == "")
      path_ = ".";
    return path_;
  }

  inline void ProcessCmdLine::showPath()
  {
    *pOut_ << path_ << " ";
  }

  /*----< options operations >---------------------------------------*/

  inline void ProcessCmdLine::option(Option option)
  {
    options_.push_back(option);
  }

  inline ProcessCmdLine::Options ProcessCmdLine::options()
  { 
    if (options_.size() == 0)
      options_.push_back('f');
    return options_; 
  }

  inline bool ProcessCmdLine::hasOption(Option opt)
  {
    for (auto op : options_)
      if (op == opt)
        return true;
    return false;
  }

  inline void ProcessCmdLine::showOptions()
  {
    for (auto opt : options_)
    {
      *pOut_ << opt << " ";
    }
  }

  /*----< patterns operations >--------------------------------------*/

  inline void ProcessCmdLine::pattern(const Pattern& pattern)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.erase(patterns_.begin());
    patterns_.push_back(pattern);
  }

  inline ProcessCmdLine::Patterns ProcessCmdLine::patterns()
  {
    if (patterns_.size() == 0)
      patterns_.push_back("*.*");
    return patterns_;
  }

  inline void ProcessCmdLine::showPatterns()
  {
    for (auto patt : patterns_)
    {
      *pOut_ << patt << " ";
    }
  }

  /*----< maxItems operations >--------------------------------------*/

  inline void ProcessCmdLine::maxItems(Number maxItems)
  {
    maxItems_ = maxItems;
  }

  inline ProcessCmdLine::Number ProcessCmdLine::maxItems()
  {
    return maxItems_;
  }

  inline void ProcessCmdLine::showMaxItems()
  {
    *pOut_ << maxItems_ << " ";
  }

  /*----< regex operations >-----------------------------------------*/

  inline void ProcessCmdLine::regex(const Regex& rx)
  {
    regex_ = rx;
  }

  inline ProcessCmdLine::Regex ProcessCmdLine::regex()
  {
    if (regex_ == "")
      regex_ = ".*";
    return regex_;
  }

  inline void ProcessCmdLine::showRegex()
  {
    *pOut_ << regex_ << " ";
  }

  /*----< parseError operation >-------------------------------------*/

  inline bool ProcessCmdLine::parseError()
  {
    return parseError_;
  }

  /*----< command line operations >----------------------------------*/

  inline void ProcessCmdLine::processCmdLine(int argc, char** argv, std::ostream& out)
  {
    pOut_ = &out;

    char lastOption = '\0';

    if (msg_.str() == "")
      defaultUsageMessage();

    for (int i = 1; i < argc; ++i)
    {
      if (argv[i][0] == '/')
      {
        lastOption = argv[i][1];
        if(lastOption != 'P' && lastOption != 'p' && lastOption != 'n' && lastOption != 'R')
          options_.push_back(lastOption);
      }
      else
      {
        Patterns splits;
        switch (lastOption)
        {
        case 'P':
          path_ = argv[i];
          break;
        case 'p':
          splits = split(std::string(argv[i]), ',');
          for (auto patt : splits)
            patterns_.push_back(patt);
          break;
        case 'n':
          maxItems_ = atoi(argv[i]);
          break;
        case 'R':
          regex_ = argv[i];
          break;
        case 'h':
          usage();
          break;
        default:
          break;
        }
      }
    }
    // default path and patterns

    if (path_ == "")
      path_ = ".";
    if (patterns_.size() == 0)
      patterns_.push_back("*.*");
  }

  inline ProcessCmdLine::ProcessCmdLine(int argc, char** argv, std::ostream& out)
  {
    processCmdLine(argc, argv, out);
  }

  inline void ProcessCmdLine::showCmdLine(int argc, char** argv, bool showFirst)
  {
    *pOut_ << "\n\n  commandline args: ";
    
    if (showFirst)
      * pOut_ << "\n  " << argv[0] << " ";
    else
      *pOut_ << "\n  ";

    for (int i = 1; i < argc; ++i)
    {
      *pOut_ << argv[i] << " ";
    }
  }

  inline void ProcessCmdLine::showCmdLine()
  {
    *pOut_ << "\n  ";
    if (path() != "")
    {
      *pOut_ << "\n  Path:     ";
      showPath();
    }
    *pOut_ << "\n  options:  ";
    preface("", false);
    showOptions();
    *pOut_ << "\n  patterns: ";
    preface("", false);
    showPatterns();
    if (regex_ != "")
    {
      *pOut_ << "\n  Regex:    ";
      *pOut_ << regex_;
    }
    if (maxItems_ > 0)
    {
      *pOut_ << "\n  maxItems: ";
      preface("", false);
      showMaxItems();
    }
    *pOut_ << "\n";
  }

  inline void ProcessCmdLine::defaultUsageMessage()
  {
    msg_ << "\n  Command Line: [/opt arg]* [/opt]*";
    msg_ << "\n    /opt arg has option type - a character, and option argument, a literal string";
    msg_ << "\n    Examples:";
    msg_ << "\n      /P \"../..\"             // starting path";
    msg_ << "\n      /p \"*.h,*.cpp,*.cs\"    // file patterns - no spaces";
    msg_ << "\n      /n \"42\"                // max items";
    msg_ << "\n      /R \"threads|sockets\"   // regular expression";
    msg_ << "\n    /option has option type with no argument";
    msg_ << "\n    Examples:";
    msg_ << "\n      /s                     // recurse";
    msg_ << "\n      /f                     // process files";
    msg_ << "\n      /d                     // process directories";
    msg_ << "\n";
  }

  inline void ProcessCmdLine::setUsageMessage(const std::string& msg)
  {
    msg_ << msg;
  }

  inline void ProcessCmdLine::usage(const std::string& msg)
  {
    if (msg == "")
      *pOut_ << msg_.str();
    else
      msg_.str(msg);
  }

  /////////////////////////////////////////////////////////////////////
  // Converter class
  // - supports converting unspecified types to and from strings
  // - a type is convertible if it provides insertion and extraction
  //   operators

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };
  //----< convert t to a string >--------------------------------------

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }
  //----< convert a string to an instance of T >-----------------------
  /*
  *  - the string must have been generated by Converter<T>::toString(const T& t)
  *  - T::operator>> must be the inverse of T::operator<< 
  */
  template<typename T>
  T Converter<T>::toValue(const std::string& src)
  {
    std::istringstream in(src);
    T t;
    in >> t;
    return t;
  }

  ///////////////////////////////////////////////////////////////////////
  // Box class
  // - wraps primitive type in class
  // - preserves primitive syntax

  template<typename T>
  class Box
  {
  public:
    Box() : primitive_(T()) {}
    Box(const T& t) : primitive_(t) {}
    operator T&() { return primitive_; }
    T& operator=(const T& t) { primitive_ = t; return primitive_; }
  private:
    T primitive_;
  };

  ///////////////////////////////////////////////////////////////////////
  // ToXml interface
  // - defines language for creating XML elements

  struct ToXml
  {
    virtual std::string toXml(const std::string& tag) = 0;
    virtual ~ToXml() {};
  };

  ///////////////////////////////////////////////////////////////////////
  // PersistFactory<T> class
  // - wraps an instance of user-defined type
  // - preserves semantics of user-defined type
  // - adds toXml("tag") method

  template<typename T>
  class PersistFactory : public T, ToXml
  {
  public:
    PersistFactory() = default;
    PersistFactory(const T& t)
    {
      T::operator=(t);
    }
    std::string toXml(const std::string& tag)
    {
      std::ostringstream out;
      out << "<" << tag << ">" << *this << "</" << tag << ">";
      return out.str();
    }
  };
}