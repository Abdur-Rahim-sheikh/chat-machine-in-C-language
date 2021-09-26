#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
// #define max_history 15413 // it's a prime, it's for final use
#define max_history 100
#define min_word_to_be_old_friend 0

//Let's implement a simple map,, using hashcode

int history[max_history],maybe_imposter,old_friend;
char secret[100],magic[100],history_txt[max_history*5];


int hashcode(char *s);
void init();
void store_history();
void reader();
void writer();
int work_on_msg(char *str);
void imposter_checking(int tried);
int main(){
    pid_t pid,ppid,cpid;
    int i,j,k,n,m;
    init();
    // printf("Code is: %d\n",hashcode("rea"));
    
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,(void *)reader,NULL);
    pthread_create(&tid2,NULL,(void *)writer,NULL);
    
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    // finale
    printf("Connection closed for now!\n");
    if(maybe_imposter<=5) store_history();
    return 0;
}
int hashcode(char *s){
    int ans = 0;
    //Ignoring the collision
    for(int i=0;s[i]!='\0';i++){
        int ch = (s[i]|32) - '0';
        ans = (ans*123+ ch)%max_history;
    }
    return ans;
}

void init(){
    int fptr;
    //Read magic word
    fptr = open("magic_word.txt",O_RDONLY);
    if(fptr==-1){
        printf("Error occured when tried to open (magic_word.txt)\n");
        exit(1);
    }
    read(fptr,magic,sizeof(magic));
    close(fptr);

    //Read secret word
    fptr = open("secret_word.txt",O_RDONLY);
    if(fptr==-1){
        printf("Error occured when tried to open (secret_word.txt)\n");
        exit(1);
    }
    read(fptr,secret,sizeof(secret));
    close(fptr);

    //Read my history
    fptr = open("history_word_cnt.txt",O_RDONLY);
    if(fptr==-1){
        printf("Error occured when tried to open (history_word_cnt.txt)\n");
        exit(1);
    }
    read(fptr,history_txt,sizeof(history_txt));
    close(fptr);

    
    if(strlen(magic)==0) printf("No, magic word is set :)\n");
    else printf("I have your magic words !!!\n");

    if(strlen(secret)==0) printf("No, secret word is set :)\n");
    else printf("I have your secret words !!!\n");

    // let's read history
    int sum=0;
    for(int i=0,ind=0;i<max_history;i++,ind++){
        int num=0;
        while(history_txt[ind]!='\n')  num=num*10+(history_txt[ind++]-'0');
        history[i]=num;
        sum+=num;
    }
    if(sum>=min_word_to_be_old_friend)old_friend=1;
    printf("All your history loaded!\n");
    //check mode
    // printf("magic: %s\nsecret: %s\n",magic,secret);

}

void store_history(){
    int ptr;
    char buffer[30];
    ptr = open("history_word_cnt.txt",O_WRONLY);

    if(ptr==-1){
        printf("Error occured when tried to open (history_word_cnt.txt)\n");
        exit(1);
    }
    for(int i=0;i<max_history;i++){
        sprintf(buffer,"%d",history[i]);
        write(ptr,buffer,strlen(buffer));
        write(ptr,"\n",1);
    }
    printf("History successfully stored\n");
    close(ptr);
}

void reader(){
    char *pipeLocation = "../friend_pipe";
    char buffer[201];

    int fd = open(pipeLocation,O_RDONLY);
    if(fd == -1){
        printf("Error to connect reader pipe\n");
        return;
    }
    int t=0;
    maybe_imposter = 0;
    int tried = 0;    
    while(1){
        // implementing special code [1]
        if(maybe_imposter>5)imposter_checking(tried);

        read(fd,buffer,sizeof(buffer));
        if(strcmp(buffer,"-999")==0){
            printf("your friend has gone offline!\n");
            return;
        }
        if(maybe_imposter>5){
            if(strcmp(buffer,secret)==0){
                maybe_imposter=0;
                tried=0;
                printf("Not imposter, good to go\n");
            }
            else{
                tried++;
                continue;
            }
        }
        // implementing special code [2] here..
        maybe_imposter = maybe_imposter + work_on_msg(buffer);
        printf("massage %d'th: %s\n",++t,buffer);
    }
}

void writer(){
    char *pipeLocation = "../my_pipe";
    
    printf("write msg to pass and give -999 to exit.\n");
    char msg[200];
    int fd = open(pipeLocation,O_WRONLY);
    if(fd == -1){
        printf("Error writer pipe access\n");
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
// struct tm {
//    int tm_sec;         /* seconds,  range 0 to 59          */
//    int tm_min;         /* minutes, range 0 to 59           */
//    int tm_hour;        /* hours, range 0 to 23             */
//    int tm_mday;        /* day of the month, range 1 to 31  */
//    int tm_mon;         /* month, range 0 to 11             */
//    int tm_year;        /* The number of years since 1900   */
//    int tm_wday;        /* day of the week, range 0 to 6    */
//    int tm_yday;        /* day in the year, range 0 to 365  */
//    int tm_isdst;       /* daylight saving time             */	
// };
void print_time(FILE *fptr){
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime( &rawtime );
    
    char *buffer = asctime(info);
    buffer[strlen(buffer)-1]='\0'; //omitting the last \n asctime returns msg+'\n'
    
    fprintf(fptr,"(%s):",buffer);
}
int work_on_msg(char *buffer){
    char str[200];
    strcpy(str,buffer);
    const char delim[5] = " ";   // delim decides what substring will be replaced with \0 each time
                                // for our case it's " "(space)
    char *ptr = strtok(str, delim);
    int maybe_imposter = 0,cnt=0,important=0;
    
    // creating pointer to write important massage
    FILE *fptr = fopen("important_msg.txt","a");
    if(fptr==NULL){
        printf("Error to open important_msg.txt file\n");
        exit(1);
    }
	while (ptr != NULL){
        if(cnt==0 && strcmp(ptr,magic)==0){
            print_time(fptr);
            cnt++;
            important = 1;
            ptr = strtok(NULL, delim);
            continue;
        }
    
        if(important==1){
            // fprintf(fptr," "); //to print a space
            fprintf(fptr," %s",ptr);
        }
        int map = hashcode(ptr);
        if(old_friend==1 && history[map]==0)maybe_imposter++;
        history[map]++;
        
        cnt++;
		ptr = strtok(NULL, delim);
	}
    if(important==1){
        fprintf(fptr,"\n");
    }

    return maybe_imposter;
}

void imposter_checking(int tried){
    if(tried==0)
        printf("*** He/She maybe an imposter ask him/her the secret word! ***\a\n\n");
    else if(tried<3) printf("!wrong answer, ask again\a\n");
    else printf("RISK, so plase shut down\n\nEnter -999\n");
    
    printf("current secret word is (%s)\n",secret);

}