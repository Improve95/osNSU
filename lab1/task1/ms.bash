rm t.out
clear

# gcc t1.c -o t.out -lpthread -Werror -Wextra
# gcc t11.c -o t.out -lpthread -Werror -Wextra
# gcc t12.c -o t.out -lpthread -Werror -Wextra
# gcc t12d.c -o t.out -lpthread -Werror -Wextra
# gcc t12e.c -o t.out -lpthread -Werror -Wextra
gcc t13.c -o t.out -lpthread -Werror -Wextra

./t.out