export_path:
	export PATH="$PATH:/home/oem/improve/coding/os/lab2/task1"

cq:
	gcc -c queue.c
	ar rc libqueue.a queue.o

cqe: cq
	gcc queue-example.c -L. -lqueue -o t.out -Wall -Werror -Wextra

r:
	./t.out