#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>  
#include <unistd.h> 
#include <fcntl.h>
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
    int counter;
    char *args[5], wd[25];
    char *input = malloc(sizeof(char)*30);
    /*TLISTA historial;
    crea(&historial);*/
    TLISTA abiertos;
    crea(&abiertos);
    while(1){
        printf("->");
        fgets(input, sizeof(input), stdin);
        counter=0;
        char *ref = strtok(input, " \n");
        while (ref != NULL) {
            args[counter++] = ref;
            ref = strtok(NULL, " \n");
        }
        args[counter] = NULL;
        if (args[0] != NULL) {
            if (strcmp(args[0], "date") == 0) {
                if (args[1] != NULL && strcmp(args[1], "-t")==0) {
                    fechat();
                } else if (args[1] != NULL && strcmp(args[1],"-d")==0){
                    fechad();
                }else{
                    printf("No se encontro tu comando\n");
                }
            } else if (strcmp(args[0],"pid")==0){
                pid_t pid = getpid();
                printf("%d\n",pid);
            }
            else if (strcmp(args[0],"exit")==0 || strcmp(args[0],"bye")==0 || strcmp(args[0],"quit")==0) {
                printf("Saliendo del shell...\n");
                free(input);
                destruye(abiertos);
                break;
            }else if(strcmp(args[0],"cd")==0){
            getcwd(wd,sizeof(wd));
            printf("%s\n",wd);
            }else if(strcmp(args[0],"open")==0){
                Cmd_open(args);
            }else if(strcmp(args[0],"authors")==0){
                if(counter == 1){
                    authors();
                }else{
                    if(strcmp(args[1],"-l")==0){
                        authorsl();
                    }else if(strcmp(args[1],"-n")==0){
                        authorsn();
                    }
                    else{
                        printf("No se encontró tu comando\n");
                    }
                }
            }
            else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
        }
        else{
            printf("Error al escanear la linea.\n");
        }
    }
}
