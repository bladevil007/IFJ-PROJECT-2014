#!/bin/bash

#----PREKLAD
clear
rm -f *.o
make

#-----

#TESTS

#test1
./project  files/test1.f > files/test1.out

OK=$(diff files/test1.k files/test1.out)

if [ "$OK" = "" ]; then
echo -e '\E[32;1m'"----------------> SUCCESS TEST1"; tput sgr0
else
diff files/test1.k files/test1.out
fi


#test2
./project  files/test2.f > files/test2.out

OK=$(diff files/test2.k files/test2.out)

if [ "$OK" = "" ]; then
echo -e '\E[32;1m'"----------------> SUCCESS TEST2"; tput sgr0
else
diff files/test2.k files/test2.out
fi

#test3
./project  files/test3.f > files/test3.out

OK=$(diff files/test3.k files/test3.out)

if [ "$OK" = "" ]; then
echo -e '\E[32;1m'"----------------> SUCCESS TEST3"; tput sgr0
else
diff files/test3.k files/test3.out
fi


#test4
./project  files/test4.f > files/test4.out

OK=$(diff files/test4.k files/test4.out)

if [ "$OK" = "" ]; then
echo -e '\E[32;1m'"----------------> SUCCESS TEST4"; tput sgr0
else
diff files/test4.k files/test4.out
fi


#test5
./project  files/test5.f > files/test5.out

OK=$(diff files/test5.k files/test5.out)

if [ "$OK" = "" ]; then
echo -e '\E[32;1m'"----------------> SUCCESS TEST5"; tput sgr0
else
diff files/test5.k files/test5.out
fi

rm -f *.o












