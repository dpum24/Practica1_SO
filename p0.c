#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>  
#include <unistd.h> 
#include <fcntl.h>
#include "listas.h"

void authors(){
    printf("Rubén Sayáns Fortes, ruben.sayans@udc.es\nDiego Emilio Pumarol Guerrero, diego.pumarol@udc.es\n");
}
void authorsl(){
    printf("ruben.sayans@udc.es, diego.pumarol@udc.es\n");
}
void authorsn(){
    printf("Rubén Sayáns Fortes, Diego Emilio Pumarol Guerrero\n");
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
void help() {
    printf("authors[-l/-n]\npid\nppid\ncd [dir]\ndate[-d/-t]\nhistoric[-N/N]\nopen [archivo] modo\nclose [df]\ndup [df]\ninfosys\nhelp [cmd]\nquit\nexit\nbye");
}
void help_cmd(){
    if(strcmp(arg[1], "authors")==0){
        printf("Imprime los nombres y logins de los autores del programa. authors -l imprime solo los logins y authors -n imprime solo los nombres.");
    } else if(strcmp(arg[1], "pid")==0){
        printf("Imprime el pid del proceso que está ejecutando la shell");
    } else if(strcmp(arg[1], "ppid")==0){
        printf("Imprime el pid del proceso padre de la shell.");
    } else if(strcmp(arg[1], "cd")==0){
        printf(" Cambia el directorio de trabajo actual de la shell a dir. Si se invoca sin argumentos, imprime el directorio de trabajo actual");
    } else if(strcmp(arg[1], "date")==0){
        printf(" Imprime la fecha actual en formato DD/MM/AAAA y la hora actual en formato hh:mm ");
    } else if(strcmp(arg[1], "historic")==0){
        printf("Muestra el historial de comandos ejecutados por esta shell. \n"
               "\n"
               "historic imprime todos los comandos introducidos con su número de orden. \n"
               "\n"
               "historic N repite el comando número N (de la lista de históricos). \n"
               "\n"
               "historic -N imprime solo los últimos N comandos.");
    } else if(strcmp(arg[1], "open")==0){
        printf(" Abre un archivo y lo añade (junto con el descriptor de archivo y el modo de apertura) a la lista de archivos abiertos por la shell");
    } else if(strcmp(arg[1], "close")==0){
        printf("Cierra el descriptor de archivo df y elimina el ítem correspondiente de la lista. ");
    } else if(strcmp(arg[1], "dup")==0){
        printf("Duplica el descriptor de archivo df, creando la correspondiente nueva entrada en la lista de archivos. ");
    } else if(strcmp(arg[1], "infosys")==0){
        printf("Imprime información sobre la máquina que ejecuta la shell. ");
    } else if(strcmp(arg[1], "help")==0){
        printf("help muestra una lista de comandos disponibles. help cmd ofrece una breve descripción del uso del comando cmd");
    } else if((strcmp(arg[1], "quit") || strcmp(arg, "bye") || strcmp(arg, "exit"))==0){
        printf("Finaliza la shell. ");
    }
}

void Cmd_open (char * tr[],TLISTA *abiertos){
    int i,df, mode=0;
    TIPOELEMENTOLISTA elemento;
    //tr[0] es el comando open
    //tr[1] es el nombre del fichero
    //tr[2] es el modo
    if (tr[1]==NULL) { /*no hay parametro*/
        if (!esVacia(*abiertos)){
        listararchivos(*abiertos);
        }else{
            printf("No hay ningun archivo abierto\n");
        }
        return;
    }
    for (i=1; tr[i]!=NULL; i++)
      if (!strcmp(tr[i],"cr")) mode|=O_CREAT;
      else if (!strcmp(tr[i],"ex")) mode|=O_EXCL;
      else if (!strcmp(tr[i],"ro")) mode|=O_RDONLY; 
      else if (!strcmp(tr[i],"wo")) mode|=O_WRONLY;
      else if (!strcmp(tr[i],"rw")) mode|=O_RDWR;
      else if (!strcmp(tr[i],"ap")) mode|=O_APPEND;
      else if (!strcmp(tr[i],"tr")) mode|=O_TRUNC; 
      else break;
      
    if ((df=open(tr[1],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        strcpy(elemento.filename,tr[1]);
        elemento.filedes=df;
        elemento.mode=fcntl(df,F_GETFL);
        inserta(abiertos,fin(*abiertos),elemento);
        printf ("Anadida entrada a la tabla ficheros abiertos\n");
}
}
int TrocearCadena(char * cadena, char * trozos[]){ 
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
    return i;
}

int main(int argc, char** argv){
    //ps aux
    //valgrind
    int counter;
    char *args[10], wd[25];
    char *input = malloc(sizeof(char) * 20);
    TLISTA abiertos;
    crea(&abiertos);
    while(1){
        printf("->");
        fgets(input, 20, stdin);
        counter = TrocearCadena(input,args);
        if (counter != 0) {
            if (strcmp(args[0], "date") == 0) {
                if (args[1] != NULL && strcmp(args[1], "-t")==0) {
                    fechat();
                } else if (args[1] != NULL && strcmp(args[1],"-d")==0){
                    fechad();
                }else if (args[1] = NULL){
                    fecha();
                } else {
                    perror("No se encontro tu comando\n");
                }
            } else if (strcmp(args[0],"pid")==0){
                pid_t pid = getpid();
                printf("%d\n",pid);
            }
            else if(strcmp(args[0],"cd")==0){//Directorios
                if (args[1] == NULL){
                getcwd(wd,sizeof(wd));
                printf("%s\n",wd);
                }else{
                    if(chdir(args[1])==-1){
                        printf("Error al encontrar el directorio %s\n",args[1]);
                    }
                    else{
                        printf("Exitoso cambio a %s\n",args[1]);
                    }
                }
            }
            else if(strcmp(args[0],"open")==0){//Comando Open
                Cmd_open(args,&abiertos);
            }
            else if(strcmp(args[0],"authors")==0){//Autores de la practica
                if(counter == 1){
                    authors();
                }else{
                    if(strcmp(args[1],"-l")==0){
                        authorsl();
                    }else if(strcmp(args[1],"-n")==0){
                        authorsn();
                    }
                    else{
                        perror("No se encontró tu comando\n");
                    }
                }
            }else if(strcmp(arg[0],"help")==0){
                if(strcmp(arg[1] != NULL) && strcmp(arg[1],"authors") || strcmp(arg[1], "pid") || strcmp(arg[1], "ppid") || strcmp(arg[1], "cd") || strcmp(arg[1], "date") || strcmp(arg[1], "historic") || strcmp(arg[1], "open") || strcmp(arg[1], "close") || strcmp(arg[1], "dup") || strcmp(arg[1], "infosys") || strcmp(arg[1], "help") || strcmp(arg[1], "quit") || strcmp(arg[1], "exit") || strcmp(arg[1], "bye")){
                    help_cmd();
                } else help();
            } else if (strcmp(args[0],"exit")==0 || strcmp(args[0],"bye")==0 || strcmp(args[0],"quit")==0) {//Sale del shell
                printf("Saliendo del shell...\n");
                free(input);
                destruye(abiertos);
                break;
            }
            else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
        }
        else{
            perror("Error al escanear la linea.\n");
        }
    }
}
