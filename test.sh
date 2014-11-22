#!/bin/bash
make
compile=false
if [ $? -eq 0 ]
then
  echo ' '
  echo -e '\e[32m ...---Preklad v poradku---... \e[39m'
  echo
  compile=true
else
  echo '  '
  echo -e '\e[31m ...---Chyba prekladu---... \e[39m'
  echo
fi

if [ $compile = true ]
then
  for file in files/*.f 
  do
    expected=$(head -n 1 $file | awk '{print $2}')
    ./project $file 
    if [ "$?" = "$expected" ]; then
        echo -e "Test file: $file  	$?	\e[32m OK \e[39m"
      else
        echo -e "Test file: $file	$?	\e[31m NOK \e[39m"
    fi  
  done
fi
