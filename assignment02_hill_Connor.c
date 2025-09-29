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
#include <string.h>

struct student{
    unsigned long msg_type;
    char student[20];
    int ID;
};

int main(){
    struct student Student;
    struct mq_attr attributes;
    attributes.mq_flags=0;
    attributes.mq_maxmsg=5;
    attributes.mq_msgsize= sizeof(Student);
    attributes.mq_curmsgs=0;
    mqd_t queue = mq_open("/student_reg_queue",O_CREAT | O_RDWR,0,&attributes);
if(fork()==0){ //This creates the frontend of the queue
    char names[3][10]={"Alice","Bob","Charlie"}; //initialize our Names to send to the queue
    for(int i=0;i<3;i++){
        struct student registree;
        strcpy(registree.student,names[i]);
        registree.msg_type=0;
        mq_send(queue,(char *)&registree,sizeof(registree),registree.msg_type);
        printf("Frontend] @ Xs: Sending %s...",registree.student);
        sleep(1);
    }
    printf("Frontend] @ Xs: All students submitted! My job is done."); //all names have been sent
}else 
if(fork()==0){
for(int i=0;i<3;i++){
    struct student registree;
    int Number=1000;
    mq_receive(queue,(char *)&registree,sizeof(registree),(int *)registree.msg_type);
    printf("[Database] @ Ys: Start processing %s...",registree.student);
    sleep(3);
    printf("[Database] @ Zs: Finished processing %s. Assigned ID: %d",registree.student,registree.ID);
    registree.ID=Number;
    Number++;
    registree.msg_type=1;
    mq_send(queue,(char *)&registree,sizeof(registree),registree.msg_type);
}
}else
if(fork()==0){
struct student registree;
for(int i=0;i<3;i++){
mq_receive(queue,(char *)&registree,sizeof(registree),(int *)registree.msg_type); //take in priority 2 message from the DB
printf("[Logger] @ As:CONFIRMED - ID: %d, Name: %s",registree.ID,registree.student); //Print confirmation
}
}else{
wait(NULL);
mq_close(queue);
mq_unlink("/student_reg_queue"); //destroy the queue after all child processes have stopped
}
return 0;
}