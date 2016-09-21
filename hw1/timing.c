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



main (int argc, char** argv) { 
    clock_t begin = clock();

    fork();
    execve("./hw1", &argv[1]);
    
    clock_t end = clock();

    double utime = (double) (end - begin) / CLOCKS_PER_SEC;

} 


