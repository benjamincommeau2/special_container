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
#include <list>
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
#include <iomanip>
#include <sstream>
/* //////////////////////////////////////////////////////////////
Class Definition
*/ //////////////////////////////////////////////////////////////
#ifndef MAP_HPP
#define MAP_HPP
template<class Key, class Val, class Compare = std::less<Key>>
class Map {
 public:
  typedef uint64_t Clr;
  Map() {
    hard_clear();
  }
  /* //////////////////////////////////////////////////////////////
  List of T's
  */ //////////////////////////////////////////////////////////////
  class T {
   public:
    T() {}
    T(const Key& key) {key_ = key;}
    T(const Key& key, const Val& val, const Clr& clr)
      {key_ = key; val_ = val; clr_ = clr;}
   private:
    friend class Map;
    Key key_; Val val_; Clr clr_;
  };
  typedef std::list<T> ListT;
  typedef ListT::iterator IterListT;
  typedef ListT::const_iterator ConstIterListT;

  /* //////////////////////////////////////////////////////////////
  Set of List Iterators
  */ //////////////////////////////////////////////////////////////
  struct LessIter {
    Compare compare_ = Compare();
    bool operator() (const IterListT& lhs, const IterListT& rhs) const {
      return compare_(lhs->key_, rhs->key_);
    }
  };
  typedef std::set<IterListT,LessIter> SetT;
  typedef SetT::iterator IterSetT;
  typedef std::pair<IterSetT,bool> IterBoolSetT;
  /* //////////////////////////////////////////////////////////////
  Iterator Class Definition
  */ //////////////////////////////////////////////////////////////
  template<class IterType, bool IsConst>
  class Iterator {
    friend class Map;
   private:
    /* //////////////////////////////////////////////////////////////
    Set Key
    */ //////////////////////////////////////////////////////////////
    template<class IterType_ = IterType>
      std::enable_if_t<std::is_same<IterType_, IterListT>::value, void>
      setKey(const Key& key) {(*qt_.t_).key_ = key;}
    template<class IterType_ = IterType>
      std::enable_if_t<std::is_same<IterType_, IterSetT>::value, void>
      setKey(const Key& key) {(**qt_.t_).key_ = key;}
    /* //////////////////////////////////////////////////////////////
    Set Clr
    */ //////////////////////////////////////////////////////////////
    template<class IterType_ = IterType>
      std::enable_if_t<std::is_same<IterType_, IterListT>::value, void>
      setClr(const Clr& clr) {(*(qt_.t_)).clr_ = clr;}
    template<class IterType_ = IterType>
      std::enable_if_t<std::is_same<IterType_, IterSetT>::value, void>
      setClr(const Clr& clr) {(**(qt_.t_)).clr_ = clr;}
    /* //////////////////////////////////////////////////////////////
    Iter Class Internal get/set
    */ //////////////////////////////////////////////////////////////
    IterType& It() {return qt_.t_;}
    IterType getIt() const {return qt_.t_;}
   public:
    /* //////////////////////////////////////////////////////////////
    Constant Qualifier of Iter T Val/Clr/Key
    */ //////////////////////////////////////////////////////////////
    template<class IterType_ = IterType, bool IsConst_ = IsConst>
    class QT {
     private:
      friend class Iterator;
      mutable IterType_ t_;
     public:
      /* //////////////////////////////////////////////////////////////
      Key Read-Only
      */ //////////////////////////////////////////////////////////////
      template<class IterType__ = IterType_>
        std::enable_if_t<std::is_same<IterType__, IterListT>::value, Key>
        key() const {return (*t_).key_;}
      template<class IterType__ = IterType_>
        std::enable_if_t<std::is_same<IterType__, IterSetT>::value, Key>
        key() const {return (**t_).key_;}
      /* //////////////////////////////////////////////////////////////
      Clr Read-Only
      */ //////////////////////////////////////////////////////////////
      template<class IterType__ = IterType_>
        std::enable_if_t<std::is_same<IterType__, IterListT>::value, Clr>
        clr() const {return (*t_).clr_;}
      template<class IterType__ = IterType_>
        std::enable_if_t<std::is_same<IterType__, IterSetT>::value, Clr>
        clr() const {return (**t_).clr_;}
      /* //////////////////////////////////////////////////////////////
      Val Read-Only
      */ //////////////////////////////////////////////////////////////
      template<class IterType__ = IterType_, bool IsConst__ = IsConst_>
        std::enable_if_t<std::is_same<IterType__, IterListT>::value
        && IsConst__, Val>
        val() const {return (*t_).val_;}
      template<class IterType__ = IterType_, bool IsConst__ = IsConst_>
        std::enable_if_t<std::is_same<IterType__, IterSetT>::value
        && IsConst__, Val>
        val() const {return (**t_).val_;}
      /* //////////////////////////////////////////////////////////////
      Val Read/Write
      */ //////////////////////////////////////////////////////////////
      template<class IterType__ = IterType_, bool IsConst__ = IsConst_>
        std::enable_if_t<std::is_same<IterType__, IterListT>::value
        && !IsConst__, Val&>
        val() {return (*t_).val_;}
      template<class IterType__ = IterType_, bool IsConst__ = IsConst_>
        std::enable_if_t<std::is_same<IterType__, IterSetT>::value
        && !IsConst__, Val&>
        val() {return (**t_).val_;}
    };
   private:
    bool is_valid_ = false;
    mutable QT<IterType, IsConst> qt_;
   public:
    /* //////////////////////////////////////////////////////////////
    Public Methods Definitions
    */ //////////////////////////////////////////////////////////////
    bool IsValid() {return is_valid_;}
    /* //////////////////////////////////////////////////////////////
    STL Types for Algrothim Iterator Compliance
    */ //////////////////////////////////////////////////////////////
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = QT<IterType, IsConst>;
    using reference = QT<IterType, IsConst>&;
    using pointer = QT<IterType, IsConst>*;
    /* //////////////////////////////////////////////////////////////
    Conditionally Overloaded Reference Operators
    */ //////////////////////////////////////////////////////////////
    QT<IterType,IsConst> operator*() {return qt_;}
    /* //////////////////////////////////////////////////////////////
    Conditionally Overloaded Pointer Operators
    */ //////////////////////////////////////////////////////////////
    QT<IterType,IsConst>* operator->() {return &qt_;}
    /* //////////////////////////////////////////////////////////////
    Constructors using both non/constant
    */ //////////////////////////////////////////////////////////////
    Iterator(const Iterator&) = default;
    Iterator() {}
    template<class IterType_ = IterType, class = 
      std::enable_if_t<std::is_same<IterType_, IterListT>::value>>
    Iterator(const IterListT& it) {qt_.t_ = it;}
    //Iterator(const ConstIterListT& it) {it_ = it;}
    template<class IterType_ = IterType, class = 
      std::enable_if_t<std::is_same<IterType_, IterSetT>::value>>
    Iterator(const IterSetT& it) {qt_.t_ = it;}
    template<bool IsConst_ = IsConst, class = std::enable_if_t<IsConst_>>
    Iterator(const Iterator<IterType, false>& rhs)
      {qt_.t_ = rhs.getIt(); is_valid_ = rhs.is_valid_;}
    /* //////////////////////////////////////////////////////////////
    Carefully constructed Assignments to handle both non/constant
    */ //////////////////////////////////////////////////////////////
    Iterator& operator=(const Iterator&) = default;
    template<bool IsConst_ = IsConst, class = std::enable_if_t<!IsConst_>>
    Iterator& operator= (const Iterator<IterType,false>& rhs)
    {qt_.t_ = rhs.qt_.t_; is_valid_ = rhs.is_valid_; return *this;}
    template<bool IsConst_, bool IsConst__ = IsConst, class =
       std::enable_if_t<IsConst__>>
    void operator= (const Iterator<IterType,IsConst_>& rhs) const
    {qt_.t_ = rhs.qt_.t_; is_valid_ = rhs.is_valid_; }
    /*
    template<bool IsConst_ = IsConst, class = std::enable_if_t<IsConst_>>
    Iterator& operator=(const Iterator<IterType,false>& rhs) const
    {it_ = rhs.it_; is_valid_ = rhs.is_valid_; return *this;}
    */
    /* //////////////////////////////////////////////////////////////
    Increment/Decrement Operators
    */ //////////////////////////////////////////////////////////////
    Iterator& operator++() { qt_.t_++; return *this; }
    Iterator& operator--() { qt_.t_--; return *this; }
    Iterator operator++(int)
      { Iterator tmp = *this; ++(*this); return tmp;}
    Iterator operator--(int)
      { Iterator tmp = *this; --(*this); return tmp;}
    /* //////////////////////////////////////////////////////////////
    Comparison Operators
    */ //////////////////////////////////////////////////////////////
    friend bool operator== (const Iterator& a, const Iterator& b)
      { return a.getIt() == b.getIt(); }
    friend bool operator!= (const Iterator& a, const Iterator& b)
      { return a.getIt() != b.getIt(); }
  };
  /* //////////////////////////////////////////////////////////////
  Iterator Defintions
  */ //////////////////////////////////////////////////////////////
  using ListIterator =      Iterator<IterListT,false>;
  using ConstListIterator = Iterator<IterListT,true>;
  using MapIterator =       Iterator<IterSetT,false>;
  using ConstMapIterator =  Iterator<IterSetT,true>;
  /* //////////////////////////////////////////////////////////////
  Check if Constant Iterators are valid
  */ //////////////////////////////////////////////////////////////
  /*
  static_assert(std::is_copy_constructible_v<ConstListIterator>);
  static_assert(std::is_trivially_copy_constructible_v<ConstListIterator>);
  static_assert(std::is_copy_constructible_v<ConstMapIterator>);
  static_assert(std::is_trivially_copy_constructible_v<ConstMapIterator>);
  */
  /* //////////////////////////////////////////////////////////////
  Implicit Methods Definitions without Iterators
  */ //////////////////////////////////////////////////////////////
  void setClr(const Clr& clr);
  void flatten_clear();
  Clr getClrMax() const;
  Clr getClr() const;
  void setClrMax(const Clr& x);
  void clear();
  void hard_clear();
  /* //////////////////////////////////////////////////////////////
  Implicit Methods Definitions with Iterators
  */ //////////////////////////////////////////////////////////////
  std::string to_string() const;
  MapIterator map_find(const Key& key);
  void move2Front(MapIterator& it);
  MapIterator rawInsert(const Key& key, const Val& val);
  void reInsertKey(MapIterator& it, const Key& key);
  void reInsertKey(MapIterator& it0, const Key& key0,
    MapIterator& it1, const Key& key1);
  MapIterator map_lower_bound(const Key& key);
  MapIterator map_upper_bound(const Key& key);
  ConstMapIterator map_clower_bound (const Key& key) const;
  ConstMapIterator
  map_cupper_bound (const Key& key) const;
  ConstMapIterator map_cbegin() const;
  ConstMapIterator map_cend() const;
  MapIterator map_begin(); 
  MapIterator map_end(); 
  ConstListIterator list_cbegin() const;
  ConstListIterator list_cend() const; 
  ListIterator list_begin();
  ListIterator list_end();
  MapIterator try_emplace(const Key& key, const Val& val);
 private:
  /* //////////////////////////////////////////////////////////////
  Private Variables
  */ //////////////////////////////////////////////////////////////
  Clr clr_ = 1;
  Clr clr_max_ = UINT64_MAX;
  mutable SetT set_;
  mutable ListT list_ = {T()};
  mutable ListT list_temp_ = {T()};
  /* //////////////////////////////////////////////////////////////
  Private Variable using Iterator Class
  */ //////////////////////////////////////////////////////////////
  ListIterator list_begin_;
  mutable ListIterator list_temp_it_ = ListIterator(list_temp_.begin());
  ListIterator itl_;
  MapIterator itm_;
  ConstListIterator citl_;
  ConstMapIterator citm_;
};

