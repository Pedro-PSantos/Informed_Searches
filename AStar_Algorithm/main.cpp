#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include "no.h"


/*
//                  Abordagem Construtiva
//
//Evitar trocas duas vezes para a mesma zona habitacional.
//
//Se a solução passa por mover duas zonas habitacionais, é
//indiferente mover uma primeiro que a outra.
//
*/

/*
//                  Abordagem Melhorativa
//
//Apenas faz sentido mover uma zona habitacional apenas uma
//só vez para uma zona livre.
//
//Colocar violações (zonas habitacionais sem acesso a uma das portas),
//com uma penalização na função de avaliação.
//
*/

/*
//      Validade de Solucao
//
//Construir um método para detetar
//
*/


using namespace std;





//Estruturas de Dados com os nos
vector<No> Caminho;
vector<No> Gerados;
vector<No> PorVerificar;
//////////////////////////////////////////////////



//Variaveis para guardar o nro de cada tipo de zona
int zonashabitacionais=0;
int zonascirculacao=0;
int zonasinutilizaveis=0;
//////////////////////////////////////////////////



//Variavel para guardar os mapas das cidades
//  1 = + (Zona Habitacional)
//  0 = . (Zona de Circulacao)
// -1 = # (Muralha/Nao pode haver construcao)
int mapa[9][9];
int mapaaux[9][9];
int Instancias[10][9][9]={ 
// 5x5
{{0,1,1,1,0,},{1,1,0,1,1,},{0,1,-1,-1,0,},{1,1,0,1,0,},{1,1,0,0,0,},},
{{0,1,1,0,0,},{1,1,-1,1,1,},{0,-1,-1,-1,0,},{1,1,-1,1,0,},{0,1,0,1,1,},},
{{0,1,1,0,0,},{1,-1,0,1,1,},{1,-1,-1,-1,0,},{0,1,1,-1,0,},{0,1,0,1,1,},},
{{1,1,1,1,0,},{1,1,0,0,1,},{1,0,1,0,1,},{1,0,0,1,1,},{0,1,1,1,1,},},
// 7x7
{{1,1,1,1,0,1,1,},{0,1,0,0,0,0,0,},{1,1,0,1,0,-1,1,},{0,1,0,0,1,-1,0,},{1,-1,-1,-1,1,-1,1,},{1,0,0,0,1,0,1,},{0,0,1,0,0,0,0,},},
{{0,1,0,0,0,1,0,},{0,1,0,1,1,1,0,},{1,0,-1,-1,-1,0,1,},{0,1,-1,-1,-1,1,0,},{1,1,-1,-1,-1,0,1,},{0,1,1,0,1,0,1,},{0,0,1,1,0,1,0,},},
{{1,1,1,1,1,1,1,},{1,1,0,0,0,1,1,},{1,0,1,0,1,0,1,},{1,1,0,0,0,1,1,},{1,0,1,0,1,0,1,},{1,1,0,0,0,1,1,},{1,1,1,1,1,1,1,},},
// 9x9
{{0,1,1,1,0,-1,0,0,0,},{1,1,0,1,1,-1,1,1,1,},{0,1,-1,-1,0,0,0,0,0,},{1,1,0,-1,-1,-1,0,1,0,},{1,1,0,-1,0,-1,0,1,0,},{1,0,0,0,0,1,0,1,0,},{1,0,-1,-1,-1,1,0,1,0,},{0,1,0,1,0,1,0,0,0,},{0,0,0,1,0,0,0,0,0,},},
{{0,1,1,1,0,0,0,0,-1,},{1,1,-1,1,1,1,1,1,-1,},{0,1,-1,1,0,1,-1,0,-1,},{-1,-1,-1,1,0,1,-1,1,0,},{1,1,0,1,0,1,-1,1,0,},{1,0,-1,-1,-1,-1,-1,1,0,},{1,0,1,0,0,0,-1,1,-1,},{0,1,0,1,0,1,-1,0,-1,},{0,0,0,1,0,0,0,0,-1,},},
{{1,1,1,1,0,1,1,1,1,},{1,0,1,1,0,1,1,0,1,},{1,1,0,1,0,1,0,1,1,},{1,1,1,1,0,1,1,1,1,},{0,0,0,0,1,0,0,0,0,},{1,1,1,1,0,1,1,1,1,},{1,1,0,1,0,1,0,1,1,},{1,0,1,1,0,1,1,0,1,},{1,1,1,1,0,1,1,1,1,},},
};
//////////////////////////////////////////////////



