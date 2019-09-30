#include <iostream>
#include <queue>

using namespace std;

class GrafoMatriz{

    int v;
    int** mv;
    int dfsCounterPreOrdem;
    int dfsCounterPosOrdem;
    int bfsCounter;

    public:
        //Funções básicas
        GrafoMatriz(int);
        void adicionarAresta(int, int);
        void verGrafo();
        //Busca em profundidade
        int* dfs(bool);
        void dfsR(int, int[], int[]);
        void verDfs(bool);
        //Busca em largura
        int* bfs(int);
        void verBfs(int);
        //Verifica se há ciclos
        bool hasCicle();
        //Numeração topologica de um grafo
        int* getNumeracaoTopologica();
        void verNumeracaoTopologica();
        bool isGrafoTopologico();
        //Caminho de um grafo com peso 1 nas arestas
        int* menorCaminhoSemPeso(int);
        void verMenorCaminhoSemPeso(int);
        //Subgrafos
        tuple<int, int*> getComponentesConexas();
        void getComponentesConexasR(int, int[], int);
        void verComponentesConexas();

};

GrafoMatriz::GrafoMatriz(int v){
    this->v = v;
    this->mv = new int*[v];
    for (int i = 0; i < v; i++){
        this->mv[i] = new int[v];
        for (int j = 0; j < v; j++)
            this->mv[i][j] = 0;
    }
}

void GrafoMatriz::adicionarAresta(int o, int d){
    this->mv[o][d] = 1;
}

void GrafoMatriz::verGrafo(){
    for (int i = 0; i < this->v; i++){
        cout << i << ": ";
        for (int j = 0; j < this->v; j++)
            if (this->mv[i][j] != 0)
                cout << j << " ";
        cout << endl;
    }
}

int* GrafoMatriz::dfs(bool returnPreOrdem = true){
    this->dfsCounterPosOrdem = this->dfsCounterPreOrdem = 0;
    int* preOrdem = new int[this->v];
    int* posOrdem = new int[this->v];
    for (int i = 0; i < this->v; i++)
        preOrdem[i] = posOrdem[i] = -1;

    for (int i = 0; i < this->v; i++)
        if (preOrdem[i] == -1)
            dfsR(i, preOrdem, posOrdem);

    return returnPreOrdem ? preOrdem : posOrdem;
}

void GrafoMatriz::dfsR(int w, int preOrdem[], int posOrdem[]){
    preOrdem[w] = this->dfsCounterPreOrdem++;
    for (int i = 0; i < this->v; i++)
        if (this->mv[w][i] != 0 && preOrdem[i] == -1)
            dfsR(i, preOrdem, posOrdem);
    posOrdem[w] = this->dfsCounterPosOrdem++;
}

void GrafoMatriz::verDfs(bool returnPreOrdem = true){
    int* visita = dfs(returnPreOrdem);
    cout << "Ordem dos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << i << " ";
    cout << endl;
    cout << "Ordem de acesso dos vertices por DFS -> ";
    for (int i = 0; i < this->v; i++)
        cout << visita[i] << " ";
    cout << endl;
}

int* GrafoMatriz::bfs(int w){
    this->bfsCounter = 0;
    int* visita = new int[this->v];
    for (int i = 0; i < this->v; i++)
        visita[i] = -1;
    visita[w] = this->bfsCounter++;
    queue<int> qm;
    qm.push(w);
    while(!qm.empty()){
        int atual = qm.front();
        qm.pop();
        for (int i = 0; i < this->v; i++)
            if (this->mv[atual][i] != 0 && visita[i] == -1){
                qm.push(i);
                visita[i] = this->bfsCounter++;
            }
    }
    return visita;
}

void GrafoMatriz::verBfs(int w){
    int* visita = bfs(w);
    cout << "Ordem dos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << i << " ";
    cout << endl;
    cout << "Ordem de acesso dos vertices por BFS -> ";
    for (int i = 0; i < this->v; i++)
        cout << visita[i] << " ";
    cout << endl;
}

