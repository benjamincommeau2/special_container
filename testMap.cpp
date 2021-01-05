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

void quick_print_test() {
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
}

void test_try_emplace() {
  struct K {
    int x; int y;
    K() {};
    K(const int& x_, const int& y_) { x = x_; y = y_; }
    bool operator <(const K& rhs) const {
      return x != rhs.x ? x < rhs.x : y < rhs.y;
    }
    std::string to_string() {
      std::string tmp = "";
      tmp += "(";
      tmp += std::to_string(x);
      tmp += ",";
      tmp += std::to_string(y);
      tmp += ")";
      return tmp;
    }
  };
  struct V {
    std::complex<double> v;
    V() {}
    V(const std::complex<double>& v_) {v = v_;}
    V(const double& r_, const double& i_) {v = std::complex<double>(r_, i_);}
    std::string to_string() {
      return "(" + std::to_string(v.real()) + " "
      + std::to_string(v.imag()) + ")";
    }
  };
  struct T {
    K k; V v;
    T() {}
    T(const K& k_, const V& v_) { k = k_; v = v_; }
    T(const int& x_, const int& y_, const double& r_, const double& i_) {
      k = K(x_, y_); v = V(r_, i_);
    }
  };
  Map<K, V> map;
  std::vector<T> input = {T(1,2,3,4)};
  std::vector<T> output = {T(1,2,3,4)};
  for (int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << map.to_string() << std::endl;
}

template<class K, class V, class C = std::less<K>>
void test_constant_iterator(const Map<K,V,C>& M) {
  auto it = M.map_cbegin();
  it = M.map_cbegin();
  it = M.map_clower_bound(K(0,0));
  (*it).val_.v = -1;
}

int main() {
  struct K {
    int x; int y;
    K() {};
    K(const int& x_, const int& y_) { x = x_; y = y_; }
    bool operator <(const K& rhs) const {
      return x != rhs.x ? x < rhs.x : y < rhs.y;
    }
    std::string to_string() {
      std::string tmp = "";
      tmp += "(";
      tmp += std::to_string(x);
      tmp += ",";
      tmp += std::to_string(y);
      tmp += ")";
      return tmp;
    }
  };
  struct V {
    std::complex<double> v;
    V() {}
    V(const std::complex<double>& v_) {v = v_;}
    V(const double& r_, const double& i_) {v = std::complex<double>(r_, i_);}
    std::string to_string() {
      return "(" + std::to_string(v.real()) + " "
      + std::to_string(v.imag()) + ")";
    }
  };
  struct T {
    K k; V v;
    T() {}
    T(const K& k_, const V& v_) { k = k_; v = v_; }
    T(const int& x_, const int& y_, const double& r_, const double& i_) {
      k = K(x_, y_); v = V(r_, i_);
    }
  };
  Map<K, V> map;
  std::vector<T> input = {T(1,2,1,2),T(3,3,3,3),T(7,3,7,3),T(2,7,2,7)};
  for (int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << "print map" << std::endl;
  std::cout << map.to_string() << std::endl;
  std::cout << "test list iterator forward" << std::endl;
  Map<K,V>::ListIterator it = map.list_begin(); 
  int h = 0;
  while(it != map.list_end()) {
    std::cout << it->key_.to_string() << " " << it->val_.to_string() << std::endl;
    if(h%2==0) {it++;} else {++it;}
    h++;
  }
  std::cout << std::endl;
  std::cout << "test list iterator reverse" << std::endl;
  h = 0;
  it = map.list_end();
  it--;
  while(true) {
    std::cout << it->key_.to_string() << " " << it->val_.to_string() << std::endl;
    if (it == map.list_begin()) {break;}
    if(h%2==1) {it--;} else {--it;}
    h++;
  }
  std::cout << std::endl;
  std::cout << "test map map iterator" << std::endl;
  auto itm = map.map_begin();
  h = 0;
  while(itm != map.map_end()) {
    std::cout << itm->key_.to_string() << " " << itm->val_.to_string()
      << std::endl;
    if(h%2==1) {itm++;} else {++itm;}
    h++;
  }
  //std::vector<T> input = {T(1,2,1,2),T(3,3,3,3),T(7,3,7,3),T(2,7,2,7)};
  std::cout << std::endl;
  std::cout << "test map lower and upper bound" << std::endl;
  auto it_begin = map.map_lower_bound(K(1,3));
  auto it_end = map.map_upper_bound(K(3,4));
  itm = it_begin;
  h=0;
  while(itm != it_end) {
    std::cout << itm->key_.to_string() << " " << itm->val_.to_string()
      << std::endl;
    if(h%2==1) {itm++;} else {++itm;}
    h++;
  }
  std::cout << std::endl;
  std::cout << "test map find" << std::endl;
  auto it_find = map.map_find(K(3,3));
  std::cout << it_find->key_.to_string() << " " << it_find->val_.to_string()
    << std::endl;
  std::cout << "Test Constant Iterator" << std::endl;
  test_constant_iterator(map);
  itm = map.map_begin();
  h=0;
  while(itm != map.map_end()) {
    std::cout << itm->key_.to_string() << " " << itm->val_.to_string()
      << std::endl;
    if(h%2==1) {itm++;} else {++itm;}
    h++;
  }
  return 0;
}
