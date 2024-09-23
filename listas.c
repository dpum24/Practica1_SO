#include "song_list.h"


void createEmptyListS (tListS *L){
    L->lastPos = NULLS;//Crea una lista vacía
}

//createEmptyList (tListS) → tListS
//Objetivo: Inicializa una lista vacía
//Entradas:
//Salidas: Una lista vacía
//Precondiciones:
//Poscondiciones: Queda una lista vacía
bool isEmptyList (tListS L){
    return L.lastPos == NULLS;//Si lastPos vale -1 entonces la lista está vacia y devuelve true, si no devuelve false
}
//isEmptyList (tListS) → bool
//Objetivo: Valorar si una lista está o no vacía
//Entradas: Una lista
//Salidas: Devuelve TRUE si está vacia, si no FALSE
//Precondiciones: Tiene que haber una lista inicializada
//Poscondiciones:

tPosS firstS (tListS L){
    return 0;//La primera posición de una lista siempre es 0
}
//first (tListS) → tPosS
//Objetivo: Devuelve la posición del primer elemento de la lista
//Entradas:
//Salidas: La posición del primer elemento de la lista
//Precondiciones: La lista no puede estar vacia
//Poscondiciones:


tPosS lastS (tListS L){
    return L.lastPos;//lastPos tiene el valor del último elemento de la lista
}
//last (tListS) → tPosS
//Objetivo: Devuelve la posición del último elemento de la lista
//Entradas:
//Salidas: La posición del último elemento de la lista
//Precondiciones: La lista no puede estar vacia
//Poscondiciones:


tPosS nextS (tPosS P, tListS L){
    if(P > 0 && P < L.lastPos){//P tiene que estar entre el primer y último valor de la lista
        return P+1;
    }
    return NULLS;
}
//next (tPosS, tListS) → tPosS
//Objetivo: Devuelve la posición siguiente a la actual
//Entradas: Una lista y la posición del elemento actual
//Salidas: La siguiente posición o LNULL
//Precondiciones: La posición indicada es una posición válida en la lista
//Poscondiciones:


tPosS previous (tPosS P, tListS L){
    if(P > 0 && P < L.lastPos){//P tiene que estar entre el primer y último valor de la lista
        return P-1;
    }
    return NULLS;
}
//previous (tPosS, tListS) → tPosS
//Objetivo: Devuelve la posición anterior a la actual
//Entradas: Una lista y la posición del elemento actual
//Salidas: La anterior posición o LNULL
//Precondiciones: La posición indicada es una posición válida en la lista
//Poscondiciones:

bool insertItem (tItemS S, tPosS P, tListS *L){
    if (P==NULLS){
        return insertItemS(S, L->lastPos+2, *L);
    }
    if (P<1||P>MAX||L->lastPos==MAX-1){//Devuelve false si la posición introducida no es válida
        return false;
    }
    int truePos = P-1;
    for(int i=L->lastPos; i>=truePos; i--){//Al añadir una posición todos los puestos posteriores se mueven a la derecha
        L->data[i+1] = L->data[i];
    }
    L->data[truePos] = S;//Añade la posición
    L->lastPos++;
    return true;
}
//insertItem (tItemS, tPosS, tListS) → tList, bool
//Objetivo: Inserta un elemento en la lista antes de la posición indicada. Si la posición es LNULL, entonces se añade al final.
//Entradas: Una lista, la posición del elemento actual y el contenido del elemento a insertar
//Salidas:  Devuelve un valor true si el elemento fue insertado; false en caso contrario.
//Precondiciones: La posición indicada es una posición válida en la lista o bien nula (LNULL)
//Poscondiciones:  Las posiciones de los elementos de la lista posteriores a la del elemento insertado pueden haber variado


void deleteAtPosition (tPosS P, tListS *L){
    for(int i=P; i<L->lastPos; i++){
        L->data[i]=L->data[i+1];//Al eliminar una posición, todas las posiciones de la derecha tienen que moverse hacia la izquierda
    }
    L->lastPos--;//lastPos baja una unidad
}
//deleteAtPosition (tPosL, tList) → tList
//Objetivo: Elimina de la lista el elemento que ocupa la posición indicada.
//Entradas: Una lista y una posición indicada
//Salidas: Una lista sin el elemento indicado
//Precondiciones: La posición indicada es una posición válida en la lista.
//Poscondiciones:   Las posiciones de los elementos de la lista posteriores a la de la posición eliminada pueden haber variado.


tSong getItemS (tPosS P, tListS L){
    return L.data[P];//Devuelve el contenido de la posición introducida
}
//getItem (tPosL, tList) → tItemL
//Objetivo: Devuelve el contenido del elemento que ocupa la posición indicada.
//Entradas: Una lista y una posición indicada
//Salidas: El contenido de la posición
//Precondiciones: La posición indicada es una posición válida en la lista.
//Poscondiciones:


void updateItem (tItemS I, tPosS P, tListS *L){
    L->data[P]=I;//Cambia el contenido de la posición introducida
}
//updateItem (tItemL, tPosL, tList) → tList
//Objetivo: Modifica el contenido del elemento situado en la posición indicada
//Entradas: Una lista, una posición y el contenido a introducir
//Salidas: Lista con un elemento modificado
//Precondiciones: La posición indicada es una posición válida en la lista
//Poscondiciones: El orden de los elementos de la lista no se ve modificado.

tPosS findItemS (tUserName U, tListS S) {
    if (S.lastPos == NULLS)
        return NULLS;
    bool igual;
    for (int i = 0; i <= S.lastPos; i++) {
        igual = true;
        for (int j = 0; j < NAME_LENGTH_LIMIT; j++) {
            if (U[j] != S.data[i].songTitle[j]) {
                igual = false;//Comprueba que los carácteres coinciden uno a uno
            }
        }
        if (igual)
            return i;//Si es así, devuelve la posición del valor introducido
    }
    return NULLS;//Si no, devuelve nulo
}
//findItem (tUserName, tList) → tPosL
//Objetivo: Devuelve la posición del primer elemento de la lista cuyo nombre de usuario se corresponda con el indicado (o LNULL si no existe tal elemento)
//Entradas: Una lista y un nombre de usuario
//Salidas: La posición del elemento
//Precondiciones: Tiene que haber una lista inicializada
//Poscondiciones:
