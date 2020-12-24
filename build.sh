opt="O3" # removes performance timing loops, compliant high optimization
opt="O1" # allows performance timing loops
script=testMap
> compile.errors
g++ -std=c++20 -$opt -c $script.cpp 2>&1 | tee -a compile.errors
g++ -std=c++20 -$opt -o myexec $script.o 2>&1 | tee -a compile.errors
if [[ -s compile.errors ]] ; then
  exit
else
  ./myexec
fi
