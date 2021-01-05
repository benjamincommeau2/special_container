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

#ifndef MAP_HPP
#define MAP_HPP
template<class Key, class Val, class Compare = std::less<Key>>
class Map {
 public:
  typedef uint64_t Clr;

  /* //////////////////////////////////////////////////////////////
  List of T's
  */ //////////////////////////////////////////////////////////////

  struct T {
    Key key_; Val val_; Clr clr_;
    T() {}
    T(const Key& key) {key_ = key;}
    T(const Key& key, const Val& val, const Clr& clr) {
      key_ = key; val_ = val; clr_ = clr;
    }
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
  Iterators
  */ //////////////////////////////////////////////////////////////

  template<bool IsConst>
  class ListIterBool {
   public:
    /* //////////////////////////////////////////////////////////////
    STL Types for Algrothim Iterator Compliance
    */ //////////////////////////////////////////////////////////////
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = IterListT;
    using reference = T&;
    /* //////////////////////////////////////////////////////////////
    Carefully constructed copy and assignments operators to handle both
    iterators and constant iterators
    */ //////////////////////////////////////////////////////////////
    ListIterBool(const ListIterBool&) = default;
    ListIterBool& operator=(const ListIterBool&) = default;
    ListIterBool(const IterListT& it) {ptr_ = it;}
    ListIterBool(const ConstIterListT& it) {ptr_ = it;}
    template<bool WasConst, class = std::enable_if_t<IsConst && !WasConst>>
    ListIterBool(const ListIterBool<WasConst>& rhs) : ptr_(rhs.ptr_) {}
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    ListIterBool& operator=(const ListIterBool<false>& rhs)
      {ptr_ = rhs.ptr_; return *this;}
    /* //////////////////////////////////////////////////////////////
    Constant Enforcement Operators
    */ //////////////////////////////////////////////////////////////
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    reference operator*() const { return *ptr_; }
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    pointer operator->() const { return ptr_; }
    /* //////////////////////////////////////////////////////////////
    Non-Constant Enforcement Operators
    */ //////////////////////////////////////////////////////////////
    template<bool WasConst = IsConst, class = std::enable_if_t<!WasConst>>
    reference operator*() { return *ptr_; }
    template<bool WasConst = IsConst, class = std::enable_if_t<!WasConst>>
    pointer operator->() { return ptr_; }
    /* //////////////////////////////////////////////////////////////
    Increment Operators
    */ //////////////////////////////////////////////////////////////
    ListIterBool& operator++() { ptr_++; return *this; }
    ListIterBool& operator--() { ptr_--; return *this; }
    ListIterBool operator++(int)
      { ListIterBool tmp = *this; ++(*this); return tmp;}
    ListIterBool operator--(int)
      { ListIterBool tmp = *this; --(*this); return tmp;}
    /* //////////////////////////////////////////////////////////////
    Comparison Operators
    */ //////////////////////////////////////////////////////////////
    friend bool operator== (const ListIterBool& a, const ListIterBool& b)
      { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const ListIterBool& a, const ListIterBool& b)
      { return a.ptr_ != b.ptr_; };
   private:
    pointer ptr_;
  };
  
  using ListIterator = ListIterBool<false>;
  using ConstListIterator = ListIterBool<true>;

  static_assert(std::is_copy_constructible_v<ConstListIterator>);
  static_assert(std::is_trivially_copy_constructible_v<ConstListIterator>);

