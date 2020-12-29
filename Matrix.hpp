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
  std::string to_string();
  void clr();
  Map<K,V>::IterSetT find(const Index& x, const Index& y);
  Map<K,V>::IterBoolSetT
    add(const Index& x, const Index& y, const Value& v);
  void transpose();


 private:
  Map<K,V> map_;
  Map<Matrix::K,Matrix::V>::IterListT it_;
};

/* //////////////////////////////////////////////////////////////
Explicit Methods
*/ //////////////////////////////////////////////////////////////

void
Matrix::
transpose() {
  if (map_.getClr() == map_.getClrMax()) { map_.flatten_clear(); }
  auto list_it = map_.list_.begin();
  list_it++;
  auto list_begin = map_.list_.begin();
  auto set_it_0 = map_.set_.begin();
  auto set_it_1 = map_.set_.begin();
  Index x; Index y;
  while (list_it != map_.list_.end()) {
    std::cout << map_.to_string() << std::endl;
    std::cout << list_it->key_.x_ << " " << list_it->key_.y_ << std::endl;
    if (list_it->clr_ == map_.getClr()) {
      std::cout << "if non-cleared key" << std::endl;
      set_it_0 = map_.set_.find(list_it);
      x = list_it->key_.x_;
      y = list_it->key_.y_;
      list_begin->key_.x_ = y;
      list_begin->key_.y_ = x;
      set_it_1 = map_.set_.find(list_begin);
      if (set_it_1 != map_.set_.end()) {
        std::cout << "extract 2 keys" << std::endl;
        auto nh_1 = map_.set_.extract(set_it_1);
        auto nh_0 = map_.set_.extract(set_it_0);
        // modify
        nh_1.value()->key_.x_ = x;
        nh_1.value()->key_.y_ = y;
        nh_1.value()->clr_ = map_.getClr()+1;
        nh_0.value()->key_.x_ = y;
        nh_0.value()->key_.y_ = x;
        nh_0.value()->clr_ = map_.getClr()+1;
        std::cout << "re-insert" << std::endl;
        map_.set_.insert(std::move(nh_0));
        map_.set_.insert(std::move(nh_1));
        std::cout << "move to front" << std::endl;
        map_.moveToFront(list_it);
        map_.moveToFront(list_begin);
      } else {
        std::cout << "extract 1 key" << std::endl;
        auto nh_0 = map_.set_.extract(set_it_0);
        // modify
        nh_0.value()->key_.x_ = y;
        nh_0.value()->key_.y_ = x;
        nh_0.value()->clr_ = map_.getClr()+1;
        std::cout << "re-insert" << std::endl;
        map_.set_.insert(std::move(nh_0));
        std::cout << "move to front" << std::endl;
        map_.moveToFront(list_it);
      }
      list_begin->key_.x_;
    } else if(list_it->clr_ == (map_.getClr()+1)) {
      std::cout << "else if newly transposed key" << std::endl;
      // do nothing
    } else {
      std::cout << "key is cleared" << std::endl;
      list_it = map_.list_.end();
    }
    list_it++;
  }
}

std::string
Matrix::
to_string() {
  return map_.to_string();
}

void
Matrix::
clr() {
  map_.clear();
}

Map<Matrix::K,Matrix::V,std::less<Matrix::K>>::IterSetT
Matrix::
find(const Matrix::Index& x, const Matrix::Index& y) {
  return map_.find(Matrix::K(x,y));
  //return it_;
}

Map<Matrix::K, Matrix::V, std::less<Matrix::K>>::IterBoolSetT
Matrix::
add(const Matrix::Index& x, const Matrix::Index& y, const Matrix::Value& v) {
  Map<Matrix::K, Matrix::V, std::less<Matrix::K>>::IterBoolSetT it;
  it.first = map_.find(K(x,y));
  if(it.first != map_.end()) {
    std::cout << "add->key is found" << std::endl;
    std::cout << map_.to_string() << std::endl;
    (**it.first).val_.v_ += v;
    it.second = true;
    std::cout << map_.to_string() << std::endl;
    std::cout << "add->key is found" << std::endl;
  } else {
    std::cout << "add->key is not found" << std::endl;
    std::cout << map_.to_string() << std::endl;
    it = map_.try_emplace(K(x,y),V(v));
    std::cout << map_.to_string() << std::endl;
    std::cout << "add->key is not found" << std::endl;
  }
  return it;
}

#endif