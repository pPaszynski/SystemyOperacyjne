#!/bin/bash
LIMIT_U=$(ulimit -u)
N_PROC=$(ps ux | wc -l)
LIMIT=$((LIMIT_U - N_PROC))
echo "$LIMIT" > "limit";
