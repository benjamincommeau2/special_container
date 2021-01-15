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
void test_constantness(const Map<K,V>& map) {
  //auto itm = map.map_begin(); // fails constant qualifier check
  auto itm = map.map_cbegin();
  std::cout << "itm->val()=" << itm->val().to_string() << std::endl;
  itm->val() = 69; // compiles, but does not modify
  std::cout << "itm->val()=" << itm->val().to_string() << std::endl;
  auto itl = map.list_cbegin();
  //auto itl = map.list_begin(); // fails constant qualifier check
  std::cout << "itl->val()=" << itl->val().to_string() << std::endl;
  itl->val() = 69; // compiles, but does not modify
  std::cout << "itl->val()=" << itl->val().to_string() << std::endl;
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
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2) << "(" << v.real() << " "
        << v.imag() << ")";
      return oss.str();
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
  std::vector<T> input = {T(7,3,7,3),T(3,3,3,3),T(1,2,1,2),T(2,7,2,7),
    T(4,7,4,7),T(3,7,3,7),T(6,7,6,7),T(3,5,3,5),T(1,9,1,9)};
  std::cout << "Initialize map ///////////////////////////////////////////////" << std::endl;
  for(int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << map.to_string() << std::endl;
  std::cout << "Clear and ReInitialize map less elements and new ////////////////" << std::endl;
  input = {T(3,5,3,5),T(1,9,1,9),T(7,3,7,3),T(3,7,3,7),
    T(3,3,3,3),T(1,2,1,2),T(2,7,2,7),T(2,7,2,7),T(5,1,5,1),T(2,2,2,2),T(2,1,2,1)};
  map.clear();
  for(int i = 0; i < input.size(); i++) {
    map.try_emplace(input[i].k,input[i].v);
  }
  std::cout << map.to_string() << std::endl;
  test_constantness(map);
  std::cout << map.to_string() << std::endl;
  auto itm = map.map_begin();
  //itm.It(); // error: is private
  //itm.setKey(K(0,0)); // error: is private
  //itm.setClr(1); // error: is private
  return 0;
}
