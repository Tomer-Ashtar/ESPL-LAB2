all: myshell looper mypipe

myshell: myshell.o LineParser.o
	gcc -g -m32 -Wall -o myshell myshell.o LineParser.o

myshell.o: myshell.c
	gcc -g -m32 -Wall -c -nostdlib -fno-stack-protector -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -m32 -Wall -c -nostdlib -fno-stack-protector -o LineParser.o LineParser.c

looper: looper.o
	gcc -g -m32 -Wall -o looper looper.o

looper.o: looper.c
	gcc -g -m32 -Wall -c -nostdlib -fno-stack-protector -o looper.o looper.c

mypipe: mypipe.o
	gcc -g -m32 -Wall -o mypipe mypipe.o

mypipe.o: mypipe.c
	gcc -g -m32 -Wall -c -nostdlib -fno-stack-protector -o mypipe.o mypipe.c

PHONY: clean
clean:
	rm -f *.o myshell
	rm -f *.o mypipe