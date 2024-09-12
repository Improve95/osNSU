rm t1
clear

# gcc t1.c -o t1 -lpthread -Werror -Wextra
# gcc t11.c -o t1 -lpthread -Werror -Wextra
# gcc t12.c -o t1 -lpthread -Werror -Wextra
gcc t12d.c -o t1 -lpthread -Werror -Wextra

./t1