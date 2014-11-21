make
compile=false
if [ $? -eq 0 ]
then
  echo ' '
  echo '...---Preklad vporadku---...'
  compile=true
else
  echo '  '
  echo '...---Chyba prekladu---...'
fi

if [ $compile = true ]
then
  for file in files/*.f;do
  echo " "
  echo "===Testuji $file==="
  ret=$(./project $file | grep -Eo '[0-9]{1,3}')
  if [ $ret -eq 1 ]
  then 
    echo '--Lexikalni analyza ---FAIL---'
  elif [ $ret -eq 2 ]
  then
    printf "Lexikalni analyza \t--- OK ---\n"
    printf "Syntakticka analyza \t---FAIL---\n"
  else
    echo '--Lexikalni analyza ---OK---'
    echo '--Syntakticka analyza ---OK---'
  fi
done
fi
