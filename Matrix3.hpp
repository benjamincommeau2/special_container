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
    std::string to_string() const {
      std::ostringstream oss;
      oss.precision(2);
      oss << std::fixed ;
      oss << "(" << x_ << "," << y_ << ")" << " ";
      oss << "(" << v_.real() << "," << v_.imag() << ")";
      return oss.str();
    }
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
      >,
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
      >,
      boost::multi_index::random_access<
        boost::multi_index::tag<random_access>
      >
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
  void xy_sort();
  void yx_sort();
  Container::index<order_xy>::type::iterator
    xy_find(const Matrix::Index& x, const Matrix::Index& y);
  void pesAB(const Value& s, Matrix& A, Matrix& B, bool sort);
  std::pair< Container::index<random_access>::type::iterator,
    Container::index<random_access>::type::iterator
  > random_access_equal_range_xy(const Index& x);
  std::pair< Container::index<random_access>::type::iterator,
    Container::index<random_access>::type::iterator
  > random_access_equal_range_yx(const Index& y);
  void add(const Index& x, const Index& y, const Value& v, bool sort);
  Container::index<Matrix::order_xy>::type::iterator
    xy_begin();
  Container::index<Matrix::order_yx>::type::iterator
    yx_begin();
  Container::index<Matrix::random_access>::type::iterator
    random_access_begin();
  Container::index<Matrix::random_access>::type::iterator
    random_access_end();
  Value getCoeff(const Index x, const Index y);
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

Matrix::Value Matrix::getCoeff(const Matrix::Index x, const Matrix::Index y) {
  auto it = xy_find(x,y);
  return it->v_;
}

void Matrix::pesAB(const Matrix::Value& s, Matrix& A, Matrix& B, bool sort) {

  if(sort) {
    A.xy_sort();
    B.yx_sort();
  }

  auto iA = A.random_access_begin();
  auto iA_old = A.random_access_begin();
  Index xA;

  auto iB = B.random_access_begin();
  Index yB;

  Value v = 0;

  while(iA != A.random_access_end()) {
    xA = iA->x_;
    iA_old = iA;
    iB = B.random_access_begin();
if(false) {
    while(iB != B.random_access_end()) {
      iA = iA_old;
      yB = iB->y_;
      v = 0;
      while(xA == iA->x_ && iA != A.random_access_end()
        && yB == iB->y_ && iB != B.random_access_end()) {
        if(iA->y_ == iB->x_) {
          v += iA->v_ * iB->v_;
          iA++; iB++;
        } else if(iA->y_ < iB->x_) { iA++; } else { iB++; }
      }
      add(xA, yB, s*v, sort);
      while(yB == iB->y_ && iB != B.random_access_end()) {iB++;}
    }
}
    while(xA == iA->x_ && iA != A.random_access_end()) {iA++;}
  }
}

void Matrix::
add(const Matrix::Index& x, const Matrix::Index& y, const Matrix::Value& v, bool sort) {
  auto it = xy_find(x,y);
  if(it != container_.get<order_xy>().end()) {
    it->v_ += v;
  } else {
    container_.insert(T(x,y,v)); // very time consuming
  }
}
  

std::pair<
  Matrix::Container::index<Matrix::random_access>::type::iterator,
  Matrix::Container::index<Matrix::random_access>::type::iterator
> Matrix::
random_access_equal_range_xy(const Matrix::Index& x) {
  std::pair<
    Matrix::Container::index<Matrix::random_access>::type::iterator,
    Matrix::Container::index<Matrix::random_access>::type::iterator
  > res;
  auto rangeXY = container_.get<order_xy>().equal_range(x);
  res.first = container_.project<random_access>(rangeXY.first);
  res.second = container_.project<random_access>(rangeXY.second);
  return res;
}

std::pair<
  Matrix::Container::index<Matrix::random_access>::type::iterator,
  Matrix::Container::index<Matrix::random_access>::type::iterator
> Matrix::
random_access_equal_range_yx(const Matrix::Index& y) {
  std::pair<
    Matrix::Container::index<Matrix::random_access>::type::iterator,
    Matrix::Container::index<Matrix::random_access>::type::iterator
  > res;
  auto rangeYX = container_.get<order_yx>().equal_range(y);
  res.first = container_.project<random_access>(rangeYX.first);
  res.second = container_.project<random_access>(rangeYX.second);
  return res;
}

Matrix::Container::index<Matrix::random_access>::type::iterator
Matrix::random_access_begin() {
  return container_.get<random_access>().begin();
}

Matrix::Container::index<Matrix::random_access>::type::iterator
Matrix::random_access_end() {
  return container_.get<random_access>().end();
}

Matrix::Container::index<Matrix::order_xy>::type::iterator
Matrix::xy_begin() {
  return container_.get<order_xy>().begin();
}

Matrix::Container::index<Matrix::order_yx>::type::iterator
Matrix::yx_begin() {
  return container_.get<order_yx>().begin();
}

Matrix::Container::index<Matrix::order_xy>::type::iterator
Matrix::xy_find(const Matrix::Index& x, const Matrix::Index& y) {
  return container_.get<order_xy>().find(std::make_tuple(x,y));
}

std::string Matrix::to_string() const {
  auto it_xy = container_.get<order_xy>().cbegin();
  auto it_yx = container_.get<order_yx>().cbegin();
  auto it_random_access = container_.get<random_access>().cbegin();
  std::string tmp = "";
  tmp += "order_xy | ";
  tmp += "order_yx | ";
  tmp += "random_access\n";
  while(it_xy != container_.get<order_xy>().cend()) {
    tmp += it_xy->to_string() + " | ";
    tmp += it_yx->to_string() + " | ";
    tmp += it_random_access->to_string() + "\n";
    it_xy++;
    it_yx++;
    it_random_access++;
  }
  return tmp;
}

void Matrix::xy_sort() {
  container_.get<random_access>().rearrange(
    container_.get<order_xy>().begin()
  );
}

void Matrix::yx_sort() {
  container_.get<random_access>().rearrange(
    container_.get<order_yx>().begin()
  );
}

void Matrix::insert(const Matrix::Index& x, const Matrix::Index& y,
  const Matrix::Value& v) {
  container_.insert(T(x,y,v));
}
/*






*/
