/*
Author : A.Goktug
Date : 03/09/2024

------------------
EnsureDirectories(const char *path) Ensures that all directories in the specified path exist by creating them if they are not already present It parses the path and creates each directory along the path

CreateFile(const char *filename, enum FileType type, int permissions) Creates a file or directory with the specified name, type, and permissions It also ensures that any necessary directories in the path are created

ReadFile(const char *filename) Reads and prints the details of the specified file It displays information such as type, size, creation time, last access time, and content (if it's a regular file)

WriteFile(const char *filename, const char *content) Writes content to a regular file It updates the file's content and size and sets the last access time

DeleteFile(const char *filename) Deletes the specified file If the file is found, it marks it as unused

RenameFile(const char *old_name, const char *new_name) Renames an existing file or directory It ensures that the new name does not conflict with existing files

UpdatePermissions(const char *filename, int new_permissions) Updates the permissions of a file

ListFiles() Lists all files and directories on the RAM disk, displaying their details including name, type, permissions, size, creation time, and last access time

DeleteDirectoryContents(const char *dirPath) Recursively deletes all contents within the specified directory, including subdirectories

main() Demonstrates the use of the above functions by creating files and directories, writing to a file, reading files, updating permissions, deleting files, renaming directories, and listing files on the RAM disk
------------------
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILES 10
#define MAX_FILENAME_LENGTH 256
#define MAX_FILE_CONTENT_LENGTH 1024

enum FileType { REGULAR, DIRECTORY };

struct RAMFile {
    char filename[MAX_FILENAME_LENGTH];
    enum FileType type;
    int is_used;
    int permissions;
    time_t creation_time;
    time_t last_access_time;
    size_t size;
    char content[MAX_FILE_CONTENT_LENGTH];
};

struct RAMFile ramDisk[MAX_FILES];

int EnsureDirectories(const char *path) {
    char tempPath[MAX_FILENAME_LENGTH];
    strncpy(tempPath, path, MAX_FILENAME_LENGTH);

    char *token = strtok(tempPath, "/");
    char currentPath[MAX_FILENAME_LENGTH] = "";

    while (token != NULL) {
        strcat(currentPath, token);
        int found = 0;
        for (int i = 0; i < MAX_FILES; i++) {
            if (ramDisk[i].is_used && ramDisk[i].type == DIRECTORY &&
                strcmp(ramDisk[i].filename, currentPath) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (strlen(currentPath) < MAX_FILENAME_LENGTH) {
                CreateFile(currentPath, DIRECTORY, 0);
            }
        }
        strcat(currentPath, "/");
        token = strtok(NULL, "/");
    }

    return 0;
}

int CreateFile(const char *filename, enum FileType type, int permissions) {
    if (strlen(filename) >= MAX_FILENAME_LENGTH) {
        printf("Filename is too long!\n");
        return -1;
    }

    char path[MAX_FILENAME_LENGTH];
    char fileNameOnly[MAX_FILENAME_LENGTH];
    strncpy(path, filename, MAX_FILENAME_LENGTH);

    char *lastSlash = strrchr(path, '/');
    if (lastSlash) {
        strncpy(fileNameOnly, lastSlash + 1, MAX_FILENAME_LENGTH);
        *lastSlash = '\0';
    } else {
        strncpy(fileNameOnly, filename, MAX_FILENAME_LENGTH);
        path[0] = '\0';
    }

    if (strlen(path) > 0) {
        EnsureDirectories(path);
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (!ramDisk[i].is_used) {
            strncpy(ramDisk[i].filename, filename, MAX_FILENAME_LENGTH);
            ramDisk[i].type = type;
            ramDisk[i].is_used = 1;
            ramDisk[i].permissions = permissions;
            ramDisk[i].creation_time = time(NULL);
            ramDisk[i].last_access_time = time(NULL);
            ramDisk[i].size = (type == REGULAR) ? 0 : 0;
            ramDisk[i].content[0] = '\0';
            printf("File '%s' created (%s).\n", filename, type == REGULAR ? "Regular" : "Directory");
            return 0;
        }
    }
    printf("No space left on RAM disk!\n");
    return -1;
}

int ReadFile(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && strcmp(ramDisk[i].filename, filename) == 0) {
            ramDisk[i].last_access_time = time(NULL);
            printf("File '%s' exists (%s). Size: %zu bytes. Creation time: %s. Last access time: %s.\n",
                   filename,
                   ramDisk[i].type == REGULAR ? "Regular" : "Directory",
                   ramDisk[i].size,
                   ctime(&ramDisk[i].creation_time),
                   ctime(&ramDisk[i].last_access_time));
            if (ramDisk[i].type == REGULAR) {
                printf("Content: %s\n", ramDisk[i].content);
            }
            return 0;
        }
    }
    printf("File not found!\n");
    return -1;
}

int WriteFile(const char *filename, const char *content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && strcmp(ramDisk[i].filename, filename) == 0) {
            if (ramDisk[i].type != REGULAR) {
                printf("Cannot modify directory content!\n");
                return -1;
            }
            if (strlen(content) >= MAX_FILE_CONTENT_LENGTH) {
                printf("Content too long!\n");
                return -1;
            }
            strncpy(ramDisk[i].content, content, MAX_FILE_CONTENT_LENGTH);
            ramDisk[i].size = strlen(content);
            ramDisk[i].last_access_time = time(NULL);
            printf("File '%s' content updated.\n", filename);
            return 0;
        }
    }
    printf("File not found!\n");
    return -1;
}

int DeleteFile(const char *filename) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && strcmp(ramDisk[i].filename, filename) == 0) {
            ramDisk[i].is_used = 0;
            printf("File '%s' deleted.\n", filename);
            return 0;
        }
    }
    printf("File not found!\n");
    return -1;
}

int RenameFile(const char *old_name, const char *new_name) {
    if (strlen(new_name) >= MAX_FILENAME_LENGTH) {
        printf("New filename is too long!\n");
        return -1;
    }

    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && strcmp(ramDisk[i].filename, old_name) == 0) {
            for (int j = 0; j < MAX_FILES; j++) {
                if (j != i && ramDisk[j].is_used && strcmp(ramDisk[j].filename, new_name) == 0) {
                    printf("New filename already exists!\n");
                    return -1;
                }
            }
            strncpy(ramDisk[i].filename, new_name, MAX_FILENAME_LENGTH);
            printf("File '%s' renamed to '%s'.\n", old_name, new_name);
            return 0;
        }
    }
    printf("Old file not found!\n");
    return -1;
}

int UpdatePermissions(const char *filename, int new_permissions) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && strcmp(ramDisk[i].filename, filename) == 0) {
            ramDisk[i].permissions = new_permissions;
            printf("Permissions for file '%s' updated.\n", filename);
            return 0;
        }
    }
    printf("File not found!\n");
    return -1;
}

void ListFiles() {
    printf("Files on RAM disk:\n");
    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used) {
            printf("Filename: %s, Type: %s, Permissions: %d, Size: %zu bytes, Creation time: %s, Last access time: %s\n",
                   ramDisk[i].filename,
                   ramDisk[i].type == REGULAR ? "Regular" : "Directory",
                   ramDisk[i].permissions,
                   ramDisk[i].size,
                   ctime(&ramDisk[i].creation_time),
                   ctime(&ramDisk[i].last_access_time));
            if (ramDisk[i].type == REGULAR) {
                printf("Content: %s\n", ramDisk[i].content);
            }
        }
    }
}

void DeleteDirectoryContents(const char *dirPath) {
    char path[MAX_FILENAME_LENGTH];
    snprintf(path, sizeof(path), "%s/", dirPath);

    for (int i = 0; i < MAX_FILES; i++) {
        if (ramDisk[i].is_used && ramDisk[i].type == DIRECTORY &&
            strncmp(ramDisk[i].filename, path, strlen(path)) == 0) {
            DeleteDirectoryContents(ramDisk[i].filename);
        } else if (ramDisk[i].is_used && strncmp(ramDisk[i].filename, path, strlen(path)) == 0) {
            ramDisk[i].is_used = 0;
        }
    }
}

int main()
{
    memset(ramDisk, 0, sizeof(ramDisk));

    CreateFile("dir1/file1.txt", REGULAR, 3);
    CreateFile("dir1/dir2", DIRECTORY, 1);

    WriteFile("dir1/file1.txt", "This is a test file.");

    ReadFile("dir1/file1.txt");
    ReadFile("dir1/dir2");

    UpdatePermissions("dir1/file1.txt", 1);

    DeleteFile("dir1/file1.txt");
    ReadFile("dir1/file1.txt");

    RenameFile("dir1/dir2", "dir1/dir3");
    ReadFile("dir1/dir2");
    ReadFile("dir1/dir3");

    DeleteDirectoryContents("dir1");

    ListFiles();

    return 0;
}
