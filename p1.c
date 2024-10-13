#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "listahist.h"
#include "abiertolista.h"

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
void historics(char *args[],HIST history,COMMAND *elemento){
    int i = 0;
    if(esVaciahist(history)){
        elemento->num=0;
    }else{
        elemento->num = longitudhist(history);
    }
    while(args[i]!=NULL){
        if (i==0){
        strncpy(elemento->cmd,args[i],sizeof(elemento->cmd));
        strcat(elemento->cmd," ");
        i++;
        continue;
        }
        strncat(elemento->cmd,args[i],20);
        strcat(elemento->cmd," ");
        i++;
    }
    elemento->num++;
}

void listar_abiertos(ABIERTOLISTA abiertos){
    TNODOLISTA nod;
    FILES f;
    if (!esVacia(abiertos)){//si no es vacia
        for(nod=primero(abiertos); nod != fin(abiertos);nod=siguiente(abiertos,nod)){
        recupera(abiertos,nod,&f);
        printf("File Descriptor= %d -> %s %d\n",f.filedes,f.filename,f.mode);
    }
    }else{
        printf("No hay ningun archivo abierto\n");
        }
}

void phistorics(HIST history, char *args){//en el caso de -N quitar el "-" y hacer el bucle
    int num;
    TNODOHIST nod, aux;
    COMMAND e;
    if(args == NULL){
    for(nod =primerohist(history); nod != finhist(history);nod=siguientehist(history,nod)){
        recuperahist(history,nod,&e);
        printf("N=%d %s\n",e.num,e.cmd);
    }}
    else{
        num = atoi(args);
        if(num<0){//Imprime por pantalla los ultimos -n comandos
            num = abs(num);
            nod = primerohist(history);
            for(int i=0; i!=longitudhist(history)-num;i++){
                nod = siguientehist(history,nod);
            }
            for(aux=nod;aux!=fin(history);aux=siguientehist(history,aux)){
                recuperahist(history,aux,&e);
                printf("N=%d %s\n",e.num,e.cmd);
            }
        }else{
            for(nod = primerohist(history);nod!=finhist(history);nod=siguientehist(history,nod)){
            recuperahist(history,nod,&e);
            if(e.num==num){
                printf("%s\n",e.cmd);//Repite el comando numero N
                //repeat_cmd();
                break;
            }
        }
        }
    }
}


void Cmd_open(char *tr[], ABIERTOLISTA abiertos, FILES *elemento, int *control) {
    int df, mode = 0;

    // Si no se especifica un nombre de fichero, listar los archivos abiertos
    if (tr[1] == NULL) {
        listar_abiertos(abiertos);
        *control = 0;  // Asignación correcta del valor apuntado
        return;
    }

    // Comprobar si el modo de apertura ha sido especificado
    if (tr[2] == NULL) {
        fprintf(stderr, "Error: Modo de apertura no especificado.\n");
        *control = 0;  // Error al no proporcionar el modo
        return;
    }

    // Determinar el modo de apertura
    if (!strcmp(tr[2], "cr")) {
        mode |= O_CREAT;
    } else if (!strcmp(tr[2], "ex")) {
        mode |= O_CREAT | O_EXCL;
    } else if (!strcmp(tr[2], "ro")) {
        mode |= O_RDONLY;  // Solo lectura
    } else if (!strcmp(tr[2], "wo")) {
        mode |= O_WRONLY;  // Solo escritura
    } else if (!strcmp(tr[2], "rw")) {
        mode |= O_RDWR;  // Lectura y escritura
    } else if (!strcmp(tr[2], "ap")) {
        mode |= O_WRONLY | O_APPEND;  // Agregar al final del archivo
    } else if (!strcmp(tr[2], "tr")) {
        mode |= O_WRONLY | O_TRUNC;  // Truncar el archivo a tamaño 0
    } else {
        fprintf(stderr, "Error: Modo de apertura desconocido.\n");
        *control = 0;  // Error si el modo no es reconocido
        return;
    }

    // Abrir el archivo con el modo especificado
    if ((df = open(tr[1], mode, 0777)) == -1) {
        perror("Imposible abrir fichero");
        *control = 0;  // Indicar que hubo un error al abrir el archivo
    } else {
        // Configurar la estructura elemento con la información del archivo abierto
        strcpy(elemento->filename, tr[1]);
        elemento->filedes = df;
        int flags = fcntl(df, F_GETFL);
        if (flags == -1) {
            perror("Error al obtener las banderas del archivo");
            *control = 0;  // Indicar un problema si fcntl falla
            close(df);
            return;
        }

        elemento->mode = flags;
        *control = 1;  // Indicar que la operación fue exitosa
    }
}


