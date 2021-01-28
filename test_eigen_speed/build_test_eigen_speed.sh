clear
script=test_eigen
eigen=~/code/eigen3.3.9/eigen-3.3.9
cerr=compile.errors.cerr
rerr=runs.errors.txt
> $cerr
> $rerr

opt="-std=c++20 -O3 -DNDEBUG -finline-functions -Wno-inline -ftemplate-depth-128 -I $eigen -fdiagnostics-show-template-tree -fmessage-length=80"
g++ $opt -c $script.cpp 2>&1 | tee -a $cerr
g++ $opt -o myexec.o $script.o 2>&1 | tee -a $cerr
if [[ -s $cerr ]] ; then
  vi $cerr
  exit
else
  ./myexec.o 2>&1 | tee -a $rerr
  vi $rerr
  exit
fi
