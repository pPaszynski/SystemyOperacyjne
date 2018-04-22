#!/bin/bash			//wskazuje na interpreter który powinien zostać użyty do wykonania skryptu
LIMIT_U=$(ulimit -u)		// u -max user processes
N_PROC=$(ps ux | wc -l)		//-u - wyświetla uruchomione procesy danego użytkownika  u dlugi opis | il.wierszy
LIMIT=$((LIMIT_U - N_PROC))	// 
echo "$LIMIT" > "limit";
