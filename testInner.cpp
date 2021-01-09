#include <cstddef>
#include <iostream>
#include <string>

template<bool IsConst> 
class A {
 public:
  A (int* y) {x = y;}
  template<bool IsConst_ = IsConst>
    std::enable_if_t<IsConst_, int&>
  X() {return *x; }
  template<bool IsConst_ = IsConst>
    std::enable_if_t<!IsConst_, const int&>
  X() const {return *x; }
 private:
  int* x;
};

template<bool IsConst>
void test_A(A<IsConst> a) {
  //a.X() = 1;
  std::cout << a.X()  << std::endl;
}

int main() {
  int x=0 ;
  std::cout << x  << std::endl;
  A<true>(&x).X()=1;
  std::cout << x  << std::endl;
  int y = A<false>(&x).X();
  std::cout << y  << std::endl;
  test_A(A<true>(&x));
  return 0;
}
