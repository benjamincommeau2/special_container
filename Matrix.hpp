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
  void clear();
  Map<K,V>::IterSetT find(const Index& x, const Index& y);
  Map<K,V>::IterBoolSetT
    add(const Index& x, const Index& y, const Value& v);
  void transpose();
  void ABteC(const Matrix& A, const Matrix& B, Matrix& C);
  Value getCoeff(const Matrix::Index& x, const Matrix::Index& y);
  void AoBeC(Matrix& A, Matrix& B, Matrix& C);
  void transpose2();
  Map<K,V>::Clr getClr();
  void addScaleMatrixMultTrans(const Value& v, const Matrix& A, const Matrix& B);
  Map<Matrix::K,Matrix::V>::MapIterator map_begin();
  Map<Matrix::K,Matrix::V>::MapIterator map_end();
  Map<Matrix::K,Matrix::V>::ConstMapIterator map_cbegin() const;
  Map<Matrix::K,Matrix::V>::ConstMapIterator map_cend() const;
  Map<Matrix::K,Matrix::V>::MapIterator map_lower_bound(const K& k);
  Map<Matrix::K,Matrix::V>::MapIterator map_upper_bound(const K& k);


 private:
  Map<K,V> map_;
  Map<Matrix::K,Matrix::V>::IterListT it_;
  Index index_max_ = UINT32_MAX;
  Map<Matrix::K,Matrix::V>::IterListT list_it_;
  Map<Matrix::K,Matrix::V>::IterSetT set_it_;
};

/* //////////////////////////////////////////////////////////////
Explicit Methods
*/ //////////////////////////////////////////////////////////////

Map<Matrix::K,Matrix::V>::ConstMapIterator
Matrix::
map_cbegin() const {
  return map_.map_cbegin();
}

Map<Matrix::K,Matrix::V>::ConstMapIterator
Matrix::
map_cend() const {
  return map_.map_cend();
}

Map<Matrix::K,Matrix::V>::MapIterator
Matrix::
map_begin() {
  return map_.map_begin();
}

Map<Matrix::K,Matrix::V>::MapIterator
Matrix::
map_end() {
  return map_.map_end();
}

Map<Matrix::K,Matrix::V>::MapIterator
Matrix::
map_lower_bound(const K& k) {
  return map_.map_lower_bound(k);
}

Map<Matrix::K,Matrix::V>::MapIterator
Matrix::
map_upper_bound(const K& k) {
  return map_.map_upper_bound(k);
}

void
Matrix::
addScaleMatrixMultTrans(const Matrix::Value& s, const Matrix& A,
  const Matrix& B) {
  // C = A@Bt
  auto a_lower = A.map_cbegin();
  auto a = A.map_cbegin();
  auto a_upper = A.map_cbegin();
  Index a_x;
  Index a_y;

  auto b_lower = B.map_cbegin();
  auto b = B.map_cbegin();
  auto b_upper = B.map_cbegin();
  Index b_x;
  Index b_y;

  Value v;
  while(a != A.map_end()) {
    a_x = a_upper->key_.x_;
    a_lower = A.map_lower_bound(K(a_x,0))
    a_upper = A.map_upper_bound(K(a_x,index_max_))
    while(b != B.map_cend()) {
      b_x = b_upper->key_.x_;
      b_lower = B.map_lower_bound(K(b_x,b_y));
      b_upper = B.map_upper_bound(K(b_x,b_y));
      v = 0;
      while(a != a_upper and b != b_upper) {
        a_y = a->key_.y_;
        b_y = b->key_.y_;
        if(ay == by) {
          v += a->val_ * b->val_;
          a++; b++;
        } else if(ay < by) {
          a++;
        } else {
          b++;
        }
      }
      add(a_x^b_x, a_y^b_y, s*v);
    }
  }
}

Map<Matrix::K,Matrix::V>::Clr
Matrix::
getClr() {
  return map_.getClr();
}