//Par Movimento Horizontal/Vertical na direção dos ponteiro do relógio.
//Começa com o movimento para cima(-1 vertical, 0 horizontal).
int movimentohorizontal[8]={0,1,1,1,0,-1,-1,-1};
int movimentovertical[8]={-1,-1,0,1,1,1,0,-1};
//////////////////////////////////////////////////



//Variaveis auxiliares para cada mapa e para aceder ao mapa escolhido
int TamanhoMapa[10] = {5, 5, 5, 5, 7, 7, 7, 9, 9, 9 };
int SizeMap=0;
int IndexMapa=0;
//////////////////////////////////////////////////



//Variaveis para guardar acessos as portas
int VisitaPortas[4]={0,0,0,0};//Norte,Sul,Este,Oeste
string Portas[4]={"Norte","Sul","Este","Oeste"};
bool N=false,S=false,E=false,W=false;
int portas[4][2];
//////////////////////////////////////////////////



//Variaveis para resolucao do problema
float Tempo = 0;
int maxTempo=60;//em segundos
int maxAvaliacoes=100000;
int avaliacoes=0;
int nivel=0;//neste caso ira equivaler ao nro de movimentos feitos
int geracao=1;
int expansao=1;
int movimentos=0;
int heuristica=0;
int custoMovimento=1;
int custoInacessivel=100;
int resolucaoreferencia[10]={3,4,4,4,3,2,9,11,12,13};
//////////////////////////////////////////////////



//Variavel para guardar o tempo de execucao
clock_t test=0;
//////////////////////////////////////////////////



//Funcao para inicializar os mapas
void InitMapas(){

    SizeMap=TamanhoMapa[IndexMapa];

    for(int i=0 ,j=0;i<SizeMap;i++){
        for(j=0;j<SizeMap;j++){
            mapa[i][j]=Instancias[IndexMapa][i][j];
            mapaaux[i][j]=Instancias[IndexMapa][i][j];
            if(mapa[i][j]==1)
                zonashabitacionais++;
            else if(mapa[i][j]==0)
                zonascirculacao++;
            else if(mapa[i][j]==-1)
                zonasinutilizaveis++;
            
        }
    }

    int InitPortas[4][2] = {{-1,SizeMap/2},//Norte
                            {SizeMap,SizeMap/2},//Sul
                            {SizeMap/2,SizeMap},//Este
                            {SizeMap/2,-1}};//Oeste



    for(int i=0;i<4;i++)
       for(int j=0;j<2;j++)
            portas[i][j]=InitPortas[i][j];
       

}

bool CompararFilhos(No Filho1,No Filho2){//Compara pelo valor de heuristica qual o filho gerado mais recentemente
    
    if(Filho1.getHeuristica()<Filho2.getHeuristica())//Se heuristica menor, verdadeiro 
        return true;
    else if(Filho1.getHeuristica()==Filho2.getHeuristica()&&Filho1.getGeracao()<Filho2.getGeracao())
    //caso contrario, se heuristica igual => se geracao menar(= criado primeiro), verdadeiro
        return true;
    else
        return false;
}

//Funcao para repor o mapa em origem
void ReporMapaAux(){
    for(int i=0;i<SizeMap;i++){
        for(int j=0;j<SizeMap;j++){
            mapaaux[i][j]=mapa[i][j];
        }
    }
}

//Funcao para repor os acessos as portas
void ReporCheckPortas(){
    N=false;
    S=false;
    E=false;
    W=false;
}

void ReporValorPortas(){
    VisitaPortas[0]=VisitaPortas[1]=VisitaPortas[2]=VisitaPortas[3]=0;
}



void CheckPortas(int x,int y){//Verifica se a posicao atual e uma porta

    if(x-1==portas[0][0]&&(y-1==portas[0][1]||y==portas[0][1]||y+1==portas[0][1])&&N==false){//adjacente a porta norte, return 1
        N=true;
        VisitaPortas[0]++;
    }
    
    if(x+1==portas[1][0]&&(y-1==portas[1][1]||y==portas[1][1]||y+1==portas[1][1])&&S==false){//adjacente a porta sul, return 2
        S=true;
        VisitaPortas[1]++;
    }
    
    if(y+1==portas[2][1]&&(x-1==portas[2][0]||x==portas[2][0]||x+1==portas[2][0])&&E==false){//adjacente a porta este, return 3
        E=true;
        VisitaPortas[2]++;
    }

    if(y-1==portas[3][1]&&(x-1==portas[3][0]||x==portas[3][0]||x+1==portas[3][0])&&W==false){//adjacente a porta oeste, return 4
        W=true;
        VisitaPortas[3]++;
    }
}

