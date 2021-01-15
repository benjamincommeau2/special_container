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
#include <sstream>
#include <iomanip>

#ifndef MATRIX_HPP
#define MATRIX_HPP
class Matrix {
 public:
  typedef uint32_t Index;
  /* //////////////////////////////////////////////////////////////
  Key
  */ //////////////////////////////////////////////////////////////
  struct K {
    Index x_; Index y_;
    K() {}
    K(const Index& x, const Index& y) { x_ = x; y_ = y; } 
    std::string to_string() {
      return "("+std::to_string(x_)+","+std::to_string(y_)+")";
    }
    bool operator <(const K& k) const {
      return x_ != k.x_ ? x_ < k.x_ : y_ < k.y_;
    }
  };
  /* //////////////////////////////////////////////////////////////
  Value
  */ //////////////////////////////////////////////////////////////

  typedef std::complex<double> Value;
  struct V {
    Value v_;
    V() {}
    V(const Value& v) {v_ = v;}
    std::string to_string() {
      std::stringstream stream;
      stream << std::fixed << std::setprecision(2);
      stream << "(" << (v_.real()) << ","
        << (v_.imag()) << ")";
      return stream.str();
    }
  };

  /* //////////////////////////////////////////////////////////////
  Methods
  */ //////////////////////////////////////////////////////////////
  void add(Index x, Index y, Value v);
  void transpose_emplace();


  Map<K,V> map_;
 private:
  Index index_max_ = UINT32_MAX;
};

/* //////////////////////////////////////////////////////////////
Explicit Methods
*/ //////////////////////////////////////////////////////////////

void
Matrix::
add(Index x, Index y, Value v) {
  auto itm = map_.map_find(K(x,y));
  if(itm != map_.map_end() && itm->clr() == map_.getClr()) {
    itm->val().v_ += v;
  } else {
    map_.try_emplace(K(x,y),V(v));
  }
}

void 
Matrix::
transpose_emplace() {
  if(map_.getClr() == map_.getClrMax()) {map_.flatten_clear();}
  auto old_clr = map_.getClr();
  map_.setClr(old_clr+1);
  auto itl = map_.list_begin();
  auto itm0 = map_.map_begin();
  auto itm1 = map_.map_begin();
  Index x; Index y;
  while(itl != map_.list_end() && itl->clr() == old_clr) {
    x = itl->key().x_; y = itl->key().y_;
    itm0 = map_.map_find(K(x,y));
    itm1 = map_.map_find(K(y,x));
    if(itm1 != map_.map_end() && x != y) {
      // std::cout << "transposed key exists" << std::endl;
      map_.reInsertKey(itm0, K(y,x), itm1, K(x,y));
      map_.move2Front(itm1);
    } else {
      // std::cout << "transposed key does not exist" << std::endl;
      map_.reInsertKey(itm0, K(y,x));
    }
    itl++;
  }
}

#endif
