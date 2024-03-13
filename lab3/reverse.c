#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void reverseString(char *string) {
    int len = strlen(string);
    for (int i = 0; i < len / 2; i++) {
        char c = string[i];
        string[i] = string[len - 1 - i];
        string[len - 1 - i] = c;
    }
}

void removeDir(char *filePath) {
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
                removeDir(newFilePath);
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

void copyDir(char *filePath) {
    DIR *d;
    struct dirent *dir;
    d = opendir(filePath);
    if (!d) {
        perror("cannot open dir");
        exit(EXIT_FAILURE);
    }

    reverseString(filePath);
    if (mkdir(filePath, 0700) != 0) { 
        perror("cannot make dir");
        exit(EXIT_FAILURE);
    }

    struct stat buf;
    int x;
    while ((dir = readdir(d)) != NULL) {
        char newFilePath[1024];
        reverseString(filePath);
        snprintf(newFilePath, sizeof newFilePath, "%s/%s", filePath, dir->d_name);
        reverseString(filePath);

        x = stat(newFilePath, &buf);
        if (x != 0) { 
            perror("cannot check file type"); 
            exit(EXIT_FAILURE); 
        }

        if (!S_ISREG(buf.st_mode)) continue;

        FILE *fileIn = fopen(newFilePath, "rb");
        if (fileIn == NULL) { 
            perror("cannot open input file");
            exit(EXIT_FAILURE);
        }

        reverseString(dir->d_name);
        snprintf(newFilePath, sizeof newFilePath, "%s/%s", filePath, dir->d_name);

        FILE *fileOut = fopen(newFilePath, "wb");
        if (fileOut == NULL) {
            perror("cannot open output file");
            exit(EXIT_FAILURE);
        }

        fseek(fileIn, 0, SEEK_END);
        size_t fileSize = ftell(fileIn);
        fseek(fileIn, -1, SEEK_END);
        char c;
        for (size_t i = 0; i < fileSize; i++) {
            fread(&c, 1, 1, fileIn);
            fwrite(&c, 1, 1, fileOut);
            fseek(fileIn, (i + 2) * (-1), SEEK_END);
        }

        fclose(fileIn);
        fclose(fileOut);
    }

    closedir(d);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("too few arguments");
        exit(EXIT_FAILURE);
    }

    if (access(argv[1], F_OK)) {
        reverseString(argv[1]);
        removeDir(argv[1]);
        reverseString(argv[1]);
    }
    copyDir(argv[1]);
    
    return 0;
}
