void phistorics(TLISTA history, char *arg){
    int num;
    TNODOLISTA nod, posicion;
    TIPOELEMENTOLISTA e;
    if(arg == NULL){
    for(nod =primero(history); nod != fin(history);nod=siguiente(history,nod)){
        recupera(history,nod,&e);
        printf("N=%d %s\n",e.num,e.cmd);
    }}
    else{
        num = atoi(arg);
        if(num<0){
            num = abs(num);
            posicion=primero(history);
            for(int i=0;i!=longitud(history)-num;i++){
                posicion=siguiente(history,posicion);
            }
            for(nod=posicion; nod != fin(history);nod=siguiente(history,nod)){
                recupera(history,nod,&e);
                printf("%d: %s\n",e.num,e.cmd);
            }
        }else if(num == 0){
            printf("Solo numeros son validos\n");
        }else if(num <20){
            for(nod = primero(history);nod!=fin(history);nod=siguiente(history,nod)){
            recupera(history,nod,&e);
            if(e.num==num){
                printf("%s\n",e.cmd);
                //repeat_cmd();
                break;
            }}
        }else {
            printf("Comando Incorrecto\n");
}}}

