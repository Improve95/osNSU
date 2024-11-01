export_path:
	export PATH="$PATH:/home/opp112/osNSU/lab2/task1"

cq:
	gcc -c queue.c
	ar rc libqueue.a queue.o

cqe: cq
	gcc queue-example.c -L. -lqueue -lpthread -o t.out -g -Wall -Werror -Wextra

cqt: cq
	gcc queue-threads.c -L. -lqueue -lpthread -o t.out -g -Wall -Werror -Wextra

r:
	./t.out