void Cmd_close (char *tr[], ABIERTOLISTA *abiertos){
    int df;
    TNODOLISTA nod;
    FILES e;
    if (tr[1]==NULL || (df=atoi(tr[1]))<0){
      listar_abiertos(*abiertos);// el descriptor es menor que 0
        return;
    }
    if (close(df)==-1){
        perror("Inposible cerrar descriptor");
        return;
}
    else{
       for(nod=primero(*abiertos); nod != fin(*abiertos);nod=siguiente(*abiertos,nod)){
        recupera(abiertos,nod,&e);
        if(e.filedes==atoi(tr[1])){
            break;
        }
    }
       suprime(abiertos,nod);
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

void help() {
    printf("authors[-l/-n]\npid\nppid\ncd [dir]\ndate[-d/-t]\nhistoric[-N/N]\nopen [archivo] modo\nclose [df]\ndup [df]\ninfosys\nhelp [cmd]\nquit\nexit\nbye\n");
}
void help_cmd(char * args[]){
    if(strcmp(args[0], "authors")==0){
        printf("Imprime los nombres y logins de los autores del programa. authors -l imprime solo los logins y authors -n imprime solo los nombres.\n");
    } else if(strcmp(args[0], "pid")==0){
        printf("Imprime el pid del proceso que está ejecutando la shell\n");
    } else if(strcmp(args[0], "ppid")==0){
        printf("Imprime el pid del proceso padre de la shell.\n");
    } else if(strcmp(args[0], "cd")==0){
        printf(" Cambia el directorio de trabajo actual de la shell a dir. Si se invoca sin argsumentos, imprime el directorio de trabajo actual\n");
    } else if(strcmp(args[0], "date")==0){
        printf(" Imprime la fecha actual en formato DD/MM/AAAA y la hora actual en formato hh:mm:ss\n");
    } else if(strcmp(args[0], "historic")==0){
        printf("Muestra el historial de comandos ejecutados por esta shell. \n"
               "\n"
               "historic imprime todos los comandos introducidos con su número de orden. \n"
               "\n"
               "historic N repite el comando número N (de la lista de históricos). \n"
               "\n"
               "historic -N imprime solo los últimos N comandos.");
    } else if(strcmp(args[0], "open")==0){
        printf(" Abre un archivo y lo añade (junto con el descriptor de archivo y el modo de apertura) a la lista de archivos abiertos por la shell\n");
    } else if(strcmp(args[0], "close")==0){
        printf("Cierra el descriptor de archivo df y elimina el ítem correspondiente de la lista.\n");
    } else if(strcmp(args[0], "dup")==0){
        printf("Duplica el descriptor de archivo df, creando la correspondiente nueva entrada en la lista de archivos.\n");
    } else if(strcmp(args[0], "infosys")==0){
        printf("Imprime información sobre la máquina que ejecuta la shell. ");
    } else if(strcmp(args[0], "help")==0){
        printf("help muestra una lista de comandos disponibles. help cmd ofrece una breve descripción del uso del comando cmd\n");
    } else if((strcmp(args[0], "quit") || strcmp(args[0], "bye") || strcmp(args[0], "exit"))==0){
        printf("Finaliza la shell.\n");
    }
}
void Cmd_dup(char *tr[], ABIERTOLISTA *abiertos) {
    int df, duplicado;
    FILES elemento, nuevoElemento;
    char aux[512], *p = NULL;

    // Validar si el descriptor de archivo proporcionado es correcto
    if (tr[1] == NULL || (df = atoi(tr[1])) < 0) {
        // No hay parámetro o el descriptor es menor que 0
        listar_abiertos(*abiertos);
        return;
    }

    // Duplicar el descriptor de archivo
    duplicado = dup(df);
    if (duplicado == -1) {
        perror("Error al duplicar el descriptor de archivo");
        return;
    }

    // Encontrar el nombre del archivo asociado al descriptor original (df)
    for (TNODOLISTA nod = primero(*abiertos); nod != fin(*abiertos); nod = siguiente(*abiertos, nod)) {
        recupera(*abiertos, nod, &elemento);
        if (elemento.filedes == df) {
            p = elemento.filename;  // Asignar el nombre del archivo encontrado a 'p'
            break;
        }
    }

    if (p == NULL) {
        fprintf(stderr, "Error: No se pudo encontrar el archivo asociado al descriptor %d\n", df);
        close(duplicado); // Cerrar el descriptor duplicado si no se encuentra el original
        return;
    }
    sprintf(aux, "dup %d (%s)", df, p);

    nuevoElemento.filedes = duplicado;
    strcpy(nuevoElemento.filename, aux);         // Copiar la descripción del archivo duplicado
    nuevoElemento.mode = fcntl(duplicado, F_GETFL);

    if (nuevoElemento.mode == -1) {
        perror("Error al obtener las banderas del archivo duplicado");
        close(duplicado); // Cerrar el descriptor duplicado si hay un error
        return;
    }

    // Insertar el nuevo elemento en la lista de archivos abiertos
    inserta(abiertos, fin(*abiertos), nuevoElemento);
}


void file_start(ABIERTOLISTA *abiertos){
    FILES elemento;
    TNODOLISTA dndfile=primero(*abiertos);
    int df;
    //Para entrada
    elemento.filedes = STDIN_FILENO;
    strcpy(elemento.filename, "entrada estandard");
    df = fcntl(STDIN_FILENO, F_GETFL);
    elemento.mode=df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);

    //Para salida
    elemento.filedes = STDOUT_FILENO;
    strcpy(elemento.filename,"salida estandard");
    df=fcntl(elemento.filedes,F_GETFL);
    elemento.mode = df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);

    //Para error
    elemento.filedes = STDERR_FILENO;
    strcpy(elemento.filename,"error estandard");
    df=fcntl(elemento.filedes,F_GETFL);
    elemento.mode = df;
    inserta(abiertos,dndfile,elemento);
    dndfile = siguiente(*abiertos,dndfile);
}

