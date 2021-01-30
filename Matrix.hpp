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
      oss.precision(1);
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
   mutable Index i_;
    S() {}
    S(T* pt, const Index& i) {pt_ = pt; i_ = i;}
    Index getX() const {return pt_->x_;}
    Index getY() const {return pt_->y_;}
    Clear getC() const {return pt_->c_;}
    std::string to_string() const {
      std::ostringstream oss;
      oss << i_ << " " << pt_->to_string();
      return oss.str();
    }
  };

  /* ///////////////////////////////////////////////////////////////////
  Tags
  */ ///////////////////////////////////////////////////////////////////
  struct XY {};
  struct YX {};
  struct CXY {};
  /* ///////////////////////////////////////////////////////////////////
  Type for Deque Container

1. Insertion
b. deque- All iterators and references are invalidated, unless the inserted member is at an end (front or back) of the deque (in which case all iterators are invalidated, but references to elements are unaffected). 
2. Erasure 
b. deque- All iterators and references are invalidated unless the erased members are at an end (front or back) of the deque (in which case only iterators and references to the erased members are invalidated) 
3. Resizing 
a. vector, deque, and list- As per insert/erase.

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
  template <class ForwardIt, class Compare> void
    special_quicksort(ForwardIt first, ForwardIt last, Compare compare);
  template<class ForwardIt, class UnaryPredicate> ForwardIt
    special_partition(ForwardIt first, ForwardIt last, UnaryPredicate p);
  template<class ForwardIt1, class ForwardIt2> constexpr void
    special_iter_swap(ForwardIt1 a, ForwardIt2 b);
  void sort_xy();
};

void Matrix::sort_xy() {
  struct LessXY {
     bool operator() (const T& lhs, const T& rhs) const {
       return (lhs.x_ != rhs.x_) ? (lhs.x_ < rhs.x_) : (lhs.y_ < rhs.y_);
     }
  };
  LessXY lessxy = LessXY();
  //special_quicksort(sequence_.begin(), sequence_.end(), lessxy);
  //auto it = container_.get<XY>().find(0,0);
  //std::cout << std::distance(sequence_.begin(),*((*it).pt_)) << std::endl;
  std::sort(sequence_.begin(), sequence_.end(), lessxy);
}

template <class ForwardIt, class Compare>
void
Matrix::special_quicksort(ForwardIt first, ForwardIt last, Compare compare)
{
   if(first == last) return;
   auto pivot = *std::next(first, std::distance(first,last)/2);
   ForwardIt middle1 = std::partition(first, last,
     [pivot,compare](const auto& em){ return compare(em,pivot); });
   ForwardIt middle2 = std::partition(middle1, last,
     [pivot,compare](const auto& em){ return !compare(em,pivot); });
   special_quicksort(first, middle1, compare);
   special_quicksort(middle2, last, compare);
}

template<class ForwardIt, class UnaryPredicate>
ForwardIt
Matrix::special_partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
  first = std::find_if_not(first, last, p);
  if (first == last) return first;
    for (ForwardIt i = std::next(first); i != last; ++i) {
      if (p(*i)) {
        special_iter_swap(i, first);
        ++first;
      }
    }
    return first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr void
Matrix::special_iter_swap(ForwardIt1 a, ForwardIt2 b) 
  // constexpr since C++20
{
   std::swap(*a, *b);
}

void Matrix::insert(const Matrix::Index& x, const Matrix::Index& y,
  const Matrix::Value& v) {
  sequence_.push_back(T(x,y,c_,v));
  container_.insert(S(&sequence_[sequence_.size()-1],sequence_.size()-1));
  //auto it = container_.begin();
  //(*it).pt_ = &sequence_[0];
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
    oss << it_xy->to_string() << " | ";
    oss << it_yx->to_string() << " | ";
    oss << it_cxy->to_string() << "\n";
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



void Matrix::yx_sort() {
  container_.get<random_access>().rearrange(
    container_.get<order_yx>().begin()
  );
}

*/ ///////////////////////////////////////////////////////////////////
/*






*/
