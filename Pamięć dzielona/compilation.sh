gcc -c bibl.c
gcc -c semafory.c
gcc -c gl.c
gcc bibl.o gl.o semafory.o -o prod
gcc con.c -c
gcc con.o bibl.o semafory.o  -o con
