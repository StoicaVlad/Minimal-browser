build: main.c
	gcc -std=c99 -Wall utils.h fct.h utils.c functii.c main.c -o tema2
clean: 
	rm -f tema2
