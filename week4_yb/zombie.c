// fork.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    
    pid_t childPid;
    
    childPid = fork();
    
    if(childPid > 0) {  // parent process
        printf("Parent PID : %ld, pid : %d\n",(long)getpid(), childPid);
        printf("Parent process finished..\n");
    }
    else if(childPid == 0){  // Child process code 
        printf("Child process PID : %ld\n", (long)getpid());

	sleep(10);
        printf("Child process finished...\n");
        exit(0);
    }
    else {  // if fork is failed 
        perror("fork Fail! \n");
        return -1;
    }
	
    sleep(10);
    
    return 0;
}
