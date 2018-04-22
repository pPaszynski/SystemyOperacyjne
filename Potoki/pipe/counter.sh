#!/bin/bash 

lZnakowSkonsumowanych=`cat ./konsumenci/* | wc -c`
lZnakowWyprodukowanych=`cat ./producenci/* | wc -c`

echo "PRODUCENCI: $lZnakowWyprodukowanych"
echo "KONSUMENCI: $lZnakowSkonsumowanych"
