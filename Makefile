INCS =
LIBS =

# flags
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS   = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
LDFLAGS  = ${LIBS}

CC = cc

BIN = invert
SRC = ${BIN:=.c}

all: ${BIN}

.c:
	@echo CC $<
	@${CC} -o $@ ${CFLAGS} ${LIBS} ${LDFLAGS} $<

clean:
	@rm -f ${BIN}

.PHONY: all clean dist install uninstall
