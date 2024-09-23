#include "types.h"

typedef tSong tItemS;

typedef int tPosS;
typedef struct {
    tPosS lastPos;
    tSong data[MAX];
} tListS;

void createEmptyListS (tListS *S);
bool isEmptyListS (tListS* S);
tPosS firstS (tListS S);
tPosS lastS (tListS S);
tPosS nextS (tPosS P, tListS S);
tPosS previousS (tItemS I, tListS S);
bool insertItemS (tItemS I, tPosS P, tListS S);
void deleteAtPositionS (tPosS P, tListS S);
tSong getItemS (tPosS P, tListS S);
void updateItemS (tItemS I, tPosS P, tListS S);
tPosS findItemS (tSongTitle ST, tListS S);
