#include "no.h"

No::No(){
    geracao=0;
    expansao=0;
    nivel=0;
    tamanho=0;
    heuristica=0;
    pai=0;
}
No::No(int g,int e,int n,int t,int h,int p,int m[9][9]){

    geracao=g;
    expansao=e;
    nivel=n;
    tamanho=t;
    heuristica=h;
    pai=p;

    for (int i = 0; i < tamanho; i++) 
        for (int j = 0; j < tamanho; j++) 
            Mapa[i][j] = m[i][j];

}


