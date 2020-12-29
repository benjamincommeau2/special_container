#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <complex>
#include <chrono>
#include <list>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iomanip>
#include <exception>
#include <iterator>
#include <stdexcept>

class Timer {
 private:
  std::chrono::time_point<std::chrono::high_resolution_clock>
    start_ = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock>
    end_ = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double>
    duration_ = std::chrono::duration_cast
    <std::chrono::nanoseconds>(end_-start_);
 public:
  void start() {
    start_ = std::chrono::high_resolution_clock::now();
  }
  void stop(std::string statement) {
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
};

typedef Eigen::Triplet<std::complex<double>> Tri;  
typedef std::vector<Tri> TriVec;

void
build_sparse(const uint64_t& dim, const uint64_t& n, TriVec& tri_vec,
  std::default_random_engine& rand_gen, 
  std::uniform_real_distribution<double>& urd,
Eigen::SparseMatrix<std::complex<double>>& m) {
  int sys;
  tri_vec.resize(0);
  sys = system("ps -eo cmd,%mem,%cpu --sort=-%mem | head -n 2");
  std::cout << dim*dim << " " << tri_vec.size()<<  " "
    << tri_vec.capacity() << std::endl;
  try {
    tri_vec.resize(dim*dim);
  } catch(const std::exception& e) {
    std::cerr << "build_sparse->try->for->for->"
      << "tri_vec.resize(dim*dim);"
      << std::endl;
    std::cerr << e.what() << std::endl;
    throw;
  }
  sys = system("ps -eo cmd,%mem,%cpu --sort=-%mem | head -n 2");
  std::cout << dim*dim << " " << tri_vec.size()<<  " "
    << tri_vec.capacity() << std::endl;
  std::complex<double> v;
  uint64_t i = 0;
  std::cout << "start building list" << std::endl;
  for (uint64_t row = 0; row < dim; row++) {
    for (uint64_t col = 0; col < dim; col++) {
      //std::cout << i << std::endl;
      v = std::complex<double>(urd(rand_gen), urd(rand_gen));
      tri_vec[i]=(Tri(row,col,v));
      i++;
    }
  }
  sys = system("ps -eo cmd,%mem,%cpu --sort=-%mem | head -n 2");
  std::cout << dim*dim << " " << tri_vec.size()<<  " "
    << tri_vec.capacity() << std::endl;
  std::cout << "finished building vec" << std::endl;
  std::shuffle(tri_vec.begin(), tri_vec.end(), rand_gen);
  std::cout << "finished shuffling vec" << std::endl;
  try {
  m.resize(dim,dim);
  } catch(std::exception& e) {
    std::cout << e.what() << std::endl;
    std::cout << "build_sparse->m.resize(dim,dim);" << std::endl;
    throw;
  }
  auto it = tri_vec.begin();
  std::advance(it,n);
  std::cout << "finished matrix resize" << std::endl;
  m.setFromTriplets(tri_vec.begin(), it);
  std::cout << "finished setFromTriplets" << std::endl;
}

void print_sparse(const std::string& s,
  const Eigen::SparseMatrix<std::complex<double>>& mat) {
  std::cout << s << std::endl;
  for (int k=0; k<mat.outerSize(); ++k)
    for (Eigen::SparseMatrix<std::complex<double>>::InnerIterator it(mat,k);
      it; ++it) {
      std::cout << it.row() << " " << it.col() << " " << it.value() << std::endl;
    }
    std::cout << std::endl;
}

struct keep_complex_zero {
  bool operator() (const int& row, const int& col,
    const std::complex<double>& val) const {
    return false;
  }
};

void dealloc_sparse(Eigen::SparseMatrix<std::complex<double>>& m) {
  m.setZero();
  m.prune(keep_complex_zero());
  m.makeCompressed();
}


void test_trad_mult() {
  Timer stop_watch;
  std::random_device rd;
  std::default_random_engine rand_gen(rd());
  std::uniform_real_distribution<double> urd(-1, 1);
  Eigen::SparseMatrix<std::complex<double>> a;
  Eigen::SparseMatrix<std::complex<double>> b;
  Eigen::SparseMatrix<std::complex<double>> c;
  TriVec tri_vec;
  double exp = 1.5;
  uint64_t max_qubits = 12;
  uint64_t max_dim = 1<<max_qubits;
  uint64_t max_n = uint64_t(pow((max_dim),exp));
  //uint64_t max_n = uint64_t(pow(max_dim,2));
  std::cout << "start reserving" << std::endl;
  try{
    a.reserve(max_n);
    b.reserve(max_n);
    c.reserve(max_dim*max_dim);
    //tri_vec.reserve(max_dim*max_dim+1);
    tri_vec = TriVec(max_dim*max_dim+1);
    std::cout << tri_vec.size() << " " << tri_vec.capacity() << std::endl;
    tri_vec.resize(tri_vec.capacity());
    std::cout << tri_vec.size() << " " << tri_vec.capacity() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw;
  }
  std::cout << "finished reserving" << std::endl;
  std::vector<double> times_mult;
  std::vector<double> times_mult_2;
  std::vector<double> times_alloc;
  std::vector<double> times_build;
  std::vector<double> a_size;
  std::vector<double> c_size;
  uint64_t dim;
  uint64_t n;
  int sys;
  for(uint64_t q = 1; q <= (max_qubits); q++ ) {
    dim = (1<<q);
    n = uint64_t(pow((dim),exp));
    std::cout << "q=" << q << " dim=" << dim << " n=" << n << " dim^2="
      << dim*dim << std::endl;
    stop_watch.start();
    build_sparse(dim,n, tri_vec, rand_gen, urd, a);
    build_sparse(dim,n, tri_vec, rand_gen, urd, b);
    times_build.push_back(stop_watch.get_time());
    std::cout << "finished building" << std::endl;
    //c = build_sparse(s, rand_gen, urd);
    //dealloc_sparse(c);
    stop_watch.start();
    c.resize(dim,dim); //set to zero as well
    std::cout << "resize" << std::endl;
    c.setZero();
    times_alloc.push_back(stop_watch.get_time());
    std::cout << "setZero" << std::endl;
    stop_watch.start();
    c += a*b;
    times_mult.push_back(stop_watch.get_time());
    c_size.push_back(log2(double(c.nonZeros())/double(dim*dim)));
    a_size.push_back(log2(double(a.nonZeros())/double(dim*dim)));
    std::cout << "first mult" << std::endl;
    stop_watch.start();
    c += a*b;
    times_mult_2.push_back(stop_watch.get_time());
    std::cout << "second mult" << std::endl;
  }
  
  std::cout << std::fixed << std::setprecision(2)
    << std::setw(6) << times_build[0] << " "
    << std::setw(6) << times_alloc[0] << " "
    << std::setw(6) << times_mult[0] << " "
    << std::setw(6) << times_mult_2[0] << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  for (int i = 0; i < times_mult.size() - 1; i++) {
    std::cout << std::fixed << std::setprecision(2)
              << std::setw(6) << times_build[i+1] - times_build[i] << " "
              << std::setw(6) << times_alloc[i+1] - times_alloc[i] << " "
              << std::setw(6) << times_mult[i+1] - times_mult[i] << " "
              << std::setw(6) << times_mult_2[i+1] - times_mult_2[i] << " "
              << std::setw(6) << a_size[i] << " "
              << std::setw(6) << c_size[i] << " "
              << std::endl;
  }
  std::cout << "----------------------------------------------" << std::endl;
  auto end = times_alloc.size() - 1;
  std::cout << std::fixed << std::setprecision(2)
    << std::setw(6) << times_build[end] << " "
    << std::setw(6) << times_alloc[end] << " "
    << std::setw(6) << times_mult[end] << " "
    << std::setw(6) << times_mult_2[end] << " "
    << std::setw(6) << a_size[end] << " "
    << std::setw(6) << c_size[end] << " "
    << std::endl;
  //print_sparse("b=",b);
}

/*
void oper_mult(const Eigen::SparseMatrix<std::complex<double>>& a,
               const Eigen::SparseMatrix<std::complex<double>>& b,
                     Eigen::SparseMatrix<std::complex<double>>& c) {
  for (int k_a=0; k_a<a.outerSize(); ++k_a)
    for (Eigen::SparseMatrix<std::complex<double>>::InnerIterator
      it_a(a,k_a); it_a; ++it_a) {
      for (int k_b=0; k_b<b.outerSize(); ++k_b)
        for (Eigen::SparseMatrix<std::complex<double>>::InnerIterator
          it_b(b,k_b); it_b; ++it_b) {
          c.coeffRef(it_a.row()^it_b.row(), it_a.col()^it_b.col())
            += it_a.value()*it_b.value();
        }
    }
}


void test_oper_mult() {
  Timer stop_watch;
  std::random_device rd;
  std::default_random_engine rand_gen(rd());
  std::uniform_real_distribution<double> urd(-1, 1);
  auto a = build_sparse(2, rand_gen, urd);
  auto b = build_sparse(2, rand_gen, urd);
  auto c = build_sparse(2, rand_gen, urd);
  std::vector<double> times_mult;
  std::vector<double> times_mult_2;
  std::vector<double> times_alloc;
  for(int q = 1; q < 3; q++ ) {
    // allocate
    stop_watch.start();
    a = build_sparse(q, rand_gen, urd);
    b = build_sparse(q, rand_gen, urd);
    times_alloc.push_back(stop_watch.get_time());
    // first mult
    dealloc_sparse(c);
    stop_watch.start();
    oper_mult(a,b,c);
    times_mult.push_back(stop_watch.get_time());
    // second mult
    dealloc_sparse(c);
    stop_watch.start();
    oper_mult(a,b,c);
    times_mult_2.push_back(stop_watch.get_time());
  }
  for (int i = 0; i < times_mult.size() - 1; i++) {
    std::cout << times_alloc[i+1] - times_alloc[i] << " "
              << times_mult[i+1] - times_mult[i] << " "
              << times_mult_2[i+1] - times_mult_2[i] << std::endl;
  }
}

*/
int main() {
  /*
  try {
    throw std::runtime_error("oops");
  } catch(const std::exception& e) {
    std::cerr << "build_sparse->try->for->for->tri_list.push_back(T(row,col,v))"
      << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << "build_sparse->try->for->for->tri_list.push_back(T(row,col,v))"
      << std::endl;
    throw;
  }
  */
  test_trad_mult();
  return 0;
}