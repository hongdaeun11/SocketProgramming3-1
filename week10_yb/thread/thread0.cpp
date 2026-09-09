/*go함수와 come함수를 멀티 쓰레드로 동시에 실행시킬려고 하나 잘안되는 예제*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
/*0부터 99까지의 i값을 출력하는 함수*/
void  *go(void *arg) {
  int i; 
  for(i=0; i<100; i++){
      printf("i -->%d\n",i);
  }
}
/*0부터 99까지 j값을 출력하는 함수*/
void *come(void *arg) {
  int j; 
  for(j=0; j<100;j++){
      printf("j -->%d\n",j);
  }
}

int main() {
    pthread_t  goThreadID;//go를 실행시키는 쓰레드의 ID를 저장할 변수
	pthread_t comeThreadID;//come을 실행시키는 쓰레드의 ID를 저장할 변수
    
	printf("thread started...\n\n");
	/*go 함수를 멀티 쓰레드로 실행시키고 ID를 goThreadID에 대입*/
    int goThreadState = pthread_create(&goThreadID, NULL,go, NULL); 
	if(goThreadState!=0)
		printf("goThread creation error");
  	/*come 함수를 멀티 쓰레드로 실행시키고 ID를 comeThreadID에 대입*/
    int comeThreadState = pthread_create(&comeThreadID, NULL,come, NULL); 
    if(comeThreadState!=0)
		printf("comeThread creation error");
    
    printf("thread stopped..\n\n");
    return 0;
}

