#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int global_var = 0;
static int static_var = 0;
void* thread_function(void* arg) {
int local_var = 0;
int* dynamic_var = (int*)calloc(sizeof(int), 1);
local_var++;
(*dynamic_var)++;
global_var++;
static_var++;
free(dynamic_var);
pid_t pid;
int i;
for (i = 0; i < 3; i++) {
pid = fork();
if (pid == 0) {
local_var++;
(*dynamic_var)++;
global_var++;
static_var++;
exit(0);
} else if (pid < 0) {
fprintf(stderr, "Fork failed\n");
return NULL;
}
}
for (i = 0; i < 3; i++) {
wait(NULL);
}
pthread_exit(NULL);
}
int main() {
pthread_t threads[3];
int i;
for (i = 0; i < 3; i++) {
pthread_create(&threads[i], NULL, thread_function, NULL);
}
for (i = 0; i < 3; i++) {
pthread_join(threads[i], NULL);
}
return 0;
}