bool CheckBounds(int x,int y){//Verifica se a posicao atual esta dentro dos limites do mapa

    bool check = false;

    if(x>=0 && x<SizeMap)
        if(y>=0 && y<SizeMap)
            check = true;

    return check;
}

bool CompararGerados(int Map[9][9]){
    

    int checksame=0;
    vector<No>::iterator it;

    for(it=Gerados.begin();it!=Gerados.end();it++,checksame=0){
        for(int i=0;i<SizeMap;i++)
            for(int j=0;j<SizeMap;j++)
                if((*it).getMapa()[i][j]==Map[i][j]){
                    checksame++;
                }
        if(checksame==SizeMap*SizeMap){
            if(nivel>=(*it).getNivel())
            return true;
        }
    }
    
    return false;
    
}


void CheckPath(int i,int j){


    CheckPortas(i,j);
    if(N==true&&S==true&&E==true&&W==true){//se todas as portas estiverem encontradas, retorna
        return;
    }

    for(int k=0;k<8;k++){

        int y=j+movimentohorizontal[k]; // y = largura
        int x=i+movimentovertical[k]; // x = altura
        
        if(CheckBounds(x,y)&&mapaaux[x][y]==0){//verifica se está dentro dos limites do mapa e é de circulacao
            mapaaux[x][y]=1;//Desmarca a zona de circulação para não ser contada novamente
            CheckPath(x,y);//Chamada recursiva para verificar as zonas adjacentes    
        }
    }

}


int ValorHeuristica(){

    avaliacoes++;

    int porta = 0;
    for(int i=0;i<SizeMap;i++){
        for(int j=0;j<SizeMap;j++){
            if(mapaaux[i][j]==1){
                CheckPath(i,j);
                ReporMapaAux();
                ReporCheckPortas();   
            }
        }
    }

    int ContadorportaMenosAcessosVezes = VisitaPortas[0];// inicia Contador na Porta Norte
    string portaMenosAcessos = "Norte";

    for(int i = 0; i<3;i++)//Itera pelo resto das portas
        if(VisitaPortas[i+1]<ContadorportaMenosAcessosVezes){
            ContadorportaMenosAcessosVezes = VisitaPortas[i+1];//Guarda o numero de vezes que a porta menos visitada foi visitada
            portaMenosAcessos = Portas[i+1];//Guarda o nome da porta menos visitada
        }

    //CODIGO COMENTADO PARA SABER INFORMACAO SOBRE QUAL A PORTA MENOS VISITADA
    //cout << "Porta menos visitada e a porta " << portaMenosAcessos << endl;

    /*cout << "As portas tem:\n" << Portas[0] << ":" << VisitaPortas[0] << " acessos, " 
    << Portas[1] << ":" << VisitaPortas[1] << " acessos, " 
    << Portas[2] << ":" << VisitaPortas[2] << " acessos. " 
    << Portas[3] << ":" << VisitaPortas[3] << " acessos.\n"; */

    ReporValorPortas();

    return (zonashabitacionais-ContadorportaMenosAcessosVezes) * custoInacessivel;
}

