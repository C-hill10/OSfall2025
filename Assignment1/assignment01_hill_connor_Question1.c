#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
fork(); //First fork, 2 total processes
fork() && fork() || fork(); //parent will execute first 2 forks and maybe 3rd, child only executes first
fork(); // double processes with no restriction
printf("hi\n");
return 0;
}
