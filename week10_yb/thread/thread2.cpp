/*자원을 공유 했을때 예상치 못한 결과가 나오는 예제*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
/*insertNumber1 과 insertNumber2 두개의 쓰레드에서 공유할 변수*/
int array[20]; 
int index=0;//배열의 인덱스
int number=0;//배열에 대입할 변수

void  *insertNumber1(void * arg) {
   while(1){
   if(index>=20) break;

   array[index]=number;
   printf("insertNumber1---> array[%d]=%d\n",index,array[index]);
   sleep(2);
   index++;
   number++;
   }
}
void  *insertNumber2(void * arg) {
	while(1){
      if(index>=20) break;
   array[index]=number;
   printf("insertNumber2---> array[%d]=%d\n",index,array[index]);
   index++;
   sleep(1);
   number++;
	if(index>20) break;
	}
}

int main() {
    pthread_t  insertNumber1ThreadID;//insertNumber1을 실행시키는 쓰레드의 ID를 저장할 변수
	pthread_t insertNumber2ThreadID;//insertNumber2을 실행시키는 쓰레드의 ID를 저장할 변수
    
	printf("\n\n********* multi thread started  **********\n\n");
	/*insertNumber1 함수를 멀티 쓰레드로 실행시키고 ID를 insertNumber1ID에 대입*/
    int number1ThreadState = pthread_create(&insertNumber1ThreadID, NULL,insertNumber1, NULL); 
	/*insertNumber2 함수를 멀티 쓰레드로 실행시키고 ID를 insertNumber2ID에 대입*/
    int number2ThreadState = pthread_create(&insertNumber2ThreadID, NULL,insertNumber2, NULL); 
    
    void * t_return;
	/*insertNumber1 함수가 종료 될때 까지 main 함수를 대기 시킴*/
	int state=pthread_join(insertNumber1ThreadID,&t_return);
	/*insertNumber2 함수가 종료 될때 까지 main 함수를 대기 시킴*/
	state=pthread_join(insertNumber2ThreadID,&t_return);
	printf("\n\n****** multi thread result **********\n\n");
	for(int i=0;i<20;i++)
		printf("array[%d]:%d\n",i,array[i]);

    printf("thread finished..\n\n");
    return 0;
}

