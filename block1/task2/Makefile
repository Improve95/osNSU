compile:
	gcc -c src/mythread/mythread.c
	ar rc libmythread.a mythread.o
	gcc src/main.c -L. -lmythread -o main.out