# Makefile for Crack of Doom

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= proj.c timer.c KBD.c mouse.c graphics.c vbe.c video_gr.c bitmap.c game.c handler.c
CFLAGS= -Wall

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
