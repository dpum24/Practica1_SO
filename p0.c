#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "listas.h"

void authors(){
    printf("Rubén Sayáns Fortes, ruben.sayans@udc.es\nDiego Emilio Pumarol Guerrero, diego.pumarol@udc.es");
}
void authorsl(){
    printf("ruben.sayans@udc.es, diego.pumarol@udc.es");
}
void authorsn(){
    printf("Rubén Sayáns Fortes, Diego Emilio Pumarol Guerrero");
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
    char *cmd,*arg, wd[25];
    char *input = malloc(sizeof(char)*20);
    /*TLISTA historial;
    crea(&historial);*/
    while(1){
        printf("->");
        fgets(input,sizeof(input),stdin);
        input[strcspn(input, "\n")] = 0;
        cmd = strtok(input," ");
        arg = strtok(NULL,"\n");
        if (cmd != NULL) {
            if (strcmp(cmd, "date") == 0) {
                if (arg != NULL && strcmp(arg, "-t")==0) {
                    fechat();
                } else if (arg != NULL && strcmp(arg,"-d")==0){
                    fechad();
                }else{
                    printf("No se encontro tu comando\n");
                }
            } else if (strcmp(cmd,"pid")==0){
                pid_t pid = getpid();
                printf("%d\n",pid);
            }
            else if (strcmp(cmd,"exit")==0 || strcmp(cmd,"bye")==0 || strcmp(cmd,"quit")==0) {
                printf("Saliendo del shell...\n");
                free(input);
                break;
            }else if(strcmp(cmd,"cd")==0){
            getcwd(wd,sizeof(wd));
            printf("%s\n",wd);
            }
            else {
                printf("Comando no reconocido: %s\n", cmd);
            }
        }
        else{
            printf("Error al escanear la linea.\n");
        }
    }
}
