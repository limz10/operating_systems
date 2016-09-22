/*
    COMP 111 Operating Systems
    Mingzhe Li
    Fall 2016
*/


#include <stdio.h> 
#include <sys/times.h>   
#include <unistd.h> 
#include <limits.h> 
#include <time.h>


struct tms {
    clock_t tms_utime;  /* user */
    clock_t tms_stime;  /* system */
    clock_t tms_cutime; /* user of children */
    clock_t tms_cstime; /* system of children */
};

clock_t times(struct tms *buffer);



int main (int argc, char** argv) { 
    struct tms begin, end;
    pid_t pid;

    times(&begin);

    while() {
        pid = fork();
        if (pid < 0) 
            printf("Error forking! %s\n", argv[1]);
         
        if (pid == 0) {
            execve(argv[1], &argv[1], NULL);
            wait();
        }
    }


    times(&end);

    double user_time = (double) (end.tms_utime - begin.tms_utime) / CLOCKS_PER_SEC;
    double sys_time = (double) (end.tms_stime - begin.tms_stime) / CLOCKS_PER_SEC;
    double cuser_time = (double) (end.tms_cutime - begin.tms_cutime) / CLOCKS_PER_SEC;
    double csys_time = (double) (end.tms_cstime - begin.tms_cstime) / CLOCKS_PER_SEC;

    printf("User Time: %lf\n System Time: %lf\n Child User Time: %lf\n Child System TIme: %lf\n");

    return 0;

} 