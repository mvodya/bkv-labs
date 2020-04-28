nasm -felf64 Sin.asm
if [ $? -eq 0 ]
 then
  g++ -no-pie sin.cxx Sin.o Cartesian.o -dl -lfltk -oSin -g
  if [ $? -eq 0 ]
   then
     ./Sin
  fi
fi
