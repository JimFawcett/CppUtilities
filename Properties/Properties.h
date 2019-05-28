#pragma once
/////////////////////////////////////////////////////////////////////
// Properties.h - provides getter and setter methods               //
// ver 1.1                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018         //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides a single Property<T> class that provides
*  getter and setter methods and backing store T t_ using the property 
*  name.
*  - Here's an example:
*      Property<int> intProp;
*      intProp(42)    Sets its internal store to 42
*      intProp()      returns the internal store value
*  - So one declaration creates two methods with simple syntax, using
*    the property's instance name. 
*  - The getter and setter methods are virtual, so you can override
*    them to provide logic that constraints getting and setting.
*  - Property also provides the method ref() which returns a reference
*    to the underlying stored value.  That lets us call member functions
*    directly on the stored type.  However, that lets users bypass
*    setter logic, so use it with caution.
*
*  Required Files:
*  ---------------
*  Properties.h
*
*  Maintenance History:
*  --------------------
*  ver 1.1 : 30 Jan 2019
*  - added ref()
*  ver 1.0 : 11 Oct 2018
*  - first release
*/
namespace Utilities
{
  template <typename T>
  class Property
  {
  public:
    virtual ~Property() {}

    //----< setter assigns the state of t to the internal store >----

    virtual void operator()(const T& t)
    {
      t_ = t;
    }
    //----< getter returns the value of the internal store >---------
    /*
    *  - Return by value is used so that clients cannot change the
    *    internal store without using setter.
    */
    virtual T operator()()
    {
      return t_;
    }
    //----< getter that returns reference >--------------------------
    /*
    *  - Allows us to call member function on stored type
    *  - Note that this allows a user to bypass any logic
    *    in setter.
    */
    virtual T& ref()
    {
      return t_;
    }
  private:
    T t_;
  };
}