  template<bool IsConst>
  class MapIterBool {
   private:
    IterSetT ptr_;
   public:
    /* //////////////////////////////////////////////////////////////
    STL Types for Algrothim Iterator Compliance
    */ //////////////////////////////////////////////////////////////
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = IterSetT;
    using reference = T&;
    /* //////////////////////////////////////////////////////////////
    Carefully constructed copy and assignments operators to handle both
    iterators and constant iterators
    */ //////////////////////////////////////////////////////////////
    MapIterBool(const MapIterBool&) = default;
    MapIterBool& operator=(const MapIterBool&) = default;
    MapIterBool(const IterSetT& it) {ptr_ = it;}
    template<bool WasConst, class = std::enable_if_t<IsConst && !WasConst>>
    MapIterBool(const MapIterBool<WasConst>& rhs) : ptr_(rhs.ptr_) {}
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    MapIterBool& operator=(const MapIterBool<false>& rhs)
      {ptr_ = rhs.ptr_; return *this;}
    /* //////////////////////////////////////////////////////////////
    Constant Enforcement Operators
    */ //////////////////////////////////////////////////////////////
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    reference operator*() const {std::cout << "CEO" << std::endl; return **ptr_;}
    template<bool WasConst = IsConst, class = std::enable_if_t<WasConst>>
    IterListT operator->() const { return *ptr_; }
    /* //////////////////////////////////////////////////////////////
    Non-Constant Enforcement Operators
    */ //////////////////////////////////////////////////////////////
    template<bool WasConst = IsConst, class = std::enable_if_t<!WasConst>>
    reference operator*() {std::cout << "NCEO" << std::endl; return **ptr_; }
    template<bool WasConst = IsConst, class = std::enable_if_t<!WasConst>>
    IterListT operator->() { return *ptr_; }
    /* //////////////////////////////////////////////////////////////
    Increment Operators
    */ //////////////////////////////////////////////////////////////
    MapIterBool& operator++() { ptr_++; return *this; }
    MapIterBool& operator--() { ptr_--; return *this; }
    MapIterBool operator++(int)
      { MapIterBool tmp = *this; ++(*this); return tmp;}
    MapIterBool operator--(int)
      { MapIterBool tmp = *this; --(*this); return tmp;}
    /* //////////////////////////////////////////////////////////////
    Comparison Operators
    */ //////////////////////////////////////////////////////////////
    friend bool operator== (const MapIterBool& a, const MapIterBool& b)
      { return a.ptr_ == b.ptr_; };
    friend bool operator!= (const MapIterBool& a, const MapIterBool& b)
      { return a.ptr_ != b.ptr_; };
    
  };
  
  using MapIterator = MapIterBool<false>;
  using ConstMapIterator = MapIterBool<true>;

  static_assert(std::is_copy_constructible_v<ConstMapIterator>);
  static_assert(std::is_trivially_copy_constructible_v<ConstMapIterator>);

  

  /* //////////////////////////////////////////////////////////////
  Methods
  */ //////////////////////////////////////////////////////////////

  IterBoolSetT try_emplace(const Key& key, const Val& val);
  void setIterList(IterListT& it, const Key& key);
  IterSetT begin();
  IterSetT end();
  std::string to_string();
  void clear();
  void hard_clear();
  void setClrMax(const Clr& x);
  Clr getClr();
  Clr getClrMax();
  void setClr(const Clr& clr);
  IterSetT find(const Key& key);
  void flatten_clear();
  void moveToFront(IterListT& it);
  ListIterator list_begin();
  ListIterator list_end();
  MapIterator map_begin();
  MapIterator map_end();
  ConstMapIterator map_cbegin() const;
  ConstMapIterator map_cend() const;
  ConstMapIterator map_clower_bound(const Key& key) const;
  ConstMapIterator map_cupper_bound(const Key& key) const;
  MapIterator map_lower_bound(const Key& key);
  MapIterator map_upper_bound(const Key& key);
  MapIterator map_find(const Key& key);
  void reInsertKey(MapIterator& it, const Key& key);
  void reInsertKey(MapIterator& it0, const Key& key0,
    MapIterator& it1, const Key& key1);
  void move2Front(MapIterator& it);

  /* //////////////////////////////////////////////////////////////
  Public Variables
  */ //////////////////////////////////////////////////////////////

  SetT set_;
  ListT list_;
  mutable ListT list_temp_ = {T()};
  mutable IterListT list_temp_it_ = list_temp_.begin();

