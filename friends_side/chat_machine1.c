#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
void reader();
void writer();

int main(){
    pid_t pid,ppid,cpid;
    int i,j,k,n,m;
    
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,(void *)reader,NULL);
    pthread_create(&tid2,NULL,(void *)writer,NULL);
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    printf("Connection closed for now.\n");
    
    return 0;
}

void reader(){
    char *pipeLocation = "../my_pipe";
    char buffer[201];

    int fd = open(pipeLocation,O_RDONLY);
    if(fd == -1){
        printf("Error to connect reader pipe\n");
        return;
    }
    int t =0;    
    while(1){
        read(fd,buffer,sizeof(buffer));
        if(strcmp(buffer,"-999")==0){
            printf("your frined has gone offline!\n");
            return;
        }
        printf("massage %d'th: %s\n",++t,buffer);
    }
}

void writer(){
    char *pipeLocation = "../friend_pipe";
    
    printf("write msg to pass and give -999 to exit.\n");
    char msg[200];
    int fd = open(pipeLocation,O_WRONLY);
    if(fd == -1){
        printf("Error in writer pipe access\n");
        return;
    }

    while(1){
        fgets(msg,180,stdin);
        msg[strlen(msg)-1]='\0'; // this line to ommit \n from the last of the string

        write(fd,msg,strlen(msg));

        if(strcmp(msg,"-999")==0){
            printf("You are offline!!!\n");
            return;
        }
    }
}