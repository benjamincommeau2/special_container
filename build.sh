clear
#gcc opt="O3" removes performance timing loops, compliant high optimization
#gcc opt="O1" allows performance timing loops
script=testMatrix
error=compile.cerr
run=run.txt
execo=myexec.o
> $error
> $run
opt="-O3 -std=c++20 -fdiagnostics-show-template-tree -fmessage-length=80"
g++ $opt -c $script.cpp 2>&1 | tee -a $error
g++ $opt -o $execo $script.o 2>&1 | tee -a $error
if [[ -s $error ]] ; then
  vi $error
  exit
else
  ./$execo 2>&1 | tee -a $run
  if [[ -s $run ]] ; then
    vi $run
    exit
   fi
fi
