#pragma once
///////////////////////////////////////////////////////////////////////
// Environment.h - Provides aliases used in several of the utilities //
// ver 1.0                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018           //
///////////////////////////////////////////////////////////////////////
/*
*  Package Purpose:
*  ----------------
*  Provide one place for common defintions.
*
*  Required Files:
*  ---------------
*  Environment.h
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 11 Oct 2018
*  - first release
*/

#include <string>
#include <vector>

namespace Utilities
{
  using MessageStr = std::string;
  using PathStr = std::string;
  using File = std::string;
  using Files = std::vector<File>;
  using Pattern = std::string;
  using Patterns = std::vector<Pattern>;
  using Dir = std::string;
  using Dirs = std::vector<Dir>;
}