
all: bgrep strlen stats
CC=gcc
CFLAGS=-O3 -Wall -Werror
bgrep: bgrep.c
	$(CC) $(CFLAGS) -o bgrep bgrep.c

strlen: strlen.c
	$(CC) $(CFLAGS) -o strlen strlen.c

stats: stats.c
	$(CC) $(CFLAGS) -o stats stats.c


clean:
	rm -f bgrep stats strlen
