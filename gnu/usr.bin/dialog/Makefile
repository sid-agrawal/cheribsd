# $FreeBSD$

.include <src.opts.mk>

DIALOG=	${SRCTOP}/contrib/dialog
PROG=   dialog

LIBADD=	dialog tinfow ncursesw m
CFLAGS+= -I${.CURDIR} -I${DIALOG}
.PATH:	${DIALOG}

.include <bsd.prog.mk>
