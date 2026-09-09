/*세마포어 방식으로동기화 해서 문제를 해결*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
/*pop,push 두개의 쓰레드에서 공유할 변수*/
char stack[5]; 
int index=0;//배열의 인덱스

sem_t  semaphore;

void  *pop(void * arg) {
   int i;
   for(i=0;i<5;i++){
      sem_wait(&semaphore);
      index--;
	  printf("pop -->stack[%d]=%c\n",index,stack[index]);

   }
}


void  *push(void * arg) {
   int i;
   for(i=0;i<5;i++){
	  stack[index]='A'+i;
   	  printf("push -->stack[%d]=%c\n",index,stack[index]);
	  index++;
       sem_post(&semaphore);

   }
}


int main() {
    pthread_t  popThreadID;//pop을 실행시키는 쓰레드의 ID를 저장할 변수
	pthread_t pushThreadID;//push을 실행시키는 쓰레드의 ID를 저장할 변수
	sem_init(&semaphore,0,0);
	printf("\n\n*********멀티 쓰레드 시작.**********\n\n");
	/*pop함수를 멀티 쓰레드로 실행시키고 ID를 popID에 대입*/
    int popThreadState = pthread_create(&popThreadID, NULL,pop, NULL); 
	/*push 함수를 멀티 쓰레드로 실행시키고 ID를 pushID에 대입*/
    int pushThreadState = pthread_create(&pushThreadID, NULL,push, NULL); 
    
    void * t_return;
	/*insertNumber1 함수가 종료 될때 까지 main 함수를 대기 시킴*/
	int state=pthread_join(popThreadID,&t_return);
	/*insertNumber2 함수가 종료 될때 까지 main 함수를 대기 시킴*/
	state=pthread_join(pushThreadID,&t_return);
    printf("쓰레드 종료..\n\n");
	sem_destroy(&semaphore);
    return 0;
}

