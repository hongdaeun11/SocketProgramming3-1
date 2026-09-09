#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    
    pid_t childPid;
    int i;
    
    childPid = fork();
    
    if(childPid > 0) {  // parent process 
        printf("parent PID : %ld, pid : %d\n",(long)getpid(), childPid);
        sleep(2);
        printf("parent finished...\n");
        exit(0);
    }
    else if(childPid == 0){  // child process code 

        printf("child started\n");
        
        for(i=0;i<10;i++) {
            printf("child process PID : %ld parent PID : %ld\n",(long)getpid(), (long)getppid());
            sleep(1);
        }
        
        printf("child process finished... \n");
        exit(0);
    }
    else {  
        perror("fork Fail! \n");
        return -1;
    }
    
    return 0;
}


