PROG = eserv

CC = gcc

CFLAGS = -O0 -Wall -Wextra -Werror \
	 -D_POSIX_SOURCE -D_BSD_SOURCE -g \
	 -std=gnu99
# -D_DEBUG
LDFLAGS = -lpthread

OBJS = \
	libeserv/mempool.o \
	libeserv/hash.o \
	libeserv/io.o \
	libeserv/http.o \
	libeserv/request.o \
	libeserv/analysis.o \
	libeserv/entry.o \
	libeserv/cgi.o \
	libeserv/misc.o \
	main.o \
	\
	cgi_custom.o

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(CFLAGS) $(LDFLAGS) $(OBJS)

clean:
	rm -f $(PROG) $(OBJS)
