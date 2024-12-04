#include "../headers/argsChecker.h"

void checkCountArguments(int argc) {
    if (argc != 3) {
        perror("Wrong count of arguments");
        exit(EXIT_FAILURE);
    }
}

void checkIfValidParsedInt(int number) {
    if (number <= 0) {
        fprintf(stderr, "Incorrect input\n");
        exit(EXIT_FAILURE);
    }
}