export_path:
	export PATH="$PATH:/home/opp112/osNSU/lab2/task1"

cq:
	gcc -c queue.c
	ar rc libqueue.a queue.o

cqp: cq
	gcc t3.c -L. -lqueue -lpthread -o t.out

r:
	./t.out