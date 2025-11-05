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
int counterleft,counterright,workdone,lastworkdone,loopcounter,dangercounter=0;
pthread_cond_t not_full_left = PTHREAD_COND_INITIALIZER;; //number of empty buffer spaces 
pthread_cond_t not_empty_left= PTHREAD_COND_INITIALIZER;; // items available to consume
pthread_cond_t not_full_right = PTHREAD_COND_INITIALIZER;; //number of empty buffer spaces 
pthread_cond_t not_empty_right = PTHREAD_COND_INITIALIZER;; // items available to consume
static pthread_mutex_t left_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t right_mutex = PTHREAD_MUTEX_INITIALIZER;
static int shelf[8];

void *producerleft(void *param) {
    int id= *((int *)param);
for(int i=0;i<50;i++){
int item=rand()%50;
pthread_mutex_lock(&right_mutex);
pthread_mutex_lock(&left_mutex);
// critical section
while(counterleft==4){
pthread_cond_wait(&not_full_left,&left_mutex);
}
shelf[inright]=item;
inleft=(inleft+1)%4;
counterleft++;
workdone++;
loopcounter++;
printf("[Producer left] produced item %d -> shelf has %d item(s)\n",item,counterleft);
pthread_cond_signal(&not_empty_left);
pthread_mutex_unlock(&left_mutex);
pthread_mutex_unlock(&right_mutex);
}
pthread_exit(0);
}

void *producerright(void *param) {
    int id= *((int *)param);
for(int i=0;i<50;i++){
int item=rand()%50;
pthread_mutex_lock(&left_mutex);
pthread_mutex_lock(&right_mutex);
// critical section
while(counterright==4){
pthread_cond_wait(&not_full_right,&right_mutex);
}
shelf[inright]=item;
inright=((inright+1)%4)+4;
counterright++;
workdone;
loopcounter++;
printf("[Producer right] produced item %d -> shelf has %d item(s)\n",item,counterright);
pthread_cond_signal(&not_empty_right);
pthread_mutex_unlock(&left_mutex);
pthread_mutex_unlock(&right_mutex);
}
pthread_exit(0);
}

void *consumerleft(void *param) {
int id= *((int *)param);
for(int i=0;i<50;i++){
int item;
pthread_mutex_lock(&left_mutex);
// critical section
while(counterleft==0){
pthread_cond_wait(&not_empty_left,&left_mutex);
}
item=shelf[outleft];
outleft=(outleft+1)%4;
counterleft--;
workdone++;
loopcounter++;
printf("[Consumer left] consumed item %d -> shelf has %d item(s)\n",item,counterleft);
pthread_cond_signal(&not_full_left);
pthread_mutex_unlock(&left_mutex);
}
pthread_exit(0);   
}

void *consumerright(void *param) {
int id= *((int *)param);
for(int i=0;i<50;i++){
int item;
pthread_mutex_lock(&right_mutex);
// critical section
while(counterright==0){
pthread_cond_wait(&not_empty_right,&right_mutex);
}
item=shelf[outright];
outright=((outright+1)%4)+4;
counterright--;
workdone++;
loopcounter++;
printf("[Consumer right] consumed item %d -> shelf has %d item(s)\n",item,counterright);
pthread_cond_signal(&not_full_right);
pthread_mutex_unlock(&right_mutex);
}
pthread_exit(0);   
}

void *monitorfnc(void * params){
    printf("monitor function created\n");
    while(loopcounter<200){
usleep(3000000); //sleep for 
if(workdone>lastworkdone){
    printf("[Monitor] Everything working as expected\n");
    dangercounter=0;
    lastworkdone=workdone;
} else{
    if(dangercounter<=1)
    dangercounter++;
    printf("[Monitor] no activity for last 2 checks, possible stall detected\n");
}
    }
pthread_exit(0);  
}

int main(int argc, char *argv[]){
pthread_t producer1,producer2,consumer1,consumer2,monitor;
int id=1;
int id2=2;
if(pthread_create(&monitor,NULL,monitorfnc,(void *)&id)!=0){
  perror("pthread_create");
}
if(pthread_create(&producer1,NULL,producerright,(void *)&id)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer1,NULL,consumerright,(void *)&id)!=0){
  perror("pthread_create");
}
if(pthread_create(&producer2,NULL,producerleft,(void *)&id2)!=0){
  perror("pthread_create");
}
if(pthread_create(&consumer2,NULL,consumerleft,(void *)&id2)!=0){
  perror("pthread_create");
}
pthread_join(producer1,NULL);
pthread_join(producer2,NULL);
pthread_join(consumer1,NULL);
pthread_join(consumer2,NULL);
pthread_join(monitor,NULL);
pthread_mutex_destroy(&right_mutex);
pthread_cond_destroy(&not_full_right);
pthread_cond_destroy(&not_empty_right);
pthread_mutex_destroy(&left_mutex);
pthread_cond_destroy(&not_full_left);
pthread_cond_destroy(&not_empty_left);
return 0;
}