void Cmd_cwd() {
    char wd[PATH_MAX];
    if (getcwd(wd, sizeof(wd)) != NULL) {
        printf("%s\n", wd);
    } else {
        perror("Error al obtener el directorio de trabajo"); // Manejo de errores
    }
}

int main(int argc, char** argv) {
    COMMAND c;
    FILES f;
    pid_t pid;
    DIR *dir;
    struct stat buf;
    struct dirent *ent;
    int counter, control, df;
    char *args[20], wd[40], path[256];
    char *input = malloc(sizeof(char) * 30);
    ABIERTOLISTA abiertos;
    HIST historial;
    crea(&abiertos);
    creahist(&historial);
    file_start(&abiertos);
    TNODOHIST dndhist = primerohist(historial);
    TNODOLISTA dndfile = fin(abiertos);
    while(1) {
        printf("->");
        fgets(input, 20, stdin);
        counter = TrocearCadena(input,args);//Trocea "input" en el array de strings "args"
        if (counter != 0) {
            if (strcmp(args[0], "date") == 0) {//Comando de fechas falta el caso de sin argumentos
                if (args[1] != NULL && strcmp(args[1], "-t")==0) {
                    fechat();//Formato hh:mm:ss
                } else if (args[1] != NULL && strcmp(args[1],"-d")==0){
                    fechad();//Formato DD/MM/YYYY
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
                }else{//Si recibimos argumentos, cambiar al directorio si existe
                    if(chdir(args[1])==-1){
                        printf("Error al encontrar el directorio %s\n",args[1]);
                    }
                    else{
                        printf("Exitoso cambio a %s\n",args[1]);
                    }
                }
            }
            else if(strcmp(args[0],"open")==0){//Comando Open
                Cmd_open(args,abiertos,&f,&control);//Guarda en "e" los elemetnos del archivo abierto
                if(control==1){
                    inserta(&abiertos,dndfile,f);//Insertamos los elementos del archivo en la lista "abiertos"
                    dndfile = siguiente(abiertos,dndfile);
                }
            }else if (strcmp(args[0],"close")==0){//Comando Close
                Cmd_close(args,&abiertos);
                dndfile = fin(abiertos);
                printf("Close\n");
            }
            else if(strcmp(args[0],"dup")==0){
                Cmd_dup(args,&abiertos);
            }
            else if (strcmp(args[0],"infosys")==0){//Informacion del Sistema
                infosys();
            }
            else if(strcmp(args[0],"authors")==0){//Autores de la practica
                if(counter == 1){
                    authors();//Devuelve nombres y correos
                }else{
                    if(strcmp(args[1],"-l")==0){
                        authorsl();//Devuelve nuestros correos
                    }else if(strcmp(args[1],"-n")==0){
                        authorsn();//Devuelve nuestros nombres
                    }
                    else{
                        perror("No se encontró tu comando\n");
                    }
                }
            }else if(strcmp(args[0],"help")==0){
                if(args[1] != NULL){
                    help_cmd(args);//Ayuda con argumento incluido
                } else help();//Imprime por pantalla todos los comandos disponibles
            }
            else if(strcmp(args[0],"historic")==0){
                phistorics(historial,args[1]);//Acceso a la lista de comandos introducidos
            }else if(strcmp(args[0],"makefile")==0){
                if (args[1]==NULL){
                    getcwd(wd,sizeof(wd));
                    printf("%s\n",wd);
                }else{
                    df = creat(args[1],O_RDWR);
                    strcpy(f.filename,args[1]);
                    f.mode = O_RDWR;
                    f.filedes = df;
                    inserta(&abiertos,fin(abiertos),f);
                }
            }else if(strcmp(args[0],"makedir")==0){
                if (args[1]==NULL){
                    getcwd(wd,sizeof(wd));
                    printf("%s\n",wd);
                }else{
                    mkdir(args[1],0777);
                    opendir(args[1]);
                }
            }else if (strcmp(args[0],"listdir")==0){
                //primero directorio y luego argumentos
                if(args[1]!=NULL){
                    if (strcmp(args[1],"-hid")==0){

                    }else if (strcmp(args[1],"-acc")==0){
                    }else if (strcmp(args[1],"-link")==0){

                    }else{
                        dir = opendir(args[1]);
                        ent = readdir(dir);
                        if(dir == NULL){
                            perror("No se pudo abrir directorio.\n");
                            continue;
                        }
                        while((ent = readdir(dir)) != NULL){
                            if(stat(args[1],&buf) == -1){
                                perror("Error al abrir el archivo\n");
                                continue;
                            }
                            printf("%lu\t%s\n",ent->d_ino,ent->d_name);
                            //sino buf.st_ino/buf.st_size
                        }
                        closedir(dir);
                    }
                }else{
                    printf("Directorio actual.\n");
                }

            }else if (strcmp(args[0], "cwd") == 0) {
                if (args[1] != NULL) {
                    fprintf(stderr, "Error: El comando 'cwd' no acepta argumentos.\n");
                } else {
                    char wd[PATH_MAX];
                    if (getcwd(wd, sizeof(wd)) != NULL) {
                        printf("%s\n", wd);
                    } else {
                        perror("Error al obtener el directorio de trabajo");
                    }
                }
            }
            else if (strcmp(args[0],"exit")==0 || strcmp(args[0],"bye")==0 || strcmp(args[0],"quit")==0) {//Sale del shell
                printf("Saliendo del shell...\n");
                free(input);//Al salir liberamos memoria
                destruye(&abiertos);
                destruyehist(&historial);
                break;
            }
            else {
                printf("Comando no reconocido: %s\n", args[0]);
            }
            historics(args,historial,&c);//Con cada iteracion del shell, guardamos en la lista de historial los comandos introducidos
            insertahist(&historial,dndhist,c);
            dndhist = siguientehist(historial,dndhist);
        }
        else {
            perror("Error al escanear la linea.\n");
        }
    }
}
