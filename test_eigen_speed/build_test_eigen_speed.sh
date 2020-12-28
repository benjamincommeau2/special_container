opt="O1" # allows performance timing loops
opt="O3" # removes performance timing loops, compliant high optimization
script=test_eigen
nodebug="-DNDEBUG"
nodebug=""
> compile.errors.txt
g++ $nodebug -L/usr/lib/gcc/x86_64-linux-gnu/10 -lstdc++ -std=c++20 -$opt -c $script.cpp 2>&1 | tee -a compile.errors.txt
g++ $nodebug -L/usr/lib/gcc/x86_64-linux-gnu/10 -lstdc++ -std=c++20 -$opt -o myexec.o $script.o 2>&1 | tee -a compile.errors.txt
if [[ -s compile.errors.txt ]] ; then
  exit
else
  ./myexec.o
fi
