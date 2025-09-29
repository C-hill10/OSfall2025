// Author name: Connor Hill
// Email: connor.hill10@okstate.edu	
// date:9/17/2025
// Program Description: Testing return values of fork() as a condition for more forks


#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
fork(); //Initial fork, creates 1 parent and 1 child
if(fork() && fork()){ // Initial 2 processes will fork twice and return non 0 values
// this causes both original 2 processes to pass conditional and each fork
// an additional time
fork();
}
printf("hi\n");
return 0;
}
