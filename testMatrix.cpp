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
#include "Matrix.hpp"
#include <sstream>


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

typedef std::complex<double> V;
typedef uint32_t I;

struct T {
  I x; I y; V v;
  T() {}
  T(const I& x_, const I& y_, const double& r_, const double& i_) {
    x = x_; y = y_; v = V(r_,i_);
  }
  double r() {return v.real();}
  double i() {return v.imag();}
};

int main() {
  Matrix mat;
  std::vector<T> t = {T(1,1,1,1),T(1,2,1,2),T(2,2,2,2),T(2,1,2,1),T(3,4,3,4),
    T(4,3,4,3),T(5,5,5,5),T(1,6,1,6),T(2,4,2,4)};
  for(uint32_t i = 0; i< t.size(); i++) {
    //std::cout << "t[i].v=" << t[i].v.real() << " " << t[i].v.imag() << std::endl;
    mat.add(t[i].x,t[i].y,t[i].v);
  }
  mat.clear();
  std::cout << "clear" << std::endl;
  std::cout << mat.to_string() << std::endl;
  for(uint32_t i = 0; i<= 6; i++) {
    //std::cout << "t[i].v=" << t[i].v.real() << " " << t[i].v.imag() << std::endl;
    mat.add(t[i].x,t[i].y,t[i].v);
  }
  std::cout << mat.to_string() << std::endl;
  std::cout << "start transpose" << std::endl;
  mat.transpose();
  std::cout << mat.to_string() << std::endl;
  return 0;
}
