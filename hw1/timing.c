/*
    COMP 111 Operating Systems
    Mingzhe Li
    Fall 2016
*/


#include <stdio.h> 
#include <sys/times.h>   
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <limits.h> 
#include <time.h>


int main (int argc, char** argv) { 
    struct tms begin, end;
    pid_t pid;

    times(&begin);

    for (int i = 0; i < 1; i++) {
        pid = fork();
        if (pid < 0) 
            printf("Error forking! %s\n", argv[1]);
         
        if (pid == 0) {
		printf("before child\n");
		execve(argv[1], NULL, NULL);
		printf("after child\n");
	}
	
	else {
		printf("here in parent, PID: %f\n", pid);
		wait(NULL);   
	} 
		
    }


    times(&end);

    double user_time = (double) (end.tms_utime - begin.tms_utime) / CLOCKS_PER_SEC;
    double sys_time = (double) (end.tms_stime - begin.tms_stime) / CLOCKS_PER_SEC;
    double cuser_time = (double) (end.tms_cutime - begin.tms_cutime) / CLOCKS_PER_SEC;
    double csys_time = (double) (end.tms_cstime - begin.tms_cstime) / CLOCKS_PER_SEC;

    printf("\nUser Time: %lf\n System Time: %lf\n Child User Time: %lf\n Child System TIme: %lf\n", user_time, sys_time, cuser_time, csys_time); 

    return 0;

} 
