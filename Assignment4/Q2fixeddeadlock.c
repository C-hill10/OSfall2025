#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
// this is the one with 2 producers that deadlock, they acquire locks in opposite order
int inleft=0;
int inright=4;
int outleft=0;
int outright=4;
int counterleft,counterright=0;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;; //number of empty buffer spaces 
pthread_cond_t not_empty= PTHREAD_COND_INITIALIZER;; // items available to consume
static pthread_mutex_t left_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t right_mutex = PTHREAD_MUTEX_INITIALIZER;
static int shelf[8];

void *producerleft(void *param) {
for(int i=0;i<50;i++){
int item=rand()%50;
pthread_mutex_lock(&left_mutex);
printf("producer left has claimed left mutex");
pthread_mutex_lock(&right_mutex);
printf("producer left has claimed right mutex");
// critical section
while(counterleft==4){
pthread_mutex_unlock(&right_mutex);
pthread_cond_wait(&not_full,&left_mutex);
}
pthread_mutex_lock(&right_mutex);
shelf[inleft]=item;
inleft=(inleft+1)%4;
counterleft++;
printf("[Producer left] produced item %d -> shelf has %d item(s)\n",item,counterleft);
pthread_mutex_unlock(&right_mutex);
pthread_cond_signal(&not_empty);
pthread_mutex_unlock(&left_mutex);
}
pthread_exit(0);
}

void *producerright(void *param) {
for(int i=0;i<50;i++){
int item=rand()%50;
pthread_mutex_lock(&left_mutex);
printf("producer right has claimed left mutex");
pthread_mutex_lock(&right_mutex);
printf("producer right has claimed left mutex");
// critical section
while(counterright==4){
    pthread_mutex_unlock(&right_mutex);
pthread_cond_wait(&not_full,&left_mutex);
}
pthread_mutex_lock(&right_mutex);
shelf[inright]=item;
inright=((inright+1)%4)+4;
counterright++;
printf("[Producer right] produced item %d -> shelf has %d item(s)\n",item,counterright);
pthread_mutex_unlock(&right_mutex);
pthread_cond_signal(&not_empty);
pthread_mutex_unlock(&left_mutex);
}
pthread_exit(0);
}


void *consumer(void *param) {
int id= *((int *)param);
for(int i=0;i<50;i++){
int item;
pthread_mutex_lock(&left_mutex);
printf("consumer %d has claimed left mutex",id);
pthread_mutex_lock(&right_mutex);
printf("consumer %d has claimed right mutex",id);
// critical section
//if neither shelf has anything, wait
while((counterright == 0 && counterleft == 0)){
pthread_mutex_unlock(&right_mutex); 
pthread_cond_wait(&not_full,&left_mutex);
} 
pthread_mutex_lock(&right_mutex);
if(rand()%2){ // if 1 check right shelf first
if (counterright!=0)
{
   item=shelf[outright];
outright=((outright+1)%4)+4;
counterright--;
printf("[Consumer %d] consumed item %d -> right shelf has %d item(s)\n",id,item,counterright);
}else if(counterleft!=0){
item=shelf[outleft];
outleft=(outleft+1)%4; 
counterleft--;   
printf("[Consumer %d] consumed item %d -> left shelf has %d item(s)\n",id,item,counterleft);
}

}else {//check left shelf first
    if(counterleft!=0){
item=shelf[outleft];
outleft=(outleft+1)%4;    
counterleft--;   
printf("[Consumer %d] consumed item %d -> left shelf has %d item(s)\n",id,item,counterleft);
} else if(counterright!=0){
item=shelf[outright];
outright=((outright+1)%4)+4;
counterright--;
printf("[Consumer %d] consumed item %d -> right shelf has %d item(s)\n",id,item,counterright);
}
}



pthread_mutex_unlock(&right_mutex);
pthread_cond_signal(&not_full);
pthread_mutex_unlock(&left_mutex);
}
pthread_exit(0);   
}

int main(int argc, char *argv[]){
pthread_t producer1,producer2,consumer1,consumer2;
int id=1;
int id2=2;
printf("from main starting program\n");
if(pthread_create(&producer1,NULL,producerright,NULL)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer1,NULL,consumer,(void *)&id)!=0){
  perror("pthread_create");
}
id++;
if(pthread_create(&producer2,NULL,producerleft,NULL)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer2,NULL,consumer,(void *)&id2)!=0){
  perror("pthread_create");
}
pthread_join(producer1,NULL);
pthread_join(producer2,NULL);
pthread_join(consumer1,NULL);
pthread_join(consumer2,NULL);
pthread_mutex_destroy(&right_mutex);
pthread_cond_destroy(&not_full);
pthread_cond_destroy(&not_empty);
pthread_mutex_destroy(&left_mutex);
return 0;
}