 private:
  Clr clr_ = 1;
  Clr clr_max_ = UINT64_MAX; //0xFFFFFFFFFFFFFFFF;
  IterBoolSetT iter_bool_set_;
  //IterListT iter_list_ = list_.begin();
  //IterListT iter_list_clr_end_ = list_.end();
  IterListT list_it_ = list_.begin();
  IterListT list_begin_ = list_.begin();

};

/* //////////////////////////////////////////////////////////////
Explicit Methods
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
move2Front(MapIterator& it) {
  list_.splice(list_begin_, list_, *it.m_ptr);
  list_begin_ = *it.m_ptr;
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
reInsertKey(MapIterator& it0, const Key& key0,
  MapIterator& it1, const Key& key1) {
  auto nh0 = set_.extract(it0.m_ptr);
  auto nh1 = set_.extract(it1.m_ptr);
  (*nh0.value()).key_ = key0;
  (*nh1.value()).key_ = key1;
  set_.insert(std::move(nh0));
  set_.insert(std::move(nh1));
  it0 = map_find(key0);
  it1 = map_find(key1);
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
reInsertKey(MapIterator& it, const Key& key) {
  auto nh = set_.extract(it.m_ptr);
  (*nh.value()).key_ = key;
  set_.insert(std::move(nh));
  it = map_find(key);
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_find(const Key& key) {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).key_ = key;
  return MapIterator(set_.find(list_temp_it_));
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
MapIterator
Map<Key, Val, Compare>::
map_lower_bound(const Key& key) {
  list_temp_it_ = list_temp_.begin();
  (*list_temp_it_).key_ = key;
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
void
Map<Key, Val, Compare>::
moveToFront(IterListT& it) {
  list_.splice(list_begin_, list_, it);
  list_begin_ = it;
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
IterSetT
Map<Key, Val, Compare>::
find(const Key& key) {
  list_it_ = list_.begin();
  setIterList(list_it_, key);
  return set_.find(list_it_);
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
Clr
Map<Key, Val, Compare>::
getClrMax() {
  return clr_max_;
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
Clr
Map<Key, Val, Compare>::
getClr() {
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
  set_.clear();
  list_.clear();
}

template<class Key, class Val, class Compare>
std::string
Map<Key, Val, Compare>::
to_string() {
  auto itm = set_.begin();
  auto itl = list_.begin();
  std::stringstream tmp ;
  tmp << std::fixed << std::setprecision(2);
  tmp << "clr_=" << clr_;
  tmp << " list_.begin() -> ";
  tmp << itl->key_.to_string() << " ";
  tmp << itl->val_.to_string() << " ";
  tmp << std::to_string(itl->clr_) << "\n";
  tmp << "set_.size()=" << std::to_string(set_.size());
  tmp << " | ";
  tmp << "list_.size()=" << std::to_string(list_.size());
  tmp << "\n";
  itl++;
  while (itm != set_.end() && itl != list_.end()) {
    tmp << (**itm).key_.to_string() << " ";
    tmp << (**itm).val_.to_string() << " ";
    tmp << std::to_string((**itm).clr_) << " | ";
    tmp << itl->key_.to_string() << " ";
    tmp << itl->val_.to_string() << " ";
    tmp << std::to_string(itl->clr_) << "\n";
    itm++; itl++;
  }
  return tmp.str();
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
IterSetT
Map<Key, Val, Compare>::
begin() {
  return set_.begin();
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
IterSetT
Map<Key, Val, Compare>::
end() {
  return set_.end();
}

template<class Key, class Val, class Compare>
void
Map<Key, Val, Compare>::
setIterList(IterListT& it, const Key& key) {
  if (list_.size() != 0) {
    if(it != list_.end()) { } else { it--; }
    it->key_ = key;
  } else {
    list_.push_back(T(key));
    it = list_.begin();
    it -> clr_ = clr_;
  }
}

template<class Key, class Val, class Compare>
Map<Key, Val, Compare>::
IterBoolSetT
Map<Key, Val, Compare>::
try_emplace(const Key& key, const Val& val) {
  if (list_.size() > 0) {
  } else {
    list_.push_back(T());
    list_.begin() -> clr_ = clr_;
    list_begin_ = list_.end();
  }
  list_it_ = list_.begin();
  list_it_ -> key_ = key; 
  iter_bool_set_.first = set_.find(list_it_);
  if (iter_bool_set_.first != set_.end()) {
    //std::cout << "key exists" << std::endl;
    if ((**iter_bool_set_.first).clr_ != clr_) {
      // std::cout << "key is cleared" << std::endl;
      (**iter_bool_set_.first).clr_ = clr_;
      (**iter_bool_set_.first).val_ = val;
      list_.splice(list_begin_, list_, *iter_bool_set_.first);
      iter_bool_set_.second = true;
      list_begin_ = *iter_bool_set_.first;
    } else {
     // std::cout << "key is not cleared" << std::endl;
      iter_bool_set_.second = false;
    }
  } else {
    //std::cout << "key does not exist" << std::endl;
    list_it_ = list_.end(); list_it_--;
    if (list_it_ -> clr_ != clr_) {
     // std::cout << "a key is reusable" << std::endl;
      auto nh = set_.extract(list_it_);
      (*nh.value()).key_ = key;
      (*nh.value()).val_= val;
      (*nh.value()).clr_ = clr_;
      set_.insert(std::move(nh));
      iter_bool_set_.first = set_.find(list_it_);
      iter_bool_set_.second = true;
      list_.splice(list_begin_, list_, list_it_);
      list_begin_ = list_it_;
    } else {
      //std::cout << "no key is reusable" << std::endl;
      list_it_ = list_.begin();
      list_it_ -> val_ = val;
      list_it_ -> clr_ = clr_;
      list_it_ -> key_ = key;
      iter_bool_set_ = set_.insert(list_it_);
      list_.push_front(T());
      list_.begin() -> clr_ = clr_;
      list_begin_ = list_it_;
      //std::cout << "val=" << val.v_.real() << std::endl;
      //std::cout << to_string() << std::endl;
    }
  }
  return  iter_bool_set_;
}

#endif
