p2:	p2.c menu.c menu.h utility.c utility.h directory.h directory.c file.h file.c
	gcc -std=c11 -Wall -Wextra -Wpedantic -o p2 p2.c menu.c utility.c directory.c file.c


clean:
	rm p2 

