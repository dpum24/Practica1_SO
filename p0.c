#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "listas.h"

void authors(){
    printf("Rubén Sayáns Fortes, ruben.sayans@udc.es\nDiego Pumarol Guerrero, diego.pumarol@udc.es");
}
void authorsl(){
    printf("ruben.sayans@udc.es, diego.pumarol@udc.es");
}
void authorsn(){
    printf("Rubén Sayáns Fortes, Diego Pumarol Guerrero");
}

void fecha(){
    time_t t;
    struct tm *current;
    time(&t);
    current = localtime(&t);
    printf("%02d/%02d/%04d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
    struct tm *now;
    time(&t);
    now = localtime(&t);
    printf("%02d/%02d/%04d\n",now->tm_hour,now->tm_min,now->tm_sec);
}
void fechad(){
    time_t t;
    struct tm *current;
    time(&t);
    current = localtime(&t);
    printf("%02d/%02d/%04d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
}
void fechat(){
    time_t t;
    struct tm *now;
    time(&t);
    now = localtime(&t);
    printf("%02d/%02d/%04d\n",now->tm_hour,now->tm_min,now->tm_sec);
}

int main(int argc, char** argv){
    //ps aux
    //valgrind
    char cmd[20], arg[20];
    /*TLISTA list;
    crea(&list);*/
    while(1){
        printf("->");
        scanf("%s", cmd);
        scanf("%s",arg);
        if(strcmp(cmd,"quit")==0 || strcmp(cmd,"bye")==0 || strcmp(cmd,"exit")==0){
            //destruye(&list);
            break;
        }else{
            //inserta(list,primero(list),cmd);
        }
    }
}
