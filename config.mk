# sfeed version
VERSION = 0.9

# customize below to fit your system

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS =
LIBS = -lc

# debug
#CFLAGS = -fstack-protector-all -O0 -g -std=c99 -Wall -Wextra -pedantic -DVERSION=\"${VERSION}\" -D_POSIX_C_SOURCE=200809L
##CFLAGS = -O0 -g -cstd=c99 -Wall -Wextra -pedantic -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# optimized
CFLAGS = -O2 -std=c99 -DVERSION=\"${VERSION}\" -D_POSIX_C_SOURCE=200809L
LDFLAGS = -s ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}

# compiler and linker
CC = cc
