#include<stdio.h>
#include<unistd.h>
// #include<fcntl.h>
#include<sys/stat.h>
int main(){
    pid_t pid,ppid,cpid;
    int i,j,k,n,m;
    
    char *pipePath1 = "./my_pipe";
    char *pipePath2 = "./friend_pipe";

    if(access(pipePath1,F_OK)==0){
        printf("Your pipe already exist so just do your job.\n");
    }
    else{
        int flag = mkfifo(pipePath1,0666);

        if(flag == -1) printf("Error to create your pipe\n");
        else printf("Your pipe just been created\n");
    }

    if(access(pipePath2,F_OK)==0){
        printf("frined pipe already exist so just do your job.\n");
    }
    else{
        int flag = mkfifo(pipePath2,0666);

        if(flag == -1) printf("Error to create friend pipe\n");
        else printf("Friend pipe just been created\n");
    }
    
    return 0;
}