#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

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
  using Clear = uint64_t;
  /* ///////////////////////////////////////////////////////////////////
  type T to store in container
  */ ///////////////////////////////////////////////////////////////////
  class T {
    friend class Matrix;
   private:
    Index x_; Index y_; Clear c_;
   public:
    Value v_;
    T() {}
    T(const Index& x, const Index& y, const Clear& c, const Value& v) :
      x_(x), y_(y), c_(c), v_(v) {}
    std::string to_string() const {
      std::ostringstream oss;
      oss.precision(2);
      oss << std::fixed ;
      oss << "(" << x_ << "," << y_ << "," << c_ << ")";
      oss << " ";
      oss << "(" << v_.real() << "," << v_.imag() << ")";
      return oss.str();
    }
  };
  struct S{
    friend class Matrix;
   public:
   mutable T* pt_;
    S() {}
    S(T* pt) {pt_ = pt;}
    Index getX() const {return pt_->x_;}
    Index getY() const {return pt_->y_;}
    Clear getC() const {return pt_->c_;}
  };
  /* ///////////////////////////////////////////////////////////////////
  Tags
  */ ///////////////////////////////////////////////////////////////////
  struct XY {};
  struct YX {};
  struct CXY {};
  /* ///////////////////////////////////////////////////////////////////
  Type for Deque Container
  */ ///////////////////////////////////////////////////////////////////
  using Sequence = std::deque<T>;
  Sequence sequence_;
  /* ///////////////////////////////////////////////////////////////////
  Type for Boost's Multi-Index Container
  */ ///////////////////////////////////////////////////////////////////
  using Container = boost::multi_index_container<
    S, // the data type stored
    boost::multi_index::indexed_by<
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<XY>,
        boost::multi_index::composite_key<
          S,
          boost::multi_index::const_mem_fun<S, Index, &S::getX>,
          boost::multi_index::const_mem_fun<S, Index, &S::getY>
        >,
        boost::multi_index::composite_key_compare<
          std::less<Index>,
          std::less<Index>
        >
      >
      , 
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<YX>,
        boost::multi_index::composite_key<
          S,
          boost::multi_index::const_mem_fun<S, Index, &S::getY>,
          boost::multi_index::const_mem_fun<S, Index, &S::getX>
        >,
        boost::multi_index::composite_key_compare<
          std::less<Index>,
          std::less<Index>
        >
      >
      , 
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<CXY>,
        boost::multi_index::composite_key<
          S,
          boost::multi_index::const_mem_fun<S, Clear, &S::getC>,
          boost::multi_index::const_mem_fun<S, Index, &S::getX>,
          boost::multi_index::const_mem_fun<S, Index, &S::getY>
        >,
        boost::multi_index::composite_key_compare<
          std::less<Clear>,
          std::less<Index>,
          std::less<Index>
        >
      >
    >
  >;
  /* ///////////////////////////////////////////////////////////////////
  variables
  */ ///////////////////////////////////////////////////////////////////
  Container container_;
  Index index_max_ = UINT32_MAX;
  Clear clear_max_ = UINT64_MAX;
  Clear c_ = 1;
  Index index_last_= 0;
  /* ///////////////////////////////////////////////////////////////////
  implicit methods
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
  void reserve(Index m);
  */ ///////////////////////////////////////////////////////////////////
  void insert(const Matrix::Index& x, const Matrix::Index& y,
    const Matrix::Value& v);
  void hard_clear();
  std::string to_string() const;
};

void Matrix::insert(const Matrix::Index& x, const Matrix::Index& y,
  const Matrix::Value& v) {
  sequence_.push_back(T(x,y,c_,v));
  container_.insert(&sequence_[sequence_.size()-1]);
  auto it = container_.begin();
  (*it).pt_ = &sequence_[0];
}

void Matrix::hard_clear() {
  container_.clear();
  sequence_.clear();
  index_last_ = 0;
}

std::string Matrix::to_string() const {
  auto it_seq = sequence_.begin();
  auto it_xy = container_.get<XY>().begin();
  auto it_yx = container_.get<YX>().begin();
  auto it_cxy = container_.get<CXY>().begin();
  std::ostringstream oss;
  oss << "seq(" << sequence_.size() << ") | ";
  oss << "xy(" << container_.size() << ") | ";
  oss << "yx(" << container_.size() << ") | ";
  oss << "cxy(" << container_.size() << ")\n";
  while(it_seq != sequence_.end()) {
    oss << it_seq->to_string() << " | ";
    oss << (*it_xy).pt_->to_string() << " | ";
    oss << (*it_yx).pt_->to_string() << " | ";
    oss << (*it_cxy).pt_->to_string() << "\n";
    it_seq++; it_xy++; it_yx++; it_cxy++;
  }
  return oss.str();
}
/* ///////////////////////////////////////////////////////////////////
explicit methods
void Matrix::reserve(Index m) {
  container_.get<random_access>().reserve(m);
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

*/ ///////////////////////////////////////////////////////////////////
/*






*/
