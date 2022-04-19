#define _XOPEN_SOURCE 600 /* Get nftw() */

#include <stdio.h>
#include <sys/stat.h>
#include <ftw.h>
#include <string.h>
#include <pwd.h>
#include <grp.h> 
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>

// use stat to extract the data

/**
 * permission - done
 * user - done
 * group - done
 * size - done
 * name ? - meybe ?
 */

void show_permission(int mode, char st[]){
    strcpy(st, "----------");
    switch (mode & S_IFMT) {  // Print file mode 
        case S_IFCHR: st[0] = 'c'; 
        case S_IFDIR: 
            st[0] = 'd';
    }
    if(mode & S_IRUSR) st[1] = 'r';
    if(mode & S_IWUSR) st[2] = 'w';
    if(mode & S_IXUSR) st[3] = 'x';

    if(mode & S_IRGRP) st[4] = 'r';
    if(mode & S_IWGRP) st[5] = 'w';
    if(mode & S_IXGRP) st[6] = 'x';

    if(mode & S_IROTH) st[7] = 'r';
    if(mode & S_IWOTH) st[8] = 'w';
    if(mode & S_IXOTH) st[9] = 'x';
}

void uid_to_name(uid_t uid){
    printf("%s\t", getpwuid(uid)->pw_name); // ID owner: 
}

void gid_to_group(gid_t gid){
    printf("%s\t", getgrgid(gid)->gr_name); // ID group: 
}

void show_info(char *file_name){
    struct stat buf;

    int mode = 0;

    if ( stat(file_name, &buf) == -1){
        printf("didn't find\n");
    }
    else {

        mode = buf.st_mode;
        char st[10];
        show_permission(mode, st);
        printf("%s\t", st);

        uid_to_name(buf.st_uid); // printf("ID owner: %d\t", buf.st_uid);
        gid_to_group(buf.st_gid); // printf("ID of group: %d\t", buf.st_gid);
        printf("%ld\t", buf.st_size); // size
        printf("%ld\t", buf.st_nlink); // links: 
        // printf("last update: %ld\t", buf.st_mtime);
        // printf("last access: %ld\t", buf.st_ctime);
        // printf("last property-changed: %ld\t", buf.st_atime);
        printf("%s\n", file_name); // file name: 
    }
}

void do_ls(char dir_name[]){
    DIR *dir_ptr;
    struct dirent *dir_data;

    if((dir_ptr = opendir(dir_name)) == NULL){
        printf("could not open\n");
        return;
    } else {
        while( (dir_data = readdir(dir_ptr)) != NULL){
            printf("\t");
            char *temp_name = dir_data->d_name; 
            show_info(temp_name);
        }
        closedir(dir_ptr);
    }
}


int main(int argc, char *argv[]){
    if (argc == 1){
        char *st = ".";
        do_ls(st);
    } else {
        while(--argc){
            ++argv;
            do_ls(*argv);
        }
    }
    return 0;    
}

/*
printf("mode: ");
        mode = buf.st_mode;
        if (mode == FTW_NS) {                  // Could not stat() file
            printf("?");
        } else {
            switch (mode & S_IFMT) {  // Print file mode 
            case S_IFREG:  printf("-\n"); break;
            case S_IFDIR:  printf("d\n"); break;
            case S_IFCHR:  printf("c\n"); break;
            case S_IFBLK:  printf("b\n"); break;
            case S_IFLNK:  printf("l\n"); break;
            case S_IFIFO:  printf("p\n"); break;
            case S_IFSOCK: printf("s\n"); break;
            default:       printf("?\n"); break; // Should never happen (on Linux)
            }
        }
*/