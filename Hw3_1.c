#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int global_var = 0;
static int static_var = 0;

void* thread_function(void* arg) {
int local_var = 0;
int* dynamic_var = (int*)malloc(sizeof(int));
*dynamic_var = 0;
local_var++;
(*dynamic_var)++;
global_var++;
static_var++;
free(dynamic_var);
printf("printing from a thread. dynamic_var=%d. global var=%d , Static var=%d \n",*dynamic_var,global_var,static_var);
pthread_exit(NULL);
}


int main() {
int i;
pid_t pid;
for (i = 0; i < 3; i++) {
pid = fork();
if (pid == 0) {
pthread_t threads[3];
int j;
for (j = 0; j < 3; j++) {
pthread_create(&threads[j], NULL, thread_function,NULL);
}
for (j = 0; j < 3; j++) {
pthread_join(threads[j], NULL);
}
exit(0);
} else if (pid < 0) {
fprintf(stderr, "Fork failed\n");
return 1;
}
}
for (i = 0; i < 3; i++) {
wait(NULL);
}
printf("final value of global var =%d ,  Final value of static var= %d \n",global_var,static_var);
return 0;
}