/* //////////////////////////////////////////////////////////////
Explicit Methods without Iterators
*/ //////////////////////////////////////////////////////////////

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
setClr(const Clr& clr) {
  clr_ = clr;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
flatten_clear() {
  auto it = list_.begin();
  while(it != list_.end()) {
    if(it->clr_ != clr_) {
      it->clr_ = 0;
    } else {
      it->clr_ = 1;
    }
    it++;
  }
  clr_ = 1;
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
Clr
Map<Key, Val, Compare>::
getClrMax() const {
  return clr_max_;
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
Clr
Map<Key, Val, Compare>::
getClr() const {
  return clr_;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
setClrMax(const Clr& x) {
  clr_max_ = x;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
clear() {
  if(clr_ < clr_max_) {
    clr_++;
  } else {
    auto it = list_.begin();
    while (it != list_.end()) {
      it->clr_ = 0;
      it++;
    }
  }
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
hard_clear() {
  clr_ = 1;
  set_.clear();
  list_.clear();
  list_.push_front(T());
  list_.begin()->clr_ = clr_;
  list_begin_.It() = list_.end();
}


/* //////////////////////////////////////////////////////////////
Explicit Methods with Iterators
*/ //////////////////////////////////////////////////////////////

template<class Key, class Val, class Compare>
std::string
Map<Key, Val, Compare>::
to_string() const {
  auto citm = map_cbegin();
  auto citl = ConstListIterator(list_.begin());
  std::ostringstream tmp ;
  tmp.precision(3);
  tmp << std::fixed << std::setprecision(2);
  tmp << "clr_=" << clr_;
  tmp << " list_.begin() -> ";
  tmp << citl->key().to_string() << " ";
  tmp << (*citl).val().to_string() << " ";
  tmp << std::to_string(citl->clr()) << "\n";
  tmp << "set_.size()=" << std::to_string(set_.size());
  tmp << " | ";
  tmp << "list_.size()=" << std::to_string(list_.size());
  tmp << "\n";
  citl++;
  while (citm != map_cend() && citl_ != list_cend()) {
    tmp << (*citm).val().to_string() << " ";
    tmp << citm->val().to_string() << " ";
    tmp << std::to_string((*citm).clr()) << " | ";
    tmp << citl->key().to_string() << " ";
    tmp << citl->val().to_string() << " ";
    tmp << std::to_string(citl->clr()) << "\n";
    citm++; citl++;
  }
  return tmp.str();
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_find(const Key& key) {
  list_temp_it_.setKey(key);
  itm_ = MapIterator(set_.find(list_temp_it_.It()));
  return itm_;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
move2Front(MapIterator& itm) {
  list_.splice(list_begin_.It(), list_, *(itm.It()));
  list_begin_ = itl_;
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
rawInsert(const Key& key, const Val& val) {
  list_.push_front(T());
  list_.begin()->clr_ = clr_;
  auto itl = list_.begin(); ++itl;
  itl->val_ = val;
  itl->key_ = key;
  itl->clr_ = clr_;
  set_.insert(itl);
  itm_ = map_find(key);
  list_begin_ = ListIterator(itl);
  std::cout << to_string() << std::endl;
  return itm_;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
reInsertKey(MapIterator& it, const Key& key) {
  auto nh = set_.extract(it.It());
  (*nh.value()).key_ = key;
  set_.insert(std::move(nh));
  it = map_find(key);
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
reInsertKey(MapIterator& it0, const Key& key0,
  MapIterator& it1, const Key& key1) {
  auto nh0 = set_.extract(it0.it_);
  auto nh1 = set_.extract(it1.it_);
  (*nh0.value()).key_ = key0;
  (*nh1.value()).key_ = key1;
  set_.insert(std::move(nh0));
  set_.insert(std::move(nh1));
  it0 = map_find(key0);
  it1 = map_find(key1);
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_lower_bound(const Key& key) {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).it_.key_ = key;
  return MapIterator(set_.lower_bound(list_temp_it_));
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_upper_bound(const Key& key) {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).key_ = key;
  return MapIterator(set_.upper_bound(list_temp_it_));
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstMapIterator
Map<Key, Val, Compare>::
map_clower_bound (const Key& key) const {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).key_ = key;
  return ConstMapIterator(set_.lower_bound(list_temp_it_));
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstMapIterator
Map<Key, Val, Compare>::
map_cupper_bound (const Key& key) const {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).key_ = key;
  return ConstMapIterator(set_.upper_bound(list_temp_it_));
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstMapIterator
Map<Key, Val, Compare>::
map_cbegin() const {
  return ConstMapIterator(set_.begin());
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstMapIterator
Map<Key, Val, Compare>::
map_cend() const {
  return ConstMapIterator(set_.end());
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_begin() {
  return MapIterator(set_.begin());
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_end() {
  return MapIterator(set_.end());
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstListIterator
Map<Key, Val, Compare>::
list_cbegin() const {
  return ConstListIterator(list_begin_);
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ConstListIterator
Map<Key, Val, Compare>::
list_cend() const {
  return ConstListIterator(list_.end());
}


template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ListIterator
Map<Key, Val, Compare>::
list_begin() {
  return ListIterator(list_begin_);
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
ListIterator
Map<Key, Val, Compare>::
list_end() {
  return ListIterator(list_.end());
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
try_emplace(const Key& key, const Val& val) {
  itm_ = map_find(key);
  if (itm_ != map_end()) {
    std::cout << "key exists" << std::endl;
    if (itm_->clr() != clr_) {
      std::cout << "key is cleared" << std::endl;
      itm_.setClr(clr_);
      itm_->val() = val;
      move2Front(itm_);
      itm_.is_valid_ = true;
    } else {
      std::cout << "key is not cleared" << std::endl;
      itm_.is_valid_ = false;
    }
  } else {
    std::cout << "key does not exist" << std::endl;
    itl_.It() = list_.end(); itl_--;
    if (itl_->clr() != clr_) {
      std::cout << "a key is reusable" << std::endl;
      itm_ = map_find(itl_->key());
      reInsertKey(itm_, key);
      itl_->val() = val;
      itl_.setClr(clr_);
      move2Front(itm_);
      itm_.is_valid_ = true;
    } else {
      std::cout << "no key is reusable" << std::endl;
      itm_ = rawInsert(key, val);
      itm_.is_valid_ = true;
    }
  }
  return  itm_;
}


/*
endend



*/

#endif
