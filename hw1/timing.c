    /*
    COMP 111 Operating Systems
    Mingzhe Li
    Fall 2016
*/


#include <stdio.h>
#include <stdlib.h> 
#include <sys/times.h>   
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <limits.h> 
#include <time.h>


int main (int argc, char** argv) { 
        if (argc != 2 || &argv == NULL) {
                printf("Error input!\n");
                exit(1);
        }

        struct tms begin, end;
        clock_t begin_clock, end_clock;

        pid_t pid;

        int status;

        begin_clock = times(&begin);

        for (int i = 0; i < 10; i++) {
                pid = fork();
                if (pid < 0) 
                        printf("Error forking! %s\n", argv[1]);

                if (pid == 0) {
                        // printf("before child\n");
                        execve(argv[1], NULL, NULL);
                        exit(0);
                        // printf("after child\n");
                }
                else {
                        // printf("here in parent, PID: %i\n", pid);
                        // printf("Child Status: %i\n", WIFEXITED(status));
                        wait(&status);   
                }
        }              


        begin_clock = times(&end);

        double wall_clock = (double) (end_clock - begin_clock) / CLOCKS_PER_SEC;
        double user_time = (double) (end.tms_utime - begin.tms_utime) / CLOCKS_PER_SEC;
        double sys_time = (double) (end.tms_stime - begin.tms_stime) / CLOCKS_PER_SEC;
        double cuser_time = (double) (end.tms_cutime - begin.tms_cutime) / CLOCKS_PER_SEC;
        double csys_time = (double) (end.tms_cstime - begin.tms_cstime) / CLOCKS_PER_SEC;

        printf("\nReal Time:\t\t%lf", wall_clock);
        printf("\nUser Time:\t\t%lf\nSystem Time:\t\t%lf\nChild User Time:\t%lf\nChild System TIme:\t%lf\n", 
                user_time, sys_time, cuser_time, csys_time); 

        return 0;
} 
