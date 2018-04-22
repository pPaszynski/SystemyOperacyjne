#!/bin/bash  

`ulimit -u > limity.txt`
`ps -U $USER | wc -l >> limity.txt`

