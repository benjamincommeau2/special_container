/*
  Copyright Benjamin Commeau

Use CamelCase for all names. Start types (such as classes, structs, and typedefs) with a capital letter, other names (functions, variables) with a lowercase letter. You may use an all-lowercase name with underscores if your class closely resembles an external construct (e.g., a standard library construct) named that way.

(1) C++ interfaces are named with a Interface suffix, and abstract base classes with an Abstract prefix.
(2) Member variables are named with a trailing underscore.
(3) Accessors for a variable foo_ are named foo() and setFoo().
(4) Global variables are named with a g_ prefix.
(5) Static class variables are named with a s_ prefix.
(6) Global constants are often named with a c_ prefix.
(7) If the main responsibility of a file is to implement a particular class, then the name of the file should match that class, except for possible abbreviations to avoid repetition in file names (e.g., if all classes within a module start with the module name, omitting or abbreviating the module name is OK). Currently, all source file names are lowercase, but this casing difference should be the only difference.

The rationale for the trailing underscore and the global/static prefixes is that it is immediately clear whether a variable referenced in a method is local to the function or has wider scope, improving the readability of the code.
*/

#include <algorithm>
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <list>
#include <cstdio>
#include <complex>
#include <exception>
#include <memory>
#include <chrono>
#include <cmath>
#include <string>
#include <random>
#include "Map.hpp"

#ifndef MATRIX_HPP
#define MATRIX_HPP
template<class Key, class Val, class Compare = std::less<Key>>
class Matrix {
 public:
  typedef uint32_t Index;
  typedef std::complex<double> Value;
  /* //////////////////////////////////////////////////////////////
  List of T's
  */ //////////////////////////////////////////////////////////////


  /* //////////////////////////////////////////////////////////////
  Methods
  */ //////////////////////////////////////////////////////////////


 private:
 Map map_();
};

/* //////////////////////////////////////////////////////////////
Explicit Methods
*/ //////////////////////////////////////////////////////////////


#endif
