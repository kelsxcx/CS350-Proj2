#ifndef FILE_H
#define FILE_H
#include "utility.h"
#include <stdbool.h>
#include <sys/types.h>
 

int write_all(int fd, const char *buf, size_t count);
int open_regular(const char *path);

void close_checked(int fd);
void permissions(mode_t m, char out[11]);
void file_information(const char *path);
void display_file(const char *path);
void copy_file(const char *source, const char *destination) ;
void change_permissions(const char *path);
void show_link(const char *path);
void create_empty(const char *path);
void delete_entry(const char *path);

#endif 
