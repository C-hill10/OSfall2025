#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_THREADS 5
int global_var = 0;
static int static_var = 0;
void *thread_func(void* x) {
int* dynamic_var = (int*)malloc(sizeof(int));
int local_var=*(int*)x;
for (int i = 0; i < 100000; ++i) {
// Increment local variable
local_var++; // this is passed from the main function
// Increment global variable
global_var++;
// Increment static variable
static_var++;
// Increment dynamic variable
(*dynamic_var)++;
}
printf("Thread finished: Local var: %d, Global var: %d, Static var: %d, Dynamic var: %d\n", local_var, global_var, static_var,*dynamic_var);
/* free the dynamic variable */
free(dynamic_var);
/* exit the thread */
pthread_exit(NULL);
}
int main() {
pthread_t threads[NUM_THREADS];
int local_var = 42;
for (int i = 0; i < NUM_THREADS; ++i) {
/* Create a thread and pass the variable "local_var" to it. */
/* Make sure to check if the thread creation is successful or not. */
if(pthread_create(&threads[i], NULL, thread_func,&local_var)!=0){
    perror("pthread_create");
    exit(EXIT_FAILURE);
}
}
for (int j = 0; j < NUM_THREADS; ++j) {
/* Create a thread and pass the variable "local_var" to it. */
/* Make sure to check if the thread creation is successful or not. */
if(pthread_join(threads[j], NULL)!=0){
    perror("pthread_join");
    exit(EXIT_FAILURE);
}
}
/* Join all the threads */
/* Make sure to check if the thread are joined successfully or not. */
printf("Main thread finished. Local var: %d, Global var: %d, Static var: %d\n", local_var, global_var, static_var);
return 0;
}