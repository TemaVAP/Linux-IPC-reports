#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
int flag1=0;
int flag2=0;
static void* proc1(void* arg){
	while (!flag1){
		for (int i=0; i<10; i++){  // KRITICHESKIY UCHASTOK
			printf("1");//     KRITICHESKIY UCHASTOK
			fflush(stdout);//     KRITICHESKIY UCHASTOK
			sleep(1);//     KRITICHESKIY UCHASTOK
		}
		printf("\nVREMYA RABOTI VNE KU\n");
		sleep(3);
	}
	pthread_exit((void*)2);
}
static void* proc2(void* arg){
	while (!flag2){
		for (int i=0; i<10; i++){//     KRITICHESKIY UCHASTOK
			printf("2");//     KRITICHESKIY UCHASTOK
			fflush(stdout);//     KRITICHESKIY UCHASTOK
			sleep(1);//     KRITICHESKIY UCHASTOK
		}
		printf("\nVREMYA RABOTI VNE KU\n");
		sleep(3);
	}
	pthread_exit((void*)3);
}
int main(){
	int* exitcode1;
	int* exitcode2;
	pthread_t thread1;
	pthread_t thread2;
	pthread_create(&thread1, NULL, proc1, NULL);
	pthread_create(&thread2, NULL, proc2, NULL);
	printf("\nEnter the symbol for STOP thread!\n");
	getchar();
	flag1=1;
	flag2=1;
	pthread_join(thread1, (void**)&exitcode1);
	pthread_join(thread2, (void**)&exitcode2);
	printf("\nexitcode for 1 = %p\n", exitcode1);
	printf("\nexitcode for 2 = %p\n", exitcode2);
	printf("Program has completed");
	return 0;
}
