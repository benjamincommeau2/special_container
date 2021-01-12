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

template<class K, class V>
void test_constantness(Map<K,V>& map) {
  auto it = map.map_cbegin();
  std::cout << map.to_string() << std::endl;
  //std::cout << it->val().to_string() << std::endl;
  //auto v = (*it).val();
 // std::cout << (*it).val().to_string() << std::endl;
  //(*it).val()=0;
  //it->val()=0;
}

int main() {
  struct K {
    int x; int y;
    K() {};
    K(const int& x_, const int& y_) { x = x_; y = y_; }
    bool operator <(const K& rhs) const {
      return x != rhs.x ? x < rhs.x : y < rhs.y;
    }
    std::string to_string() const {
      return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
    }
  };
  struct V {
    std::complex<double> v;
    V() {}
    V(const std::complex<double>& v_) {v = v_;}
    V(const double& v_) {v = v_;}
    V(const double& r_, const double& i_) {v = std::complex<double>(r_, i_);}
    std::string to_string() const {
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
  // start testing
  std::vector<T> input = {T(7,3,7,3),T(3,3,3,3),T(1,2,1,2),T(2,7,2,7)};
  for(int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << "try_emplace() for first initialize" << std::endl;
  auto citm = map.map_cbegin();
  citm = map.map_cbegin();
  auto itm = map.map_begin();
  std::cout << map.to_string() << std::endl;
  /*
  map.hard_clear();
  for(int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << "try_emplace() initialize after hard_clear()" << std::endl;
  std::cout << map.to_string() << std::endl;

  map.clear();
  for(int i = 0; i < input.size()-2; i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << "try_emplace() after clear" << std::endl;
  std::cout << map.to_string() << std::endl;

  map.clear();
  input = {T(77,11,77,11),T(7,3,7,3),T(3,3,3,3),T(33,34,33,34),T(13,32,13,32),
    T(2,7,2,7)};
  for(int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << "try_emplace() after clear with new elements" << std::endl;
  std::cout << map.to_string() << std::endl;

  std::cout << "test constantness" << std::endl;
  test_constantness(map);
  std::cout << map.to_string() << std::endl;
  */
  return 0;
}
