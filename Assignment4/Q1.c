#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>



void *producer(void *param) {

pthread_exit(0);
}
void *consumer(void *param) {

pthread_exit(0);    
}
int main(int argc, char *argv[]){
int pid;
pthread_t producer1,producer2,consumer1,consumer2;
pthread_create(&producer1,NULL,producer,NULL);
pthread_create(&producer2,NULL,producer,NULL);
pthread_create(&consumer1,NULL,producer,NULL);
pthread_create(&consumer2,NULL,producer,NULL);
pthread_join(producer1,NULL);
pthread_join(producer2,NULL);
pthread_join(consumer1,NULL);
pthread_join(consumer2,NULL);
return 0;
}