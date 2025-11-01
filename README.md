# EXT2FS reader
This project focuses on the low-level implementation of the filesystem and file management, specifically targeting the ext2 filesystem, which is popular on Linux.

## Currentlly capable of
- **Read Core Structures**: Parse the filesystem image to extract and display the contents of the superblock and block group descriptors.
- **Traverse Directories**: Starting from the root directory, your program must be able to recursively traverse all subdirectories, printing the complete layout of the filesystem.



## Run 
```bash
g++ *.cpp -o main && ./main <img_file>
```

## Wokring
The program starts from the main.cpp, which is a infinite loop (until input is `exit`).
there is few commands written till now:
- info (show info, similar to dumpe2fs)
- ls (show info about current directory)
- cd <dir_name> (change directory to <dir_name> )
### Commands to add (yet)
- cat (read contents of files)
- nano (ig idk... maybe pass data into an text editor)

### Features implemented till now
- Reading general info of the FS.
- traversing through the file-structure.

### General Wokring of program
The program is divided into 2 major classes, Disk and DiskUtils...
the name explains enough about the classes.
Disk to for functions related to class data, and storing data into memory
DiskUtil is for misc functions & other commands on the disk.

There is a file `struct.hpp` which contains all strcuts of the program

## Issues with the program
while cd into certain folders , i_mode is being affected (not sure of the reason atp)
## some screen shots of the software
![ss1](ss1.png)
![ss2](ss2.png)
![ss3](ss3.png)