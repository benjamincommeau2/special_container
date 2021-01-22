clear
opt="O1" # allows performance timing loops
opt="O3" # removes performance timing loops, compliant high optimization
script=test_eigen
nodebug="-DNDEBUG"
nodebug=""
eigen=~/code/eigen3.3.9/eigen-3.3.9
echo $eigen
cerr=compile.errors.cerr
rerr=runs.errors.cerr
> $cerr
> $rerr
g++ -static-libgcc -I $eigen $nodebug -L/usr/lib/gcc/x86_64-linux-gnu/10 -lstdc++ -std=c++20 -$opt -c $script.cpp 2>&1 | tee -a $cerr
g++ -shared-libgcc -I $eigen $nodebug -L/usr/lib/gcc/x86_64-linux-gnu/10 -lstdc++ -std=c++20 -$opt -o myexec.o $script.o 2>&1 | tee -a $cerr
if [[ -s $cerr ]] ; then
  vi $cerr
  exit
else
  ./myexec.o 2>&1 | tee -a $rerr
  vi $rerr
  exit
fi
