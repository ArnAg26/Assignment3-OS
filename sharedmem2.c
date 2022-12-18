#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <semaphore.h>

struct aa{
    char *str;
    int a;
};
char *randstring(size_t length,char *String,int len) {

    char *result=malloc(sizeof(char)*length+1);
    if(result!=NULL){
        for(int i=0;i<length;i++){
            int k=rand()%len;
            result[i]=String[k];
        }
        result[length]='\0';
    }
    return result;
}


int main(int argc,char **argv){
    int length=20;
    char *String="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t len=strlen(String);
    char *array[50];
    srand(time(0));
    for(int i=0;i<50;i++){
        array[i]=randstring(20,String,len);
    }
    for(int i=0;i<50;i++){
        printf("%s\n",array[i]);
    }
    int SIZE=512;
    int sh;
    if((sh=shm_open("p",O_RDWR|O_CREAT,0777))==-1){
        perror("Error creating shm");
    }
    ftruncate(sh,SIZE);
    void *p=mmap(NULL,SIZE,PROT_WRITE,MAP_SHARED,sh,0);
    if(p<0){
        perror("Mapping error");
    }
    int sh2;
    if((sh2=shm_open("pp",O_RDWR|O_CREAT,0777))==-1){
        perror("Error creating shm");
    }
    struct stat rr;

    sem_t *sem_des = sem_open("write", O_CREAT, 0644, 1);
    if(sem_des == (void*)-1){
         perror("sem_open failure");
    }
    sleep(5);
    sem_t *semdes=sem_open("read",0,0644,0);

    for(int i=0;i<10;i++){
        for(int j=0;j<5;j++){
            memcpy(p,array[i*5+j],sizeof(char)*21);
            sem_post(sem_des);
            sleep(2);
            sem_wait(semdes);
            if(fstat(sh2,&rr)==-1){
                perror("Read error");
            }
            int *p2=(int *)mmap(NULL,256,PROT_READ,MAP_SHARED,sh2,0);
            if(p2==MAP_FAILED){
                perror("Mapping error");
            }
            printf("%d",p2[0]);
            printf("\n");
        }
    }
    


//     int x=i*5+j;
        //     p=mmap(NULL,SIZE,PROT_WRITE,MAP_SHARED,sh,64);
        //     if(p==MAP_FAILED){
        //         perror("Mapping failed");
        //     }
        //     memcpy(p,&x,sizeof(int));
    


}
