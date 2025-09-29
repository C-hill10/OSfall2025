#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
fork();
fork() || fork() && fork(); //parent will only execute first fork and skip to next line, child can execute 2 or 3 for this line
fork(); // all processes that get here will fork
printf("hi\n");
return 0;
}
