#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int value = 0;
void *func1(void *param) {
value++;
printf("alue = %d\n" , value);
pthread_exit(0);
}
int main(int argc, char *argv[]){
int pid;
pthread_t tid1, tid2;
pthread_create(&tid1,NULL,func1,NULL);
pthread_create(&tid2,NULL,func1,NULL);
pthread_join(tid1,NULL);
pthread_join(tid2,NULL);
return 0;
}