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


class Timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock>
    start_ = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock>
    end_ = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double>
    duration_ = std::chrono::duration_cast
    <std::chrono::nanoseconds>(end_-start_);
 public:
  void start() {
    start_ = std::chrono::high_resolution_clock::now();
  }
  void stop(std::string statement) {
    end_ = std::chrono::high_resolution_clock::now();
    duration_ = std::chrono::duration_cast
      <std::chrono::nanoseconds>(end_-start_);
    std::cout << log2(duration_.count()) << " "+statement << std::endl;
    start_ = std::chrono::high_resolution_clock::now();
  }
};

void test_list_slice() {
  struct T {
    int x_; T() {} T(const int& x) {x_ = x;}
  };
  std::list<T> mylist = {T(1),T(2),T(3),T(4),T(5)} ;
  struct myless {
    bool operator() (const std::list<T>::iterator& lhs,
      const std::list<T>::iterator& rhs) const {
      return lhs->x_ < rhs->x_;
    }
  };
  auto itl = mylist.begin();
  std::map<std::list<T>::iterator,bool,myless> mymap;
  auto itm = mymap.begin();
  while(itl!=mylist.end()) {
    mymap.insert(std::make_pair(itl,false));
    itl++;
  }
  auto itlf = mylist.end();
  std::cout << "print list and map after initialization" << std::endl;
  itl = mylist.begin(); itm = mymap.begin();
  while(itl != mylist.end() && itm != mymap.end()) {
    if(itl->x_ == 3) { itlf = itl; }
    std::cout << itl->x_ << " " << itm->first->x_ << std::endl;
    itl++; itm++;
  }
  std::cout << "itlf->x_=" << itlf->x_ << std::endl;
  mylist.splice(mylist.begin(),mylist,itlf);
  std::cout << "print list and map after splice" << std::endl;
  itl = mylist.begin(); itm = mymap.begin();
  while(itl != mylist.end() && itm != mymap.end()) {
    if(itl->x_ == 5) { itlf = itl; }
    std::cout << itl->x_ << " " << itm->first->x_ << std::endl;
    itl++; itm++;
  }
  std::cout << "itlf->x_=" << itlf->x_ << std::endl;
  mylist.splice(mylist.begin(),mylist,itlf);
  std::cout << "print list and map after splice" << std::endl;
  itl = mylist.begin(); itm = mymap.begin();
  while(itl != mylist.end() && itm != mymap.end()) {
    if(itl->x_ == 1) { itlf = itl; }
    std::cout << itl->x_ << " " << itm->first->x_ << std::endl;
    itl++; itm++;
  }
  // this breaks map order, but no seg fault
  itlf->x_ = 6;
  std::cout << "itlf->x_=" << itlf->x_ << std::endl;
  mylist.splice(mylist.begin(),mylist,itlf);
  std::cout << "print list and map after splice" << std::endl;
  itl = mylist.begin(); itm = mymap.begin();
  while(itl != mylist.end() && itm != mymap.end()) {
    if(itl->x_ == 1) { itlf = itl; }
    std::cout << itl->x_ << " " << itm->first->x_ << std::endl;
    itl++; itm++;
  }
}

int main() {
  struct intint {
    int x_; int y_;
    intint() {};
    intint(const int& x, const int& y) {
      x_ = x; y_ = y;
    }
    bool operator <(const intint& rhs) const {
      return x_ != rhs.x_ ? x_ < rhs.x_ : y_ < rhs.y_;
    }
    std::string to_string() {
      std::string tmp = "";
      tmp += "(";
      tmp += std::to_string(x_);
      tmp += ",";
      tmp += std::to_string(y_);
      tmp += ")";
      return tmp;
    }
  };
  struct intstr {
    int x_; std::string y_;
    intstr() {};
    intstr(const int& x, const std::string& y) {
      x_ = x; y_ = y;
    }
    bool operator <(const intstr& rhs) const {
      return x_ != rhs.x_ ? x_ < rhs.x_ : y_ < rhs.y_;
    }
    std::string to_string() {
      std::string tmp = "";
      tmp += "(";
      tmp += std::to_string(x_);
      tmp += ",";
      tmp += y_;
      tmp += ")";
      return tmp;
    }
  };
  //test_list_slice();
  Map<intint,intstr> tmap;
  tmap.try_emplace(intint(1,2),intstr(3,"a"));
  std::cout << tmap.to_string() << std::endl;
  tmap.try_emplace(intint(5,6),intstr(7,"b"));
  std::cout << tmap.to_string() << std::endl;
  tmap.try_emplace(intint(9,10),intstr(11,"c"));
  std::cout << tmap.to_string() << std::endl;
  tmap.setClrMax(1);
  tmap.clear();
  std::cout << tmap.to_string() << std::endl;
  tmap.try_emplace(intint(11,21),intstr(31,"a1"));
  std::cout << tmap.to_string() << std::endl;
  tmap.try_emplace(intint(1,2),intstr(3,"a"));
  tmap.try_emplace(intint(5,6),intstr(7,"b"));
  tmap.try_emplace(intint(9,10),intstr(11,"c"));
  std::cout << tmap.to_string() << std::endl;
  tmap.try_emplace(intint(9,10),intstr(11,"c"));
  std::cout << tmap.to_string() << std::endl;
  std::cout << "Clear Map clr_=2" << std::endl;
  tmap.setClrMax(2);
  tmap.clear();
  tmap.try_emplace(intint(1,2),intstr(3,"a"));
  tmap.try_emplace(intint(5,6),intstr(7,"b"));
  tmap.try_emplace(intint(9,10),intstr(11,"c"));
  std::cout << tmap.to_string() << std::endl;
  return 0;
}