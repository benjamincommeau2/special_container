#include <cstddef>
#include <iostream>
#include <string>

template<bool IsConst> 
class A {
 public:
  A() {}
  A(const double& v) {setV(v);}
  void setV(const double& v) {qa_.setV(v);}
  template<bool IsConst_>
  class QA {
   public:
    template<bool IsConst__ = IsConst_> std::enable_if_t<!IsConst__,double&>
    val() {return v_;}
    template<bool IsConst__ = IsConst_> std::enable_if_t<IsConst__,const double&>
    val() const {return v_;}
    void setV(const double& v) {v_ = v;}
   private:
    double v_;
  };
  QA<IsConst>& operator*() const {return qa_;}
  QA<IsConst>* operator->() const {return &qa_;}
 private:
  mutable QA<IsConst> qa_;
};

template<bool IsConst>
void test_A(const A<IsConst>& a) {
  //a->val()=33;
  std::cout << (*a).val() << std::endl;
}

int main() {
  A<false> a(2);
  A<true> b(4);
  a->val()=33;
  //b->val()=33;
  test_A(b);
  return 0;
}
