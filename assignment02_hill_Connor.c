// Author name: Connor Hill
// Email: connor.hill10@okstate.edu	
// date:9/26/2025
// Program Description: Registration portal using 3 separate processes

#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mqueue.h>


struct student{
    long msg_type;
    char student[20];
    int ID;
};
int main(){
    struct mq_attr attributes{
        long mq_flags=0;
        long mq_maxmsg=5;
        long mq_msgsize = sizeof(student);
        long mq_curmsg=0;
    }
    mqd_t queue = mq_open("/student_reg_queue",O_CREAT | O_RDWR,0,&attributes);
if(fork()==0){ //This creates the frontend of the queue
    char names[2][10]; //initialize our Names to send to the queue
    names[0]="Alice";
    names[1]="Bob";
    names[2]="Charlie"
    for(i=0;i<3;i++){
        student registree;
        strcpy(registree.student,names[i]);
        registree.msg_type=1;
        mq_send(queue,(char *)&registree,sizeof(registree),1);
        printf("Frontend] @ Xs: Sending %s...",registree.student);
        sleep(1)
    }
    printf("Frontend] @ Xs: All students submitted! My job is done."); //all names have been sent
}else 
if(fork()==0){
for(int i=0;i<3;i++){
    student registree;
    int Number=1000;
    mq_recieve(queue,(char *)&registree,sizeof(registree),1)
    printf("[Database] @ Ys: Start processing %s...",student);
    sleep(3)
    printf("[Database] @ Zs: Finished processing %s. Assigned ID: %d",registree.student,registree.id);
    registree.ID=Number;
    Number++;
    registree.msg_type=2;
    mq_send(queue,(char *)&registree,sizeof(registree),2);
}
}else
if(fork()==0){
student registree;
for(int i=0;i<3;i++){
mq_recieve(queue,(char *)&registree,sizeof(registree),2) //take in priority 2 message from the DB
printf("[Logger] @ As:CONFIRMED - ID: %d, Name: %s",registree.ID,registree.student); //Print confirmation
}
}else{
wait(NULL);
mq_close(queue);
mq_unlink("/student_reg_queue"); //destroy the queue after all child processes have stopped
}
return 0
}