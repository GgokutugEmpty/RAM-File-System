# 🗃️ RAM Disk File System

Welcome to the RAM Disk File System project! This project simulates a basic file system using RAM, providing essential file operations like creation, reading, writing, renaming, deletion, and permissions management.

## 🛠️ Features

- **File Creation**: Create regular files and directories.
- **File Reading**: Access file metadata and content.
- **File Writing**: Update the content of regular files.
- **File Deletion**: Remove files and directories.
- **File Renaming**: Rename existing files and directories.
- **Permissions Management**: Modify file permissions.
- **Directory Management**: Recursively delete directory contents.

## 📂 Structure

The file system uses a fixed-size RAM disk with up to `MAX_FILES` entries. Each entry is represented by the `RAMFile` structure:

### `RAMFile` Structure

- **`filename`**: Name of the file or directory.
- **`type`**: Type (regular file or directory).
- **`is_used`**: Indicates if the entry is used.
- **`permissions`**: Permissions (0 - none, 1 - read, 2 - write, 3 - read and write).
- **`creation_time`**: Creation timestamp.
- **`last_access_time`**: Last access timestamp.
- **`size`**: Size (for regular files).
- **`content`**: Content (for regular files).

## 🔧 Functions

- **`CreateFile(const char *filename, enum FileType type, int permissions)`**: Create a new file or directory.
- **`ReadFile(const char *filename)`**: Read metadata and content of a file.
- **`WriteFile(const char *filename, const char *content)`**: Update content of a regular file.
- **`DeleteFile(const char *filename)`**: Delete a file or directory.
- **`RenameFile(const char *old_name, const char *new_name)`**: Rename a file or directory.
- **`UpdatePermissions(const char *filename, int new_permissions)`**: Update file permissions.
- **`ListFiles()`**: List all files and directories.
- **`DeleteDirectoryContents(const char *dirPath)`**: Recursively delete contents of a directory.

## 🚀 Usage

1. **Compile the Code**:

   ```sh
   gcc -o ramdisk ramdisk.c
