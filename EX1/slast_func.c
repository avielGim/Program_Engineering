#include "slast_func.h"

void read_utmp(struct utmp *ut){
    printf("%-8.8s ", ut->ut_user);

    if(strcmp(ut->ut_line, "~") == 0){
        printf("system boot  ");
    }
    else{
        printf("%-12.12s ", ut->ut_line);
    }
    
    printf("%-16.16s ", ut->ut_host);

    time_t t = ut->ut_tv.tv_sec;
    struct tm *tm = localtime(&t);
    printf("%16.16s\n", asctime(tm));
}

void print_begin(struct utmp *ut){
    printf("\nwtmp begins ");
    
    time_t t = ut->ut_tv.tv_sec;
    struct tm *tm = localtime(&t);
    printf("%s", asctime(tm)); 
}