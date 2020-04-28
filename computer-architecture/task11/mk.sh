nasm -felf64 Sin.asm
if [ $? -eq 0 ]
 then
  g++  sin.cxx Sin.o Cartesian.o -dl -lfltk -lX11 -oSin
  if [ $? -eq 0 ]
   then
     ./Sin
  fi
fi
