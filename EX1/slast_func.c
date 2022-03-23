#include "slast_func.h"

void read_utmp(struct utmp *ut){
    printf("%-8.8s ", ut->ut_user);
    printf("%-12.12s ", ut->ut_line);
    printf("%-16.16s ", ut->ut_host);

    time_t t = ut->ut_tv.tv_sec;
    struct tm *tm = localtime(&t);
    printf("%19.19s\n", asctime(tm));
}


