
APPNAME=test
LIBPATH=`pwd`/../lib/ 

CFLAGS+=-I${LIBPATH}
LDFLAGS+=-L${LIBPATH}
LDFLAGS+=-static

CC=gcc

all:
	${CC} ${CFLAGS} main.c -o ${APPNAME} ${LDFLAGS} -lmycrypto
clean:
	rm -f ${APPNAME}

run:
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:LIBPATH 
	./${APPNAME}


