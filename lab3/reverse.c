#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void reverseString(char* str) {
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++) {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

void reverseAndCopyFile(const char* source, const char* destination) {
    FILE* sourceFile = fopen(source, "rb");
    FILE* destinationFile = fopen(destination, "wb");

    if (sourceFile == NULL || destinationFile == NULL) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    fseek(sourceFile, 0, SEEK_END);
    long fileSize = ftell(sourceFile);
    fseek(sourceFile, -1, SEEK_END);

    for (long i = fileSize - 1; i >= 0; i--) {
        fseek(sourceFile, i, SEEK_SET);
        char c;
        fread(&c, sizeof(char), 1, sourceFile);
        fwrite(&c, sizeof(char), 1, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
}

void reverseAndCopyDirectory(const char* sourceDirectory, const char* reversedDirectory) {
    if (mkdir(reversedDirectory, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }

    DIR* dir = opendir(sourceDirectory);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
   
            char* dot = strrchr(entry->d_name, '.');
            if (dot != NULL) {
         
                char reversedFileName[1024];
                strncpy(reversedFileName, entry->d_name, dot - entry->d_name);
                reversedFileName[dot - entry->d_name] = '\0';
                reverseString(reversedFileName);

                
                char* extension = dot;

                char sourcePath[2048];
                snprintf(sourcePath, sizeof(sourcePath), "%s/%s", sourceDirectory, entry->d_name);

                char destinationPath[2048];
                snprintf(destinationPath, sizeof(destinationPath), "%s/%s%s", reversedDirectory, reversedFileName, extension);

                reverseAndCopyFile(sourcePath, destinationPath);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* sourceDirectory = argv[1];
    char reversedDirectory[1024];
    strcpy(reversedDirectory, sourceDirectory);
    reverseString(reversedDirectory);

    reverseAndCopyDirectory(sourceDirectory, reversedDirectory);

    printf("Directory copied successfully.\n");
    return 0;
}