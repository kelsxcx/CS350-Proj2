#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/time.h>
#include <fcntl.h>         
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "file.h"
#include "utility.h"

void current_directory(void){
    char *buf = getcwd(NULL, 0);

    if (buf == NULL) {
        perror("getcwd");
        exit(EXIT_FAILURE);
    }

    printf("Current directory: %s\n", buf);
    free(buf);
}

void change_directory(const char *path){
    if (chdir(path) == -1){
        perror("chdir");
        exit(EXIT_FAILURE);
    }
    printf("Changed directory:");
}

void list_directory(const char *path){
    DIR *directory;
    struct dirent *entry;
    struct stat fileinfo;
    int i;

    if (*path == '\0'){
        directory = opendir(".");
    } else {
        directory = opendir(path);
    }
    

    if (directory == NULL){
        printf("Unable to open directory.\n");
        exit(0);
    }

    while ((entry = readdir(directory)) != NULL){
        i = lstat(entry->d_name, &fileinfo);
        if (i == -1){
            perror("stat");
            continue;
        }
        printf("%s is %ld bytes, with permissions %d and was last modified %ld\n",
        entry->d_name, fileinfo.st_size, fileinfo.st_mode, fileinfo.st_mtime);
    }

    closedir(directory);
}

void rename_entry(const char *source, const char *destination){
    if (destination == NULL){
        printf("destination is required.");
        exit(0);
    }
    if (rename(source, destination) == -1){
        perror("rename");
        exit(EXIT_FAILURE);
    }
    printf("Entry renamed.");
}

void create_directory(const char *path){
    if (mkdir(path) == -1){
        printf("Unable to create directory.\n");
        exit(0);
    }
}

void remove_directory(const char *path){
    if (rmdir(path) == -1){
        printf("Unable to remove directory .\n");
        exit(0);
    }
}
void make_link(const char *kind, const char *path, const char * dest){
    
    if (strcmp(kind, "h") == 0){
        if (link(path, dest) == -1){
            perror("link");
        }
    } else if (strcmp(kind, "s") == 0){
        if (symlink(path, dest) == -1){
            perror("symlink");
        }
    }
    printf("Link created.");
    exit(0);
}
