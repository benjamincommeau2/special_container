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
#include "../Matrix.hpp"

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

void printMEM() {
  auto sys = system("ps -eo cmd,%mem,%cpu --sort=-%mem | head -n 2 | tail -n 1");
}

void print_sparse(const std::string& s,
  const Eigen::SparseMatrix<std::complex<double>>& mat, Matrix& A) {
  std::cout << s << std::endl;
  for (int k=0; k<mat.outerSize(); ++k)
    for (Eigen::SparseMatrix<std::complex<double>>::InnerIterator it(mat,k);
      it; ++it) {
      std::cout << it.col() << " " << it.row() << " " << it.value() << std::endl;
    }
    std::cout << std::endl;
  std::cout << A.map_.to_string() << std::endl;
}

void
build_sparse(const uint64_t& dim, const uint64_t& n, TriVec& tri_vec,
  std::default_random_engine& rand_gen, 
  std::uniform_real_distribution<double>& urd,
  Eigen::SparseMatrix<std::complex<double>>& m,
Matrix& M) {
  int sys;
  tri_vec.resize(0);
  try {
    tri_vec.resize(dim*dim);
  } catch(const std::exception& e) {
    std::cerr << "build_sparse->try->for->for->"
      << "tri_vec.resize(dim*dim);"
      << std::endl;
    std::cerr << e.what() << std::endl;
    throw;
  }
  std::complex<double> v;
  uint64_t i = 0;
  for (uint64_t row = 0; row < dim; row++) {
    for (uint64_t col = 0; col < dim; col++) {
      v = std::complex<double>(urd(rand_gen), urd(rand_gen));
      tri_vec[i]=(Tri(row,col,v));
      i++;
    }
  }
  auto it = tri_vec.begin();
  std::advance(it,dim*dim);
  std::shuffle(tri_vec.begin(), it, rand_gen);
  try {
  m.resize(dim,dim);
  } catch(std::exception& e) {
    std::cout << e.what() << std::endl;
    std::cout << "build_sparse->m.resize(dim,dim);" << std::endl;
    throw;
  }
  it = tri_vec.begin();
  std::advance(it,n);
  m.setFromTriplets(tri_vec.begin(), it);
  M.map_.clear();
  for(uint32_t i = 0; i < n; i++) {
    M.add(tri_vec[i].col(),tri_vec[i].row(),tri_vec[i].value());
  }
  //print_sparse("m=",m,M);
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

void eigen_oper_mult(const Eigen::SparseMatrix<std::complex<double>>& a,
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
  //std::cout << c.nonZeros() << std::endl;
}

double compare_objects(Eigen::SparseMatrix<std::complex<double>>& c, Matrix& C) {
  double v=0;
  for (int k_c=0; k_c<c.outerSize(); ++k_c)
    for (Eigen::SparseMatrix<std::complex<double>>::InnerIterator
      it_c(c,k_c); it_c; ++it_c) {
      v += std::abs(C.getCoeff(it_c.col(),it_c.row()) - it_c.value());
    }
  return v;
}

void test_trad_mult() {
  Timer stop_watch;
  std::random_device rd;
  std::default_random_engine rand_gen(rd());
  std::uniform_real_distribution<double> urd(-1, 1);
  Eigen::SparseMatrix<std::complex<double>> a;
  Eigen::SparseMatrix<std::complex<double>> b;
  Eigen::SparseMatrix<std::complex<double>> c;
  Matrix A;
  Matrix B;
  Matrix C;
  TriVec tri_vec;
  double exp = 2;
  uint64_t max_qubits = 10;
  uint64_t max_dim = 1<<max_qubits;
  uint64_t max_n = uint64_t(pow((max_dim),exp));
  try{
    a.reserve(max_n);
    b.reserve(max_n);
    c.reserve(max_dim*max_dim);
    tri_vec = TriVec(max_dim*max_dim);
    tri_vec.resize(tri_vec.capacity());
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    throw;
  }
  std::vector<double> times_mult;
  std::vector<double> times_mult_2;
  std::vector<double> times_alloc;
  std::vector<double> times_build;
  std::vector<double> a_size;
  std::vector<double> c_size;
  std::vector<double> times_sort;
  std::vector<double> times_allocM;
  std::vector<double> times_multM;
  std::vector<double> times_mult_2M;
  uint64_t dim;
  uint64_t n;
  for(uint64_t q = 1; q <= (max_qubits); q++ ) {
    dim = (1<<q);
    n = uint64_t(pow((dim),exp));
    std::cout << "q=" << q << " dim=" << dim << " n=" << n << " dim^2="
      << dim*dim << std::endl;
    stop_watch.start();
    //std::cout << "start build A" << std::endl;
    build_sparse(dim,n, tri_vec, rand_gen, urd, a, A);
    //std::cout << "start build B" << std::endl;
    build_sparse(dim,n, tri_vec, rand_gen, urd, b, B);
    times_build.push_back(stop_watch.get_time());
    //////////////////////////////////////////
    stop_watch.start();
    c.resize(dim,dim); //set to zero as well
    times_alloc.push_back(stop_watch.get_time());
    stop_watch.start();
    c.setZero();
    c += a*b;
    times_mult.push_back(stop_watch.get_time());
    c_size.push_back(log2(double(c.nonZeros())/double(dim*dim)));
    a_size.push_back(log2(double(a.nonZeros())/double(dim*dim)));
    stop_watch.start();
    c.setZero();
    c += a*b;
    times_mult_2.push_back(stop_watch.get_time());
    /////////////////////////////////////////////
    stop_watch.start();
    A.map_.sort_list(); B.map_.sort_list();
    times_sort.push_back(stop_watch.get_time());
    stop_watch.start();
    C.map_.clear();
    times_allocM.push_back(stop_watch.get_time());
    stop_watch.start();
    //std::cout << "start transpose" << std::endl;
    A.transpose_emplace();
    //std::cout << "start pesABt" << std::endl;
    //C.pesABt(1,B,A);
    times_multM.push_back(stop_watch.get_time());
    stop_watch.start();
    C.map_.clear();
    //std::cout << "start pesABt 2nd" << std::endl;
    //C.pesABt(1,B,A);
    times_mult_2M.push_back(stop_watch.get_time());
    ////////////////////////////////////////////////
    printMEM();
    std::cout << "compare_objects(c,C)=" << compare_objects(c,C) << std::endl;
  }
  
  std::cout << std::fixed << std::setprecision(3)
    << std::setw(6) << times_build[0] << " "
    << std::setw(6) << times_alloc[0] << " "
    << std::setw(6) << times_mult[0] << " "
    << std::setw(6) << times_mult_2[0] << " "
    << std::setw(6) << " "
    << std::setw(6) << " "
    << std::setw(6) << times_sort[0] << " "
    << std::setw(6) << times_allocM[0] << " "
    << std::setw(6) << times_multM[0] << " "
    << std::setw(6) << times_mult_2M[0] << " "
    << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  for (int i = 0; i < times_mult.size() - 1; i++) {
    std::cout << std::fixed << std::setprecision(3)
              << std::setw(6) << times_build[i+1] - times_build[i] << " "
              << std::setw(6) << times_alloc[i+1] - times_alloc[i] << " "
              << std::setw(6) << times_mult[i+1] - times_mult[i] << " "
              << std::setw(6) << times_mult_2[i+1] - times_mult_2[i] << " "
              << std::setw(6) << a_size[i] << " "
              << std::setw(6) << c_size[i] << " "
              << std::setw(6) << times_sort[i+1]-times_sort[i] << " "
              << std::setw(6) << times_allocM[i+1]-times_allocM[i] << " "
              << std::setw(6) << times_multM[i+1]-times_multM[i] << " "
              << std::setw(6) << times_mult_2M[i+1]-times_mult_2M[i] << " "
              << std::endl;
  }
  std::cout << "----------------------------------------------" << std::endl;
  auto end = times_alloc.size() - 1;
  std::cout << std::fixed << std::setprecision(3)
    << std::setw(6) << times_build[end] << " "
    << std::setw(6) << times_alloc[end] << " "
    << std::setw(6) << times_mult[end] << " "
    << std::setw(6) << times_mult_2[end] << " "
    << std::setw(6) << a_size[end] << " "
    << std::setw(6) << c_size[end] << " "
    << std::setw(6) << times_sort[end] << " "
    << std::setw(6) << times_allocM[end] << " "
    << std::setw(6) << times_multM[end] << " "
    << std::setw(6) << times_mult_2M[end] << " "
    << std::endl;
  //print_sparse("b=",b);
}



/*


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