bool GrafoMatriz::hasCicle(){
    int* posOrdem = dfs(false);
    for (int i = 0; i < this->v; i++)
        for (int j = 0; j < this->v; j++)
            if (this->mv[i][j] != 0 && posOrdem[i] < posOrdem[j])
                return true;
    return false;
}

int* GrafoMatriz::getNumeracaoTopologica(){
    //Return -1 no vetor de retorno se nao houver numeração topologica
    int counterTopo = 0;
    int* topo = new int[this->v];
    int* grauEntrada = new int[this->v];
    for (int i = 0; i < this->v; i++){
        topo[i] = -1;
        grauEntrada[i] = 0;
    }
    for (int i = 0; i < this->v; i++)
        for (int j = 0; j < this->v; j++)
            if (this->mv[i][j] != 0)
                    grauEntrada[j]++;
    
    int* verticesVirtuais = new int[this->v];
    int comeco = 0, fim = 0;
    for (int i = 0; i < this->v; i++)
        if (grauEntrada[i] == 0)
            verticesVirtuais[fim++] = i;
    while (comeco < fim){
        int atual = verticesVirtuais[comeco++];
        topo[atual] = counterTopo++;
        for (int i = 0; i < this->v; i++)
            if (this->mv[atual][i] != 0){
                grauEntrada[i]--;
                if (grauEntrada[i] == 0)
                    verticesVirtuais[fim++] = i;
            }
    }
    if (counterTopo < this->v)
        for (int i = 0; i < this->v; i++)
            topo[i] = -1;
    return topo;
}

void GrafoMatriz::verNumeracaoTopologica(){
    if (!isGrafoTopologico()){
        cout << "Nao e um grafo topologico" << endl;
        return;
    }
    int* visita = getNumeracaoTopologica();
    cout << "Ordem dos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << i << " ";
    cout << endl;
    cout << "Numeracao topologica -> ";
    for (int i = 0; i < this->v; i++)
        cout << visita[i] << " ";
    cout << endl;
}

bool GrafoMatriz::isGrafoTopologico(){
    return getNumeracaoTopologica()[0] != -1;
}

int* GrafoMatriz::menorCaminhoSemPeso(int o){
    int *distancia = new int[this->v];
    for (int i = 0; i < this->v; i++)
        distancia[i] = -1;
    queue<int> qv;
    qv.push(o);
    distancia[o] = 0;
    while(!qv.empty()){
        int atual = qv.front();
        qv.pop();
        for (int i = 0; i < this->v; i++)
            if (distancia[i] == -1 && this->mv[atual][i] != 0){
                qv.push(i);
                distancia[i] = distancia[atual] + 1;
            }
    }
    return distancia;
}

void GrafoMatriz::verMenorCaminhoSemPeso(int o){
    int* visita = menorCaminhoSemPeso(o);
    cout << "Ordem dos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << i << " ";
    cout << endl;
    cout << "Distancia da origem " << o << " aos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << visita[i] << " ";
    cout << endl;
}

tuple<int, int*> GrafoMatriz::getComponentesConexas(){
    int cc = 0;
    int* visitadoCC = new int[this->v];
    for (int i = 0; i < this->v; i++)
        visitadoCC[i] = -1;
    for (int i = 0; i < this->v; i++)
        if (visitadoCC[i] == -1){
            getComponentesConexasR(i, visitadoCC, cc++);
        }
    tuple<int, int*> retorno (cc, visitadoCC);
    return retorno;
}

void GrafoMatriz::getComponentesConexasR(int w, int visitado[], int cc){
    visitado[w] = cc;
    for (int i = 0; i < this->v; i++)
        if (visitado[i] == -1 && this->mv[w][i] != 0)
            getComponentesConexasR(i, visitado, cc);
}

void GrafoMatriz::verComponentesConexas(){
    int* visita = get<1>(getComponentesConexas());
    cout << "Ordem dos vertices -> ";
    for (int i = 0; i < this->v; i++)
        cout << i << " ";
    cout << endl;
    cout << "Subgrafos -> ";
    for (int i = 0; i < this->v; i++)
        cout << visita[i] << " ";
    cout << endl;
}