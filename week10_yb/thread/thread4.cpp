/*Mutex 방식으로 자원의 동기화를 해도 여전히 문제가 있는 예제*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
/*pop,push 두개의 쓰레드에서 공유할 변수*/
char stack[5]; 
int index=0;//배열의 인덱스
char data;

pthread_mutex_t mutex;

void  *pop(void * arg) {
   for(int i=0;i<5;i++){
      pthread_mutex_lock(&mutex);
      index--;
	  printf("pop -->stack[%d]=%c\n",index,stack[index]);
   	  pthread_mutex_unlock(&mutex);
   }
}


void  *push(void * arg) {
   for(int i=0;i<5;i++){
      pthread_mutex_lock(&mutex);
	  data='A'+i;
	  stack[index]=data;
   	  printf("push -->stack[%d]=%c\n",index,stack[index]);
	  index++;
   	  pthread_mutex_unlock(&mutex);
   }
}


int main() {
    pthread_t  popThreadID;//pop을 실행시키는 쓰레드의 ID를 저장할 변수
	pthread_t pushThreadID;//push을 실행시키는 쓰레드의 ID를 저장할 변수
	pthread_mutex_init(&mutex,NULL);
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
	pthread_mutex_destroy(&mutex);
    return 0;
}

