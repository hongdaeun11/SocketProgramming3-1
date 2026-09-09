// fork.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
    
    pid_t childPid;
    
    childPid = fork();
    
    if(childPid > 0) {  // parent process
        printf("Parent process PID : %ld, pid : %d\n",(long)getpid(), childPid);

	sleep(20);
        printf("Parent process finished..\n");
    }
    else if(childPid == 0){  // Child process code 
        printf("Child process PID : %ld\n", (long)getpid());

	sleep(30);
        printf("Child process finished...\n");
    }
    else {  // if fork is failed 
        perror("fork Fail! \n");
        return -1;
    }
    
    return 0;
}
