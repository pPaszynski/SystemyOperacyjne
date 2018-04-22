i=1
while [ $i -le $1 ]
  do
    ./producent $i
    i=$(($i+1))
  done
