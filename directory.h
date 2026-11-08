#ifndef DIRECTORY_H
#define DIRECTORY_H
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

void current_directory(void);
void change_directory(const char *path);
void list_directory(const char *path);
void rename_entry(const char *source, const char *destination);
void create_directory(const char *path);
void remove_directory(const char *path);
void make_link(const char *kind, const char *path, const char * dest);

#endif 

