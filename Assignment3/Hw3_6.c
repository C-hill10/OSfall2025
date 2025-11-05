#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int count = 10;
void fun1_1(){
count ++;
printf("Thread1 (inside fun1_1: Count value is = %d\n", count);
}
void *fun1(void *arg){
count ++;
printf("Thread1 (before fun1_1 call): Count value is = %d\n",
count);
fun1_1();
printf("Thread1 (after fun1_1 call): Count value is = %d\n",
count);
return NULL;
}
void *fun2(void *arg){
count ++;
printf("Thread2: Count value is = %d\n", count);
return NULL;
}
int main(){
pthread_t thread1, thread2;
void *result;
pthread_create(&thread1, NULL, fun1, NULL);
pthread_create(&thread2, NULL, fun2, NULL);
pthread_join(thread1, NULL);
pthread_join(thread2, NULL);
return 0;
}