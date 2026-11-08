#define _POSIX_C_SOURCE 200809L
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <time.h>
#include <stdint.h>

//assume that it can be a symbolic link
//
int write_all(int fd, const char *buf, size_t count){
    const char *pointer = buf;
    size_t done = 0;
    while(done < count){
        ssize_t n = write(fd, pointer + done, count - done);
        if (n < 0){
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0){
            errno = EIO;
            return -1;
        }
        done += (size_t)n;
    }
    return (int)count;
}

int open_regular(const char *path){
    struct stat info;

    int fd = open(path, O_RDONLY);
    if (fd == -1){
        perror("open source");
        return -1;
    } 
    
    if (fstat(fd, &info) == -1) {
        perror("fstat");
        close_checked(fd);
        return -1;
    }

    if (S_ISREG(info.st_mode)) {
        return fd;
    }

    fprintf(stderr, "This operation requires a regular file.\n");
    close_checked(fd);
    return -1;
    
}

void close_checked(int fd){
    if(close(fd) == -1) {
        perror("close");
    }
}

void permissions(mode_t m, char out[11]){
    
    switch (m & S_IFMT) {
    case S_IFREG:  out[0] = '-'; break;
    case S_IFDIR:  out[0] = 'd'; break;
    case S_IFLNK:  out[0] = 'l'; break;
    case S_IFCHR:  out[0] = 'c'; break;
    case S_IFBLK:  out[0] = 'b'; break;
    case S_IFIFO:  out[0] = 'p'; break;
    case S_IFSOCK: out[0] = 's'; break;
    default:       out[0] = '?'; break;
    }

    out[1] = (m & S_IRUSR) ? 'r' : '-';
    out[2] = (m & S_IWUSR) ? 'w' : '-';
    out[3] = (m & S_IXUSR) ? 'x' : '-';
    out[4] = (m & S_IRGRP) ? 'r' : '-';
    out[5] = (m & S_IWGRP) ? 'w' : '-';
    out[6] = (m & S_IXGRP) ? 'x' : '-';
    out[7] = (m & S_IROTH) ? 'r' : '-';
    out[8] = (m & S_IWOTH) ? 'w' : '-';
    out[9] = (m & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}//could also already have everything set to rwx and only overwrite it  if 

void file_information(const char *path){
    struct stat info;
    char perm_out[11];

    if (lstat(path, &info) == -1){
        perror("lstat");
        exit(1);
    }

    permissions(info.st_mode, perm_out);

    printf("Path: %s\n", path);
    printf("Type/permissions: %s (0%03o)\n", perm_out, info.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)); 
    printf("Size:%jd bytes \n", (intmax_t)info.st_size);
    printf("Owner UID: %ju\n", (uintmax_t)info.st_uid);
    printf("Group GID: %ju\n", (uintmax_t)info.st_gid);
    printf("Link count: %ju\n", (uintmax_t)info.st_nlink);
    printf("Inode: %ju\n", (uintmax_t)info.st_ino);
    printf("Modified: %s\n", ctime(&info.st_mtime));
}

void display_file(const char *path){
    //read all, write all to 1
    int file;
    ssize_t nread;
    char buffer[256];

    if ((file = open_regular(path)) == -1) {
        perror("open regular");
        exit(EXIT_FAILURE);
    }

    while ( (nread=read(file, buffer, 256)) > 0) {
        if (write(1, buffer, nread) < nread) {
            close(file);
            perror("write file");
            exit(EXIT_FAILURE);
        }
    }
    close(file);
    if (nread == -1) {
        perror("read file");
        exit(EXIT_FAILURE);
    }

}

void copy_file(const char *source, const char *destination){
    //want to have a buffer, and read everything into the buffer
    //until you get a zero
    //buffer size is usually a power of two and a pretty big number
    //check if file is link or not
    struct stat source_info;
    struct stat dest_info;

    if (lstat(destination, &dest_info) > -1){
        printf("destination already exists.");
        exit(0);
    }

    if (lstat(source, &source_info) == -1){
        perror("source stat");
        exit(0);
    }

    if (dest_info.st_ino == source_info.st_ino){
        printf("source and destination paths must differ.");
        exit(0);
    }

    int infile, outfile;
    ssize_t nread;
    char buffer[256];

    if ((infile = open(source, O_RDONLY)) == -1) {
        perror("Open infile");
        exit(EXIT_FAILURE);
    }
    if ((outfile = open(destination, O_WRONLY | O_CREAT | O_EXCL, 0666))== -1) {    //process umask has to filter
        close(infile);
        perror("Open outfile");
        exit(EXIT_FAILURE);
    }


    while ( (nread=read(infile, buffer, 256)) > 0) {
        if (write_all(outfile, buffer, 256) == -1) {
            close(infile);
            close(outfile);
            perror("write all");
            return;
        }
    }
    close(infile);
    close(outfile);

    if (nread == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    printf("File copied.")
}


void change_permissions(const char *path){
    unsigned int permissions;

    do{
        printf("Octal permissions (0000-7777, e.g. 0644): ");
        if (scanf("%o", &permissions) != 1) {
            // clear buffer
            while (getchar() != '\n');
            permissions = 9999; 
            continue;
        }
    } while (permissions > 7777);
    
    if (chmod(path, (mode_t)permissions) == 0){
        printf("Permissions changed.");
    } else {
        perror("Error changing permissions");
        exit(EXIT_FAILURE);
    }
}

void show_link(const char *path){
    struct stat info;

    if (lstat(path, &info) == -1){
        perror("lstat");
        exit(1);
    }

    size_t buf_size = (size_t)info.st_size + 1;
    char *buf = malloc(buf_size);

    if (buf == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    ssize_t n = readlink(path, buf, buf_size - 1);

    if (n == -1){
        perror("read link");
        free(buf);
        exit(EXIT_FAILURE);
    }
    buf[n] = '\0';
    printf("%s\n", buf);
    free(buf);
}

void create_empty(const char *path){
    int fd;
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC);

    if (fd == -1){
        perror("open");
         
    }

    close_checked(fd);
    print("Empty file created (permissions filtered by umask).")
}

void delete_entry(const char *path){
    char yorn;
    if(path){
        do {
            printf("Delete this file? [y/N]:");
            scanf(" %c", &yorn);
            //how to handle someone typing full words
            if (yorn != 'y' && yorn != 'N'){
                printf("Invalid choice. Please enter 'y' or 'N'\n");
            }
        } while (yorn != 'y' && yorn != 'N');
        if (yorn == 'y'){
            if(remove(path) < 0){
                perror("Failed to remove entry.");
            }
            printf("File deleted.\n");
            
        }
    }
}