void
Matrix::
transpose2() {
  if (map_.getClr() == map_.getClrMax()) { map_.flatten_clear(); }
  auto list_it = map_.list_begin();
  auto map_it_trans = map_.map_begin();
  auto map_it = map_.map_begin();
  Index x; Index y;
  while(list_it != map_.list_end() and list_it->clr_ == map_.getClr()) {
    x = list_it->key_.x_;
    y = list_it->key_.y_;
    if(x == y) {
      list_it->clr_ = map_.getClr()+1;
      list_it++;
    } else {
      list_it++;
      map_it_trans = map_.map_find(K(y,x));
      map_it = map_.map_find(K(x,y));
      if(map_it_trans != map_.map_end()) {
        // std::cout << "Transposed Key exists" << std::endl;
        if(map_it_trans == list_it) {list_it++;}
        map_.reInsertKey(map_it, K(y,x) , map_it_trans, K(x,y));
        map_it->clr_ = map_.getClr()+1;
        map_it_trans->clr_ = map_.getClr()+1;
        map_.move2Front(map_it);
        map_.move2Front(map_it_trans);
      } else {
        // std::cout << "Transposed Key does not exists" << std::endl;
        map_.reInsertKey(map_it, K(y,x));
        map_it->clr_ = map_.getClr()+1;
        map_.move2Front(map_it);
      }
    }
  }
  map_.setClr(map_.getClr()+1);
}

void
Matrix::
AoBeC(Matrix& A, Matrix& B, Matrix& C) {

  auto listA_begin = A.map_.list_.begin();
  listA_begin++;
  auto listA = listA_begin;

  auto listB_begin = B.map_.list_.begin();
  listB_begin++;
  auto listB = listB_begin;

  Index xA;
  Index yA;
  Value vA;

  Index xB;
  Index yB;
  Value vB;

  C.clear();

  while(listA != A.map_.list_.end() and listA->clr_ == A.map_.getClr()) {
    xA = listA->key_.x_;
    yA = listA->key_.y_;
    vA = listA->val_.v_;
    listB = listB_begin;
    while(listB != B.map_.list_.end() and listB->clr_ == B.map_.getClr()) {
      xB = listB->key_.x_;
      yB = listB->key_.y_;
      vB = listB->val_.v_;
      C.add(xA^xB,yA^yB,vA*vB);
      listB++;
    }
    listA++;
  }
}

