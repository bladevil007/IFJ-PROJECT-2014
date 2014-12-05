#testovaci skript
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
  for file in tests/*.txt				#pre kazdy .txt testovaci subor v priecinku tests 
  do
	nazov=$(basename "$file" .txt)		#osekanie nazvu testovacieho suboru
	
	if [ -f "tests/$nazov.log" ]; then	#ak sa v priecinku nachadza .log subor s rovnakym nazvom, treba testovat aj vystup
    	expected=$(head -n 1 $file | awk '{print $2}')		#testuje sa zaroven aj navratove hodnota ako vzdy
		./project $file > tests/"$nazov".output				#vystup sa zapise do suboru s rovnakym nazvom s priponou .output
		ret=$?
	
		diff "tests/$nazov.log" tests/"$nazov".output > tests/"$nazov".diff 	#testuje sa prikazom diff .log a .output subor, 
																				#vystup operacie - pripadny rozdiel sa zapisuje do suboru .diff
		diffret=$?						#navratova hodnota diff, ak je nula tak su subory rovnake - vystup je v poriadku
		if [[ ($diffret -eq 0) && ($ret -eq $expected) ]]; then					#ak je vystup aj navratova hodnota v poriadku	
			echo -e "Test file:	 $nazov	$ret/$expected	\e[32m OK	DIFF OK \e[39m"	

		elif [[ ($diffret -ne 0) ]]; then					#ak je chyba vo vystupe
			echo -e "Test file:	 $nazov	$ret/$expected	\e[31m NOK	DIFF NOK \e[39m"
			cat tests/"$nazov".diff		#vypis na stdin aky je diff
		else							#vystup je v poriadku, navratova hodnota je zla
			echo -e "Test file:	 $nazov	$ret/$expected	\e[31m NOK	\e[32mDIFF OK \e[39m"
		fi
		rm  tests/"$nazov".output		#vymazat docastne subory
		rm  tests/"$nazov".diff


    	else							#v priecinku sa nenachadza .log subor s rovnakym nazvom, netreba testovat vystup
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
