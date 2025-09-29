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
mqd_t queue;
mqd_t logQueue;
struct student{
    unsigned long msg_type;
    char student[10];
    int ID;
};

int main(){
    struct student Student;
    struct mq_attr attributes;
    attributes.mq_flags=0;
    attributes.mq_maxmsg=5;
    attributes.mq_msgsize=sizeof(Student);
    attributes.mq_curmsgs=0;
    if(mq_open("/student_reg_queue",O_CREAT|O_RDWR,0666,&attributes) == -1){
        perror("mq_open");//Error checking on starting both queues 
    }else{
        queue =mq_open("/student_reg_queue",O_CREAT | O_RDWR,0666,&attributes);
        printf("queue=%d\n",queue);//Print out handles because that was an issue i was trying to fix 
    }
    if(mq_open("/student_reg_queue",O_CREAT|O_RDWR,0666,&attributes) == -1){
        perror("mq_open"); //Error checking on starting both queues 
    }else{
        logQueue =mq_open("/student_log_queue",O_CREAT | O_RDWR,0666,&attributes);
        printf("logQueue=%d\n",logQueue); //Print out handles because that was an issue i was trying to fix 
    }
if(fork()==0){ //This creates the frontend of the queue
    char names[3][10]={"Alice","Bob","Charlie"}; //initialize our Names to send to the queue
    for(int i=0;i<3;i++){ //3 students to register
        struct student registree;
        strcpy(registree.student,names[i]); //take in the Name into our structure
        registree.msg_type=1; //set priority to 1
       if( mq_send(queue,(char *)(&registree),(sizeof(registree)),registree.msg_type) == -1){
        perror("mq_send"); //error checking values
       }
        printf("Frontend] @ Xs: Sending %s...\n",registree.student);
        sleep(1);
    }
    printf("Frontend] @ Xs: All students submitted! My job is done.\n"); //all names have been sent
}else{ 
if(fork()==0){ //Database
    int Number=1000;
for(int i=0;i<3;i++){
    struct student registree;
    int priority;
    if(mq_receive(queue,(char *)(&registree),(sizeof(registree)),&priority) == -1){
        perror("mq_receive");
    }
    printf("[Database] @ Ys: Start processing %s...\n",registree.student);
    sleep(3);
    registree.ID=Number;
    printf("[Database] @ Zs: Finished processing %s. Assigned ID: %d\n",registree.student,registree.ID);
    Number++;
    registree.msg_type=2;
    if( mq_send(logQueue,(char *)(&registree),(sizeof(registree)),registree.msg_type) == -1){
        perror("mq_send");
       }
}
}else{
if(fork()==0){ //Logger
 struct student log;
 int priority;
 for(int i=0;i<3;i++){
    mq_receive(logQueue,(char *)&log,sizeof(log),&priority);
    while(priority!=2){
        mq_send(logQueue,(char *)(&log),(sizeof(log)),priority);
        mq_receive(logQueue,(char *)&log,sizeof(log),&priority); //take in priority 2 message from the DB
    }
 printf("[Logger] @ As:CONFIRMED - ID: %d, Name: %s\n",log.ID,log.student); //Print confirmation
 }
}else{
while(wait(NULL)>0);
if(mq_close(queue) ==-1){
    perror("mq_close");
}
if(mq_unlink("/student_reg_queue")==-1){ //destroy the queue after all child processes have stopped
    perror("mq_unlink");
}
}
}
}
return 0;
}