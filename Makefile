
bgrep: bgrep.c
	gcc -O3 -Wall -Wextra -o bgrep bgrep.c

indent:
	gnuindent -kr -nut bgrep.c

clean:
	rm -f *~
	rm -f bgrep
