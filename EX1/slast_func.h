#ifndef _SLAST_FUNC_H_
#define _SLAST_FUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utmp.h>        // also wtmp
#include <limits.h>      // LONG_MAX
#include <unistd.h>      // open & read files 
#include <fcntl.h>       // flag read only - O_RDONLY
#include <string.h>

void read_utmp(struct utmp *ut);

void print_begin(struct utmp *ut);

#endif