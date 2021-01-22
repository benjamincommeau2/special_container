#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/random_access_index.hpp>

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
      oss.precision(1);
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
  void pesAB(const Value& s, Matrix& A, Matrix& B);
  std::pair< Container::index<random_access>::type::iterator,
    Container::index<random_access>::type::iterator
  > random_access_equal_range_xy(const Index& x);
  std::pair< Container::index<random_access>::type::iterator,
    Container::index<random_access>::type::iterator
  > random_access_equal_range_yx(const Index& y);
  void add(const Index& x, const Index& y, const Value& v);
  Container::index<Matrix::order_xy>::type::iterator
    xy_begin();
  Container::index<Matrix::order_yx>::type::iterator
    yx_begin();
  Container::index<Matrix::random_access>::type::iterator
    random_access_end();
  Value getCoeff(const Index x, const Index y);
};

/* ///////////////////////////////////////////////////////////////////
explicit methods
*/ ///////////////////////////////////////////////////////////////////

Matrix::Value Matrix::getCoeff(const Matrix::Index x, const Matrix::Index y) {
  auto it = xy_find(x,y);
  return it->v_;
}

void Matrix::pesAB(const Matrix::Value& s, Matrix& A, Matrix& B) {
  container_.clear();

  A.xy_sort();
  auto xyA = A.random_access_equal_range_xy(A.xy_begin()->x_);
  auto iA = xyA.first;
  Index xA;
  Index yA;

  B.yx_sort();
  auto yxB = B.random_access_equal_range_yx(B.yx_begin()->y_);
  auto iB = yxB.first;
  Index xB;
  Index yB;

  Value v = 0;

  while(iA != A.random_access_end()) {
    xyA = A.random_access_equal_range_xy(iA->x_);
    yxB = B.random_access_equal_range_yx(B.yx_begin()->y_);
    iB = yxB.first;
    while(iB != B.random_access_end()) {
      yxB = B.random_access_equal_range_yx(iB->y_);
      v = 0;
      iA = xyA.first;
      iB = yxB.first;
      xA = iA->x_;
      yB = iB->y_;
      while(iA != xyA.second and iB != yxB.second) {
        yA = iA->y_;
        xB = iB->x_;
        if(yA == xB) {
          v += iA->v_ * iB->v_;
          iA++; iB++;
        } else if(yA < xB) { iA++; } else {iB++;}
      }
      add(xA,yB,s*v);
      if(iA != xyA.second) {iA = xyA.second;}
      if(iB != yxB.second) {iB = yxB.second;}
      std::cout << "xA,yA,xB,yB=" 
        << iA->x_ << "," 
        << iA->y_ << "," 
        << iB->x_ << "," 
        << iB->y_ << "," 
        << std::endl;
    }
  }
}

void Matrix::
add(const Matrix::Index& x, const Matrix::Index& y, const Matrix::Value& v) {
  auto it = xy_find(x,y);
  if(it != container_.get<order_xy>().end()) {
    it->v_ += v;
  } else {
    container_.insert(T(x,y,v));
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
