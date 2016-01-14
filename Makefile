INCS =
LIBS =

# flags
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

CC = cc

BIN = opacity invert flip
SRC = ${BIN:=.c}

all: ${BIN}

.c: farbfeld.h
	@echo CC $<
	@${CC} -o $@ ${CFLAGS} ${LIBS} ${LDFLAGS} $<

clean:
	@rm -f ${BIN}

.PHONY: all clean dist install uninstall
