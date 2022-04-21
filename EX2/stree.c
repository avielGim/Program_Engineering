#define _XOPEN_SOURCE 600 /* Get nftw() */
#include <ftw.h>
#include <sys/types.h> /* Type definitions used by many programs */
#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions,
                             plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>    /* Prototypes for many system calls */
#include <errno.h>     /* Declares errno and defines error constants */
#include <string.h>    /* Commonly used string-handling functions */

#include <pwd.h>
#include <grp.h>
#include <dirent.h>

void show_permission(int);
void uid_to_name(uid_t);
void gid_to_group(gid_t);
void show_type(int, int);
void print_pritier(int);

int DIRS = 0;
int FILES = 0;

static int /* Callback function called by ftw() */
dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    if (ftwb->level == 0)
    {
        printf("%s\n", &pathname[ftwb->base]); /* Print basename */
    }
    else
    {
        // if(pathname[ftwb->base] == '.'){
        //     return 0;
        // }
        int lvl = (ftwb->level - 1);
        print_pritier(lvl);

        int mode = sbuf->st_mode;
        show_type(mode, type);
        show_permission(mode);

        uid_to_name(sbuf->st_uid);  // printf("ID owner: %d\t", buf.st_uid);
        gid_to_group(sbuf->st_gid); // printf("ID of group: %d\t", buf.st_gid);

        printf("%*ld]", 12, sbuf->st_size); // size

        printf("  %s\n", &pathname[ftwb->base]); /* Print basename */
    }
    return 0; /* Tell nftw() to continue */
}

int main(int argc, char *argv[])
{
    int flags = 0;
    if (argc > 2)
    {
        fprintf(stderr, "Usage: %s directory-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 1)
    {
        if (nftw(".", dirTree, 10, flags) == -1)
        {
            perror("nftw");
            exit(EXIT_FAILURE);
        }
    }

    if (nftw(argv[1], dirTree, 10, flags) == -1)
    {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    printf("\n%d directories, %d files\n", DIRS, FILES);
    exit(EXIT_SUCCESS);
}

void print_pritier(int lvl)
{
    for (int i = 0; i < lvl; i++)
    {
        printf("│  ");
    }
    printf("├──[");
}

void show_type(int mode, int type)
{
    if (type == FTW_NS)
    { /* Could not stat() file */
        printf("?");
    }
    else
    {
        switch (mode & S_IFMT)
        {
        case S_IFREG:
            printf("-");
            FILES++;
            break;
        case S_IFDIR:
            printf("d");
            DIRS++;
            break;
        case S_IFCHR:
            printf("c");
            break;
        case S_IFBLK:
            printf("b");
            break;
        case S_IFLNK:
            printf("l");
            break;
        case S_IFIFO:
            printf("p");
            break;
        case S_IFSOCK:
            printf("s");
            break;
        default:
            printf("?");
            break; /* Should never happen (on Linux) */
        }
    }
}

void show_permission(int mode)
{ // , char st[]){
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void uid_to_name(uid_t uid)
{
    printf(" %s\t", getpwuid(uid)->pw_name); // ID owner:
}

void gid_to_group(gid_t gid)
{
    printf("%s\t", getgrgid(gid)->gr_name); // ID group:
}

// printf("%*s├──[", 2 * lvl, "");         /* Indent suitably */
