#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
int in=0;
int out=0;
int counter=0;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;; //number of empty buffer spaces 
pthread_cond_t not_empty= PTHREAD_COND_INITIALIZER;; // items available to consume
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int shelf[8];

void *producer(void *param) {
    int id= *((int *)param);
    printf("producer id is %d",id);
for(int i=0;i<50;i++){
int item=rand()%50;
printf("item %d created",item);
pthread_mutex_lock(&mutex);
// critical section
while(counter==8){
pthread_cond_wait(&not_full,&mutex);
}
shelf[in]=item;
in=(in+1)%8;
counter++;
printf("[Producer %d] produced item %d -> shelf has %d item(s)",id,item,counter);
pthread_cond_signal(&not_empty);
pthread_mutex_unlock(&mutex);
}
pthread_exit(0);
}

void *consumer(void *param) {
  int id= *((int *)param);
    printf("consumer id is %d",id);
for(int i=0;i<50;i++){
int item;
pthread_mutex_lock(&mutex);
// critical section
while(counter==0){
pthread_cond_wait(&not_empty,&mutex);
}
item=shelf[out];
out=(out+1)%8;
counter--;
printf("[Consumer %d] consumed item %d -> shelf has %d item(s)",id,item,counter);
pthread_cond_signal(&not_full);
pthread_mutex_unlock(&mutex);
}
pthread_exit(0);   
}

int main(int argc, char *argv[]){
pthread_t producer1,producer2,consumer1,consumer2;
int id=1;
printf("from main starting program");
if(pthread_create(&producer1,NULL,producer,(void *)&id)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer1,NULL,producer,(void *)&id)!=0){
  perror("pthread_create");
}
id++;
if(pthread_create(&producer2,NULL,producer,(void *)&id)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer2,NULL,producer,(void *)&id)!=0){
  perror("pthread_create");
}
pthread_join(producer1,NULL);
pthread_join(producer2,NULL);
pthread_join(consumer1,NULL);
pthread_join(consumer2,NULL);
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&not_full);
pthread_cond_destroy(&not_empty);
return 0;
}