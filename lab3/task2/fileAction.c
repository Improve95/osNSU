#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void createDirectory(const char* path) {
    if (mkdir(path, 0700) != 0) {
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }
}

void listDirectory(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void removeDirectory(char *filePath) {
    DIR *d;
    struct dirent *dir;
    d = opendir(filePath);
    if (!d) { 
        perror("cannot open dir"); 
        exit(EXIT_FAILURE);
    }

    struct stat buf;
    int x;
    while((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
            char newFilePath[1024];
            snprintf(newFilePath, sizeof newFilePath, "%s/%s", filePath, dir->d_name);
            x = stat(newFilePath, &buf);
            if (x != 0) { 
                perror("cannot check file type"); 
                exit(EXIT_FAILURE);
            }


            if (S_ISDIR(buf.st_mode)) {
                removeDirectory(newFilePath);
            } else {
                if (unlink(newFilePath) != 0) {
                    perror("cannot delete file");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    if (rmdir(filePath) != 0) {
        perror("cannot delete dir");
        exit(EXIT_FAILURE);
    }
}

void createFile(const char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

void showFileContent(const char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char c;
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    fclose(file);
}

void removeFile(const char* path) {
    if (unlink(path) != 0) {
        perror("Error removing file");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "too few arguments\n");
        exit(EXIT_FAILURE);
    }

    char* operation = argv[0];
    char* path = argv[1];

    if (strcmp(operation, "./createDirectory") == 0) {
        createDirectory(path);
    } else if (strcmp(operation, "./listDirectory") == 0) {
        listDirectory(path);
    } else if (strcmp(operation, "./removeDirectory") == 0) {
        removeDirectory(path);
    } else if (strcmp(operation, "./createFile") == 0) {
        createFile(path);
    } else if (strcmp(operation, "./showFileContent") == 0) {
        showFileContent(path);
    } else if (strcmp(operation, "./removeFile") == 0) {
        removeFile(path);
    } else {
        fprintf(stderr, "Unknown operation");
        exit(EXIT_FAILURE);
    }

    return 0;
}