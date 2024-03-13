#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void createDirectory(const char* path) {
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }
    printf("Directory created successfully: %s\n", path);
}

void listDirectory(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    printf("Contents of directory %s:\n", path);

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}


void removeDirectory(const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char entryPath[1024];
            snprintf(entryPath, sizeof(entryPath), "%s/%s", path, entry->d_name);

            if (entry->d_type == DT_DIR) {
                removeDirectory(entryPath);
            }
            else {
                if (unlink(entryPath) == -1) {
                    perror("Error removing file");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    closedir(dir);

    if (rmdir(path) == -1) {
        perror("Error removing directory");
        exit(EXIT_FAILURE);
    }

    printf("Directory removed successfully: %s\n", path);
}

void createFile(const char* path) {
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    printf("File created successfully: %s\n", path);
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
    printf("\n");
}

void removeFile(const char* path) {
    if (unlink(path) == -1) {
        perror("Error removing file");
        exit(EXIT_FAILURE);
    }

    printf("File removed successfully: %s\n", path);
}

void createSymbolicLink(const char* target, const char* linkName) {
    if (symlink(target, linkName) == -1) {
        perror("Error creating symbolic link");
        exit(EXIT_FAILURE);
    }
    printf("Symbolic link created successfully: %s -> %s\n", linkName, target);
}

void showSymbolicLinkContent(const char* path) {
    char target[1024];
    ssize_t bytesRead = readlink(path, target, sizeof(target) - 1);
    if (bytesRead == -1) {
        perror("Error reading symbolic link");
        exit(EXIT_FAILURE);
    }
    target[bytesRead] = '\0';
    printf("Symbolic link content: %s -> %s\n", path, target);
}

void showLinkedFileContent(const char* path) {
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
    printf("\n");
}

void removeSymbolicLink(const char* path) {
    if (unlink(path) == -1) {
        perror("Error removing symbolic link");
        exit(EXIT_FAILURE);
    }
    printf("Symbolic link removed successfully: %s\n", path);
}


void createHardLink(const char* target, const char* linkName) {
    if (link(target, linkName) == -1) {
        perror("Error creating hard link");
        exit(EXIT_FAILURE);
    }
    printf("Hard link created successfully: %s -> %s\n", linkName, target);
}

void removeHardLink(const char* path) {
    if (unlink(path) == -1) {
        perror("Error removing hard link");
        exit(EXIT_FAILURE);
    }
    printf("Hard link removed successfully: %s\n", path);
}

void showFilePermissionsAndLinks(const char* path) {
    struct stat fileStat;
    if (lstat(path, &fileStat) == -1) {
        perror("Error getting file information");
        exit(EXIT_FAILURE);
    }
    printf("File permissions: %o\n", fileStat.st_mode & 0777);
    printf("Number of hard links: %lu\n", (unsigned long)fileStat.st_nlink);
}

void changeFilePermissions(const char* path, mode_t permissions) {
    if (chmod(path, permissions) == -1) {
        perror("Error changing file permissions");
        exit(EXIT_FAILURE);
    }
    printf("File permissions changed successfully: %s\n", path);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <operation> <path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* operation = argv[1];
    char* path = argv[2];

    if (strcmp(operation, "createDirectory") == 0) {
        createDirectory(path);
    }
    else if (strcmp(operation, "listDirectory") == 0) {
        listDirectory(path);
    }
    else if (strcmp(operation, "removeDirectory") == 0) {
        removeDirectory(path);
    }
    else if (strcmp(operation, "createFile") == 0) {
        createFile(path);
    }
    else if (strcmp(operation, "showFileContent") == 0) {
        showFileContent(path);
    }
    else if (strcmp(operation, "removeFile") == 0) {
        removeFile(path);
    }
    else if (strcmp(operation, "createSymbolicLink") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s createSymbolicLink <target> <linkName>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        char* target = argv[3];
        createSymbolicLink(target, path);
    }
    else if (strcmp(operation, "showSymbolicLinkContent") == 0) {
        showSymbolicLinkContent(path);
    }
    else if (strcmp(operation, "showLinkedFileContent") == 0) {
        showLinkedFileContent(path);
    }
    else if (strcmp(operation, "removeSymbolicLink") == 0) {
        removeSymbolicLink(path);
    }
    else if (strcmp(operation, "createHardLink") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s createHardLink <target> <linkName>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        char* target = argv[3];
        createHardLink(target, path);
    }
    else if (strcmp(operation, "removeHardLink") == 0) {
        removeHardLink(path);
    }
    else if (strcmp(operation, "showFilePermissionsAndLinks") == 0) {
        showFilePermissionsAndLinks(path);
    }
    else if (strcmp(operation, "changeFilePermissions") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s changeFilePermissions <path> <permissions>\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        mode_t permissions = strtol(argv[3], NULL, 8);
        changeFilePermissions(path, permissions);
    }
    else {
        fprintf(stderr, "Unknown operation: %s\n", operation);
        exit(EXIT_FAILURE);
    }

    return 0;
}