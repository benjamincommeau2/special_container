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
#include <cstdio>
#include <exception>
#include <memory>
#include <chrono>
#include <cmath>
#include <string>
#include <sstream>
#include <map>
/*
#include <algorithm>
#include <iostream>
#include <set>
#include <list>
#include <complex>
#include <random>
#include "Matrix.hpp"
*/

class Timer {
 public:
  struct Vector {
   std::vector<double> times;
   Vector() {}
  };
 private:
  std::map<std::string, Vector> data_;
  std::chrono::time_point<std::chrono::high_resolution_clock>
    start_ = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock>
    end_ = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double>
    duration_ = std::chrono::duration_cast
    <std::chrono::nanoseconds>(end_-start_);
 public:
  Timer(std::vector<std::string> names) {
    for(int i = 0; i < names.size(); i++) {
      data_.try_emplace(names[i],Vector());
    }
  }
  void start() {
    start_ = std::chrono::high_resolution_clock::now();
  }
  void print_stop(std::string statement) {
    end_ = std::chrono::high_resolution_clock::now();
    duration_ = std::chrono::duration_cast
      <std::chrono::nanoseconds>(end_-start_);
    std::cout << log2(duration_.count()) << " "+statement << std::endl;
    start_ = std::chrono::high_resolution_clock::now();
  }
  double get_time() {
    end_ = std::chrono::high_resolution_clock::now();
    duration_ = std::chrono::duration_cast
      <std::chrono::nanoseconds>(end_-start_);
    return log2(duration_.count());
  }
  void set_time(std::string name) {
    double time = get_time();
    auto it = data_.find(name);
    if(it != data_.end()) {
      it->second.times.push_back(time);
    }
  }
  void fill(std::ostringstream& oss,const int& width) {
    oss.width(width);
    oss.fill('-');
    oss << "-" << std::endl;
    oss.fill(' ');
  }
  std::string get_data_as_table_string() {
    int width = 6;
    // get max size
    int max_size = 0;
    for(auto it : data_) {
      if(max_size < it.second.times.size()) {
        max_size = it.second.times.size();
      }
    }
    // string names as columns to numerical identifiers
    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed;
    int index = 0;
    for(auto it : data_) {
      oss << "(" << index <<  ") " << it.first << " | ";
      index++;
    }
    oss << std::endl;
    fill(oss,(width+3)*data_.size());
    // numerical identifiers
    index = 0;
    for(auto it : data_) {
      oss.width(width);
      oss << "("+std::to_string(index)+")" << " | ";
      index++;
    }
    oss << std::endl;
    fill(oss,(width+3)*data_.size());
    // first times
    for(auto it : data_) {
      if(it.second.times.size() > 0) {
        oss.width(width);
        oss << it.second.times[0] << " | ";
      } else {
        oss.width(width);
        oss << "n/a ";
      }
    }
    oss << std::endl;
    fill(oss,(width+3)*data_.size());
    // time differences
    for(int l = 0; l < max_size - 1; l++) {
      for(auto it : data_) {
       if(it.second.times.size() > l + 1) {
         oss.width(width);
         oss << it.second.times[l+1] - it.second.times[l] << " | ";
       } else {
         oss.width(width);
         oss << "n/a ";
       }
      }
      if(l < max_size - 1) {
        oss << std::endl;
      }
    }
    if(max_size < 2) {
      oss << std::endl;
    }
    fill(oss,(width+3)*data_.size());
    // average difference
    for(auto it : data_) {
      if(it.second.times.size() > 1) {
        oss.width(width);
        oss << (it.second.times[it.second.times.size()-1]
          - it.second.times[0])
          / (it.second.times.size()-1) << " | ";
      } else {
        oss.width(width);
        oss << "n/a";
      }
    }
    oss << std::endl; 
    fill(oss,(width+3)*data_.size());
    // last times
    for(auto it : data_) {
      if(it.second.times.size() > 0) {
        oss.width(width);
        oss << it.second.times[it.second.times.size()-1] << " | ";
      } else {
        oss.width(width);
        oss << "n/a ";
      }
    }
    oss << std::endl;
    return oss.str();
  }
};
