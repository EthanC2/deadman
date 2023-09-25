CC=gcc
WFLAGS=-Wall -Wextra -Werror
BINARY=deadman

deadman: main.c errors.h
	${CC} ${WFLAGS} main.c -o ${BINARY}

check:
	${CC} ${WFLAGS} main.c -fsyntax-only

clean:
	rm -v ${BINARY}
