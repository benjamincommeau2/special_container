#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>
#include <set>
#include <unordered_set>
#include <memory>
#include <deque>
#include "Timer.hpp"

class Matrix {
 public:
  /* ///////////////////////////////////////////////////////////////////
  basic types
  */ ///////////////////////////////////////////////////////////////////
  using Index = uint32_t;
  using Value = std::complex<double>;
  /* ///////////////////////////////////////////////////////////////////
  type T to store in container
  */ ///////////////////////////////////////////////////////////////////
  class T {
   friend class Matrix;
   public:
    Index x_; Index y_; mutable Value v_;
    T() {}
    T(const Index& x, const Index& y, const Value& v ) : x_(x), y_(y), v_(v) {}
    T(const Index& x, const Index& y, const double& r, const double& i ) {
      x_ = x; y_ = y; v_ = std::complex<double>(r,i);
    }
    std::string to_string() const {
      std::ostringstream oss;
      oss.precision(2);
      oss << std::fixed ;
      oss << "(" << x_ << "," << y_ << ")" << " ";
      oss << "(" << v_.real() << "," << v_.imag() << ")";
      return oss.str();
    }
    //std::vector<double> res = std::vector<double>(1<<5);
  };
  /* ///////////////////////////////////////////////////////////////////
  Tags
  */ ///////////////////////////////////////////////////////////////////
  struct order_xy {};
  struct order_yx {};
  struct random_access {};
  /* ///////////////////////////////////////////////////////////////////
  Type for Boost's Multi-Index Container
  */ ///////////////////////////////////////////////////////////////////
  using Container = boost::multi_index_container<
    T, // the data type stored
    boost::multi_index::indexed_by<
      boost::multi_index::random_access<
        boost::multi_index::tag<random_access>
      >
      ,
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<order_xy>,
        boost::multi_index::composite_key<
          T,
          boost::multi_index::member<T,Index,&T::x_>,
          boost::multi_index::member<T,Index,&T::y_>
        >,
        boost::multi_index::composite_key_compare<
          std::less<Index>,
          std::less<Index>
        >
      >
      ,
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<order_yx>,
        boost::multi_index::composite_key<
          T,
          boost::multi_index::member<T,Index,&T::y_>,
          boost::multi_index::member<T,Index,&T::x_>
        >,
        boost::multi_index::composite_key_compare<
          std::less<Index>,
          std::less<Index>
        >
      >
      /*
      */
    >
  >;
  /* ///////////////////////////////////////////////////////////////////
  variables
  */ ///////////////////////////////////////////////////////////////////
  Container container_;
  Index index_max_ = UINT32_MAX;
  /* ///////////////////////////////////////////////////////////////////
  implicit methods
  */ ///////////////////////////////////////////////////////////////////
  void insert(const Matrix::Index& x, const Matrix::Index& y,
    const Matrix::Value& v);
  std::string to_string() const;
  Container::index<Matrix::random_access>::type::iterator
    random_access_begin();
  Container::index<Matrix::random_access>::type::iterator
    random_access_end();
  void clear();
  void reserve(Index m);
};

/* ///////////////////////////////////////////////////////////////////
explicit methods
*/ ///////////////////////////////////////////////////////////////////
void Matrix::reserve(Index m) {
  container_.get<random_access>().reserve(m);
}

void Matrix::clear() {
  container_.clear();
}


Matrix::Container::index<Matrix::random_access>::type::iterator
Matrix::random_access_begin() {
  return container_.get<random_access>().begin();
}

Matrix::Container::index<Matrix::random_access>::type::iterator
Matrix::random_access_end() {
  return container_.get<random_access>().end();
}


std::string Matrix::to_string() const {
  auto it_random_access = container_.get<random_access>().cbegin();
  std::string tmp = "";
  tmp += "random_access\n";
  while(it_random_access != container_.get<random_access>().cend()) {
    tmp += it_random_access->to_string() + "\n";
    it_random_access++;
  }
  return tmp;
}

void Matrix::insert(const Matrix::Index& x, const Matrix::Index& y,
  const Matrix::Value& v) {
  container_.insert(random_access_end(),T(x,y,v));
}
//////////////////////////////////////////////////////////////////////

void printMEM() {
  auto sys = system("ps -eo cmd,%mem,%cpu --sort=-%mem | head -n 2 | tail -n 1");
}

int main() {
  Timer mytimer = Timer({"shuffle","init","iter","vec iter","deque init","deque iter"});
  Matrix mat;
  int index;
  int min_qubits = 1;
  int max_qubits = 11;
  mat.reserve(1<<(2*max_qubits));
  struct T {
    int x_; int y_; std::complex<double> v_;
    T() {}
    T(const int& x, const int& y, const double& r, const double& i) {
      x_ = x; y_ = y; v_ = std::complex<double>(r,i);
    }
  };
  //std::vector<std::shared_ptr<Matrix::T>> input;
  std::vector<Matrix::T> input;
  input.reserve(1<<(2*max_qubits));
  std::complex<double> res=0;
  std::deque<Matrix::T> input2;
  for(int q = min_qubits; q <= max_qubits; q++) {

    mytimer.start();
    input.resize(1<<(2*q));
    index = 0;
    std::cout << "a" << std::endl;
    for(int x = 0; x < 1<<q; x++) {
      for(int y = 0; y < 1<<q; y++) {
        //input[index] = (std::make_shared<Matrix::T>(Matrix::T(x,y,Matrix::Value(-x,-y))));
        input[index] = Matrix::T(x,y,Matrix::Value(-x,-y));
        index++;
      }
    }
    std::cout << "b" << std::endl;
    std::random_shuffle(input.begin(),input.end());
    mytimer.set_time("shuffle");

    mytimer.start();
    input2.clear(); 
    for(int i = 0; i< input.size(); i++) {
      input2.push_back(input[i]);
    }
    mytimer.set_time("deque init");

    mytimer.start();
    auto it2 = input2.begin();
    res = 0;
    index=0;
    for(int i = 0; i< input2.size(); i++) {
      //res += it2->v_;
      res+=input2[index].v_;
      it2++;
      index++;
    }
    mytimer.set_time("deque iter");
    std::cout << res << " " << log2(index) << std::endl;

    auto itv = input.begin();
    index = 0;
    res=0;
    while(itv != input.end()) {
      //res+=(*itv).v_;
      res+=input[index].v_;
      itv++;
      index++;
    }
    mytimer.set_time("vec iter");
    std::cout << log2(index) << " " << res << std::endl;

    mytimer.start();
    mat.clear();
    for(int i = 0; i < input.size(); i++) {
      mat.insert(input[i].x_,input[i].y_,input[i].v_);
      /*
      if(i%(1<<q)==0){
        printMEM();
      }
      */
    }
    mytimer.set_time("init");

    mytimer.start();
    auto it = mat.random_access_begin();
    index = 0;
    res = 0;
    while(it != mat.random_access_end()) {
      res+=it->v_;
      it++;
      index++;
    }
    mytimer.set_time("iter");
    std::cout << log2(index) << " " << res << std::endl;
    /*
    if(q==3) {
       std::cout << mat.to_string() << std::endl;
    }
    */
  }
  std::cout << mytimer.get_data_as_table_string() << std::endl;
  return 0;
}

/*






*/
