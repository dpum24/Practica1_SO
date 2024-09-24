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
void infosys(){
    struct utsname udata;
    if(uname(&udata)<0){
        perror("Error en funcion uname\n");
    }
    printf("nombresistema = %s\n", udata.sysname);
    printf("node name   = %s\n", udata.nodename);
    printf("lanzamiento     = %s\n", udata.release);
    printf("version     = %s\n", udata.version);
    printf("maquina     = %s\n", udata.machine);
}
void historics(char *args[],TLISTA *history,TNODOLISTA *dndhist){
    int i = 0;
    TIPOELEMENTOLISTA elemento;
    if(esVacia(*history)){
        elemento.num=0;
    }else{
        elemento.num = longitud(*history);
    }
    while(args[i]!=NULL){
        strcat(elemento.cmd,args[i]);
        strcat(elemento.cmd," ");
        i++;
    }
    elemento.num++;
    inserta(history,*dndhist,elemento);
    *dndhist = siguiente(*history,*dndhist);
}
void phistorics(TLISTA history, char *arg){//en el caso de -N quitar el "-" y hacer el bucle
    int num;
    char nu[3];
    TNODOLISTA nod;
    TIPOELEMENTOLISTA e;
    if(arg == NULL){
    for(nod =primero(history); nod != fin(history);nod=siguiente(history,nod)){
        recupera(history,nod,&e);
        printf("N=%d %s\n",e.num,e.cmd);
    }}
    else{
        num = atoi(arg);
        if(num==0){
            strtok(arg,"-");
            strcpy(nu,strtok(NULL," "));
            num = atoi(nu);
            printf("%d",num);
            /*for(nod =primero(history); nod != fin(history);nod=siguiente(history,nod)){
                printf("%d: %s",)
            }*/
        }else{
            for(nod = primero(history);nod!=fin(history);nod=siguiente(history,nod)){
            recupera(history,nod,&e);
            if(e.num==num){
                printf("%s\n",e.cmd);
                //repeat_cmd();
                break;
            }
        }
        }
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
    TIPOELEMENTOLISTA e;
    pid_t pid;
    int counter;
    char *args[10], wd[25];
    char *input = malloc(sizeof(char) * 20);
    TLISTA abiertos, historial;
    //TNODOLISTA dndabiertos;
    crea(&abiertos);
    crea(&historial);
    TNODOLISTA dndhist = primero(historial);
    while(1){
        printf("->");
        fgets(input, 20, stdin);
        counter = TrocearCadena(input,args);
        if (counter != 0) {
            if (strcmp(args[0], "date") == 0) {//Comando de fechas falta el caso de sin argumentos
                if (args[1] != NULL && strcmp(args[1], "-t")==0) {
                    fechat();
                } else if (args[1] != NULL && strcmp(args[1],"-d")==0){
                    fechad();
                }else{
                    perror("No se encontro tu comando\n");
                }
            } else if (strcmp(args[0],"pid")==0){//PID
                pid = getpid();
                printf("%d\n",pid);
            }else if(strcmp(args[0],"ppid")==0){
                pid = getppid();
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
                printf("Aqui funcion open\n");
                //Cmd_open(args,&abiertos);
            }else if (strcmp(args[0],"infosys")==0){
                infosys();
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
            }else if(strcmp(args[0],"help")==0){
                if(args[1] != NULL){
                    help_cmd(args);
                } else help();
            }
            else if(strcmp(args[0],"historic")==0){
                phistorics(historial,args[1]);
            }
            else if (strcmp(args[0],"exit")==0 || strcmp(args[0],"bye")==0 || strcmp(args[0],"quit")==0) {//Sale del shell
                printf("Saliendo del shell...\n");
                free(input);
                destruye(abiertos);
                destruye(historial);
                break;
            }
            else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
            historics(args,&historial,&dndhist);
        }
        else{
            perror("Error al escanear la linea.\n");
        }
    }
}
