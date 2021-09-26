#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
// #define max_history 15413 // it's a prime, it's for final use
#define max_history 100
int main(){
    pid_t pid,ppid,cpid;
    int i,j,k,n,m;
    
    FILE *ptr1 = fopen("./my_side/important_msg.txt","w");
    FILE *ptr2 = fopen("./my_side/history_word_cnt.txt","w");
    FILE *ptr3 = fopen("./my_side/magic_word.txt","w");
    FILE *ptr4 = fopen("./my_side/secret_word.txt","w");
    for(i=0;i<max_history;i++)fprintf(ptr2,"0\n");
    fprintf(ptr3,"important:");
    fprintf(ptr4,"secret");
    
    
    return 0;
}