all: sqlcmd udfexec
sqlcmd: sqlcmd.c
	gcc -Wall -o sqlcmd sqlcmd.c -lmysqlclient
udfexec: udfexec.c
	gcc -Wall -I/usr/include/mysql -shared -o udfexec.so -fPIC udfexec.c

clean:
	rm -f sqlcmd udfexec.so
