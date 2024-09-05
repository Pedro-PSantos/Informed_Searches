//No para usar
class No {
private:
    int geracao;
    int expansao;
    int nivel;
    int tamanho;
    int heuristica;
    int pai;
    int Mapa[9][9];

public:
    No();
    No(int,int,int,int,int,int,int[9][9]);

    int getGeracao(){return geracao;};
    void setGeracao(int n){geracao=n;};
    int getExpansao(){return expansao;};
    void setExpansao(int e){expansao=e;};
    int getNivel(){return nivel;};
    void setNivel(int n){nivel=n;};
    int getHeuristica(){return heuristica;};
    void setHeuristica(int h){heuristica=h;};
    int getTamanho(){return tamanho;};
    void setTamanho(int t){tamanho=t;};
    int getPai(){return pai;};
    void setPai(int p){pai=p;};
    int (*getMapa())[9] {return Mapa;}
    //void setMapa(int M[9][9]);

};
//////////////////////////////////////////////////



