i=1
while [ $i -le $1 ]
  do
    ./konsument $i
    i=$(($i+1))
  done
