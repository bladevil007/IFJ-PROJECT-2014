make clean
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

echo "format testu navratova hodnota/ocekavana hodnota"
then
  for file in tests/*.txt
  do
	nazov=$(basename "$file" .txt)
	
	if [ -f "tests/$nazov.log" ]; then
    		expected=$(head -n 1 $file | awk '{print $2}')
		./project $file > tests/"$nazov".output
		ret=$?
	
		diff "tests/$nazov.log" tests/"$nazov".output > tests/"$nazov".diff 
		diffret=$?
		if [[ ($diffret -eq 0) && ($ret -eq $expected) ]]; then		
			echo -e "Test file:	 $nazov	$ret/$expected	\e[32m OK	DIFF OK \e[39m"	

		elif [[ ($diffret -ne 0) ]]; then
			echo -e "Test file:	 $nazov	$ret/$expected	\e[31m NOK	DIFF NOK \e[39m"
			cat tests/"$nazov".diff
		else
			echo -e "Test file:	 $nazov	$ret/$expected	\e[31m NOK	\e[32mDIFF OK \e[39m"
		fi
		rm  tests/"$nazov".output
		rm  tests/"$nazov".diff


    	else
    		expected=$(head -n 1 $file | awk '{print $2}')
    		./project $file 
    		ret=$?
    		if [ "$ret" = "$expected" ]; then
        		echo -e "Test file:	 $nazov	$ret/$expected	\e[32m OK \e[39m"
      		else
        		echo -e "Test file:	 $nazov	$ret/$expected	\e[31m NOK \e[39m"
    		fi  	
	fi

  done
fi