void Algoritmo(No Pai){//Algoritmo A* (AStar) Custo Total = Custo do no + Custo de alcancaro o no

    int objetivoreferencia=resolucaoreferencia[IndexMapa];

    Gerados.push_back(Pai);//Adiciona o no pai aos nos gerados
    PorVerificar.push_back(Pai);//Adiciona o no pai aos nos por verificar
    
        

    int custo=0;

    while(!PorVerificar.empty()){//Gerar todos os estados filhos possíveis
        
        
        while(heuristica <= PorVerificar.front().getNivel())
            PorVerificar.erase(PorVerificar.begin());

        No atual = PorVerificar.front();//Retira o primeiro no da lista de nos por verificar
        PorVerificar.erase(PorVerificar.begin());//Remove o primeiro no da lista de nos por verificar  

        expansao++;
        nivel=atual.getNivel()+1;
        int gerados=0;

        for(int i = 0 ; i < SizeMap ; i++)//copia mapa do no atual para o mapa auxiliar
            for(int j = 0; j < SizeMap; j++)
                mapa[i][j]=mapaaux[i][j]=atual.getMapa()[i][j];

        
        //Geracao de todos os vizinhos do estado atual
        for(int i = 0; i < SizeMap; i++)//busca zona habitacional
            for(int j = 0; j < SizeMap; j++)
                if(mapaaux[i][j]==1)//se econtrar zona habitacional
                    for(int k = 0; k < SizeMap; k++)//procura zona de circulacao
                        for(int l = 0; l < SizeMap; l++)
                            if(mapaaux[k][l]==0){//se encontrar zona de circulacao
                                mapa[k][l]=mapaaux[k][l]=1;//troca zona de circulacao
                                mapa[i][j]=mapaaux[i][j]=0;//por zona habitacional

                                
                                Tempo = (float)(clock()-test)/CLOCKS_PER_SEC;
                                if(Tempo > 60 || avaliacoes > maxAvaliacoes){//verifica se ja passou o tempo limite
                                    std::cout << "Erro. Passou os 60 segundos ou o Maximo de 100000 avaliacoes.\n";
                                    return;
                                }
                            
                                custo=ValorHeuristica();

                                if(!CompararGerados(mapaaux) && nivel <= heuristica){//&&(custo<=heuristica)){//Verifica se o mapa ja foi gerado
                                //Caso o mapa ainda não tenha sido gerado, gera um novo no
                                    geracao++;
                                    int o = 1;
                                   
                                    if(custo+nivel<heuristica){//Guarda o valor de heuristica mais baixo
                                        heuristica=custo+nivel;
                                        }

                                    

                                    No Vizinho = No(geracao,expansao,nivel,
                                    SizeMap,custo,atual.getGeracao(),mapaaux);
                                    gerados++;

                                    Gerados.push_back(Vizinho);
                                    PorVerificar.push_back(Vizinho);
                                    
                                    if(custo + nivel <= objetivoreferencia)
                                    {
                                        cout << "Melhor caminho encontrado.\n";
                                        return;
                                    }
                                }
                                    //cout << "Mapa ja gerado\n"; 
                                // volta a por o mapa como estava
                                mapa[k][l]=mapaaux[k][l]=0;
                                mapa[i][j]=mapaaux[i][j]=1;
                            }

        if(gerados!=0){//Se hoverem gerados
            sort(PorVerificar.begin(),PorVerificar.end(),CompararFilhos);//Ordena os nos por verificar por ordem crescente de custo
        }
    }
}


int main(){


    

    cout << "Bem vindo ao programa do problema urbanístico de uma cidade muralhada medieval.\n";
    cout << "O programa ira modificar o mapa de forma a que todas as Portar sejam acessíveis.\n";

    cout << "Insira o numero do mapa que pretende resolver:\n";

    cin >> IndexMapa;

    IndexMapa--;//Para aceder ao mapa correto

    cout << "Mapa escolhido: " << IndexMapa+1 << endl;
  
    test = clock();
    
    InitMapas();

    for(int i=0;i<SizeMap;i++){
        for(int j=0;j<SizeMap;j++){
            if(mapa[i][j]==-1)
                cout << "#";
            else
                cout << mapa[i][j];
        }
        cout << endl;
    }

    heuristica=ValorHeuristica();

    cout << "Heuristica inicial: " << heuristica << endl;

    No Pai = No(geracao,expansao,nivel,SizeMap,heuristica,0,mapaaux);//Cria o no pai

    Algoritmo(Pai);

    cout << "\n\n";
    cout << "A fazer caminho...\n";

    No caminho = Gerados.back();

    for(;caminho.getPai()!=0;){
        Caminho.push_back(caminho);
        caminho=Gerados[caminho.getPai()-1];
    }
        Caminho.push_back(Pai);

    
    int size = Caminho.size();
    
    for(int aux = 0;aux < size;aux++){
        No atual = Caminho.front();
        Caminho.erase(Caminho.begin());

        for(int i=0;i<SizeMap;i++){
        for(int j=0;j<SizeMap;j++)
            if(atual.getMapa()[i][j]==-1)
                cout << "#";
            else
                cout << atual.getMapa()[i][j];
        cout << endl;
    }

    cout << "Geracao:" << atual.getGeracao() << " Expansao:" << atual.getExpansao() << " Nivel:" << atual.getNivel() << " Heuristica:" << atual.getHeuristica()+atual.getNivel() << "\n\n";
    }


    
    Tempo = (float)(clock()-test);
    float Tempo2 = Tempo/CLOCKS_PER_SEC;
    
                                

    cout << "\n Numero de Avaliacoes: " << avaliacoes << endl;
    cout << "\n Numero de Gerados: " << Gerados.size() << endl;
    cout << "\n Custo Final: " << /*Gerados.back().getHeuristica()+Gerados.back().getNivel()*/heuristica << endl;
    cout << "\nTempo de execucao: " << Tempo << "MS -- " << Tempo2 << "S" << endl;


    return 0;
}