Matrix::
Value
Matrix::
getCoeff(const Matrix::Index& x, const Matrix::Index& y) {
  list_it_ = map_.list_.begin();
  map_.setIterList(list_it_, Matrix::K(x,y));
  set_it_ = map_.set_.find(list_it_);
  if(set_it_ != map_.set_.end()) {
    if((**set_it_).clr_ == map_.getClr()) {
      return (**set_it_).val_.v_;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

void
Matrix::
ABteC(const Matrix& A, const Matrix& B, Matrix& C) {

  // build indirect iterator to avoid constant de-qualification
  Map<Matrix::K,Matrix::V,std::less<Matrix::K>>::ListT temp_listA;
  temp_listA.push_back(Map<Matrix::K,Matrix::V,std::less<Matrix::K>>::T());
  auto listA = temp_listA.begin();

  // build indirect iterator to avoid constant de-qualification
  Map<Matrix::K,Matrix::V,std::less<Matrix::K>>::ListT temp_listB;
  temp_listB.push_back(Map<Matrix::K,Matrix::V,std::less<Matrix::K>>::T());
  auto listB = temp_listB.begin();


  auto setA_begin = A.map_.set_.cbegin();
  auto setA_end = A.map_.set_.cbegin();
  auto setA = A.map_.set_.cbegin();

  auto setB_begin = B.map_.set_.cbegin();
  auto setB_end = B.map_.set_.cbegin();
  auto setB = B.map_.set_.cbegin();


  Matrix::Value value = 0;
  Index xA;
  Index xB;
  Index yA;
  Index yB;
  C.clear();

  // loop A
  while(setA_end != A.map_.set_.end()) {
    // get key
    xA = (**setA_end).key_.x_;
    // get lower bound
    listA->key_ = K(xA, 0);
    setA_begin = A.map_.set_.lower_bound(listA);
    // get upper bound
    listA->key_ = K(xA, index_max_);
    setA_end = A.map_.set_.upper_bound(listA);
    // initialize B
    setB_begin = B.map_.set_.cbegin();
    setB_end = B.map_.set_.cbegin();
    setB = B.map_.set_.cbegin();
    // loop A
    while(setB_end != B.map_.set_.end()) {
      setA = setA_begin;
      // get key
      xB = (**setB_end).key_.x_;
      // get lower bound
      listB->key_ = K(xB, 0);
      setB_begin = B.map_.set_.lower_bound(listB);
      // get upper bound
      listB->key_ = K(xB, index_max_);
      setB_end = B.map_.set_.upper_bound(listB);
      // loop B
      setB = setB_begin;
      value = 0;
      while(setB != setB_end and setA != setA_end) {
        yA = (**setA).key_.y_;
        yB = (**setB).key_.y_;
        if(yA == yB) {
          value += (**setA).val_.v_ * (**setB).val_.v_;
          setA++;
          setB++;
        } else if(yA < yB) {
          setA++;
        } else {
          setB++;
        }
      }
      C.add(xA,xB,value);
    }
  }
}

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
    //std::cout << map_.to_string() << std::endl;
    //std::cout << list_it->key_.x_ << " " << list_it->key_.y_ << std::endl;
    x = list_it->key_.x_;
    y = list_it->key_.y_;
    if (list_it->clr_ == map_.getClr() and x != y) {
      list_begin = map_.list_.begin();
      //std::cout << "if non-cleared key" << std::endl;
      set_it_0 = map_.set_.find(list_it);
      list_begin->key_.x_ = y;
      list_begin->key_.y_ = x;
      set_it_1 = map_.set_.find(list_begin);
      if (set_it_1 != map_.set_.end()) {
        //std::cout << "extract 2 keys" << std::endl;
        auto nh_1 = map_.set_.extract(set_it_1);
        auto nh_0 = map_.set_.extract(set_it_0);
        // modify
        nh_1.value()->key_.x_ = x;
        nh_1.value()->key_.y_ = y;
        nh_1.value()->clr_ = map_.getClr()+1;
        list_begin = nh_1.value();
        nh_0.value()->key_.x_ = y;
        nh_0.value()->key_.y_ = x;
        nh_0.value()->clr_ = map_.getClr()+1;
        //std::cout << "re-insert 2 keys" << std::endl;
        map_.set_.insert(std::move(nh_0));
        map_.set_.insert(std::move(nh_1));
        //std::cout << "move to front" << std::endl;
        map_.moveToFront(list_begin);
      } else {
        //std::cout << "extract 1 key" << std::endl;
        auto nh_0 = map_.set_.extract(set_it_0);
        // modify
        nh_0.value()->key_.x_ = y;
        nh_0.value()->key_.y_ = x;
        nh_0.value()->clr_ = map_.getClr()+1;
        //std::cout << "re-insert" << std::endl;
        map_.set_.insert(std::move(nh_0));
        //std::cout << "move to front" << std::endl;
      }
      auto it = list_it;
      list_it++;
      map_.moveToFront(it);
    } else if(list_it->clr_ == (map_.getClr()+1)) {
      //std::cout << "else if newly transposed key" << std::endl;
      // do nothing
      list_it++;
    } else if (list_it->clr_ < map_.getClr()){
      //std::cout << "key is cleared" << std::endl;
      list_it = map_.list_.end();
    } else {
      //std::cout << "do nothing" << std::endl;
      list_it->clr_ = map_.getClr()+1;
      list_it++;
    }
  }
  map_.setClr(map_.getClr()+1);
}

std::string
Matrix::
to_string() {
  return map_.to_string();
}

void
Matrix::
clear() {
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
  if(it.first != map_.end() and (**it.first).clr_ == map_.getClr()) {
   // std::cout << "add->key is found" << std::endl;
   // std::cout << map_.to_string() << std::endl;
    (**it.first).val_.v_ += v;
    it.second = true;
    //std::cout << map_.to_string() << std::endl;
    //std::cout << "add->key is found" << std::endl;
  } else {
    //std::cout << "add->key is not found" << std::endl;
    //std::cout << map_.to_string() << std::endl;
    it = map_.try_emplace(K(x,y),V(v));
    //std::cout << map_.to_string() << std::endl;
    //std::cout << "add->key is not found" << std::endl;
  }
  return it;
}

#endif
