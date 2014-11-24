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
  for file in files/*
  do
    expected=$(head -n 1 $file | awk '{print $2}')
    ./project $file 
    ret=$?
    if [ "$ret" = "$expected" ]; then
        echo -e "Test file: $file  	$ret	\e[32m OK \e[39m"
      else
        echo -e "Test file: $file	$ret	\e[31m NOK \e[39m"
    fi  
  done
fi
