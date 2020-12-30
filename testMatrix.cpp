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

void test_transpose() {
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
}

int main() {
  Matrix A;
  std::vector<T> tA = {
  T(0, 0,        0.9031785237401092,0),
  T(0, 1,        0.5723564912601801,0),
  T(0, 3,        0.6984202359093901,0),
  T(1, 2 ,       0.3614963440850427,0),
  T(2, 0,        0.3917661949793543,0),
  T(2, 1 ,       0.6329631043924608,0),
  T(3, 0 ,       0.005159148556627802,0),
  T(3, 2 ,       0.6041319382476973,0)};
  for(uint32_t i = 0; i< tA.size(); i++) {
    A.add(tA[i].x,tA[i].y,tA[i].v);
  }
  std::cout << "A=" << std::endl;
  std::cout << A.to_string() << std::endl;
  Matrix B;
  std::vector<T> tB = {
  T(0, 0 ,       0.6743304229389547 ,0),
  T(0, 2 ,       0.01591036096127696, 0),
  T(0, 3 ,       0.39294230987084167, 0),
  T(1, 0  ,      0.6723172732412332, 0),
  T(1, 1 ,       0.5277647948207705, 0),
  T(2, 0 ,       0.3308392493496919, 0),
  T(2, 2 ,       0.8688014582078086, 0),
  T(2, 3 ,       0.6940315754218798,0)};
  for(uint32_t i = 0; i< tB.size(); i++) {
    B.add(tB[i].x,tB[i].y,tB[i].v);
  }
  std::cout << "B=" << std::endl;
  std::cout << B.to_string() << std::endl;
  Matrix C;
  std::vector<T> tC = {
  T(0, 0 ,       0.9938459114290126,0),
  T(0, 1 ,       0.3020696061742651,0),
  T(0, 2 ,       0.01436989632517839,0),
  T(0, 3 ,       0.3548970553441753,0),
  T(1, 0 ,       0.11959717911975347,0),
  T(1, 2 ,       0.31406855087787683,0),
  T(1, 3 ,       0.2508898771945921,0),
  T(2, 0 ,       0.6897318923610583,0),
  T(2, 1 ,       0.3340556429188051,0),
  T(2, 2 ,       0.006233141574547536,0),
  T(2, 3 ,       0.153941513584498,0),
  T(3, 0 ,       0.20334952778623833,0),
  T(3, 2 ,       0.5249527928152979,0),
  T(3, 3 ,       0.42131388861553126,0)};
  for(uint32_t i = 0; i< tC.size(); i++) {
    C.add(tC[i].x,tC[i].y,tC[i].v);
  }
  std::cout << "C=" << std::endl;
  std::cout << C.to_string() << std::endl;
  B.transpose();
  std::cout << "Transposed B" << std::endl;
  Matrix Cn;
  Cn.ABteC(A,B,Cn);
  std::cout << "Cn=" << std::endl;
  std::cout << Cn.to_string() << std::endl;
  std::cout << "C=" << std::endl;
  std::cout << C.to_string() << std::endl;

  bool is_error = false;
  for(uint32_t x = 0; x < 4; x++) {
    for(uint32_t y = 0; y < 4; y++) {
      if(std::abs(C.getCoeff(x,y)-Cn.getCoeff(x,y))>1.e-10) {
        std::cout << "Error in testMatrix->ABteq->" 
          << "( C.getCoeff(x,y) , Cn.getCoeff(x,y) )=" << std::endl;
        std::cout << "( " << Matrix::V(C.getCoeff(x,y)).to_string() << " , "
          << Matrix::V(Cn.getCoeff(x,y)).to_string() << " ) " << std::endl;
        is_error=true;
      }
    }
  }
  if(is_error == false) {
    std::cout << "Passed ABteC test." << std::endl; 
  } else {
    std::cout << "Failed ABteC test." << std::endl; 
  }

  return 0;
}
