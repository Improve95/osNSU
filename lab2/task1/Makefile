export_path:
	export PATH="$PATH:/home/opp112/osNSU/lab2/task1"

cq:
	gcc -c queue.c -g
	ar rc libqueue.a queue.o

cqe: cq
	gcc queue-example.c -g -L. -lqueue -lpthread -o t.out -Wall -Werror -Wextra

cqt: cq
	gcc queue-threads.c -g -L. -lqueue -lpthread -o t.out -Wall -Werror -Wextra
	sudo chmod 777 t.out
r:
	./t.out