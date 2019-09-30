#include <iostream>
#include <algorithm>
#include <list>
#include <queue>
#include <tuple>


using namespace std;

class GrafoLista{

    int v;
    list<int> *lv;
    int dfsCounterPreOrdem;
    int dfsCounterPosOrdem;
    int bfsCounter;

    public:
        //Funções básicas
        GrafoLista(int);
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

GrafoLista::GrafoLista(int v){
    this->v = v;
    this->lv = new list<int>[v];
}

void GrafoLista::adicionarAresta(int o, int d){
    if (find(this->lv[o].begin(), this->lv[o].end(), d) != this->lv[o].end())
        return;
    this->lv[o].push_back(d);
}

void GrafoLista::verGrafo(){
    for (int i = 0; i < this->v; i++){
        cout << i << ": ";
        list<int>::iterator j;
        for (j = this->lv[i].begin(); j != this->lv[i].end(); j++)
            cout << *j << " ";
        cout << endl;
    }
}

int* GrafoLista::dfs(bool returnPreOrdem = true){
    this->dfsCounterPreOrdem = 0;
    this->dfsCounterPosOrdem = 0;
    int* preOrdem = new int[this->v];
    int* posOrdem = new int[this->v];
    for (int i = 0; i < this->v; i++){
        preOrdem[i] = -1;
        posOrdem[i] = -1;
    }

    for (int i = 0; i < this->v; i++)
        if (preOrdem[i] == -1)
            dfsR(i, preOrdem, posOrdem);

    return (returnPreOrdem) ? preOrdem : posOrdem;
}

void GrafoLista::dfsR(int w, int preOrdem[], int posOrdem[]){
    preOrdem[w] = this->dfsCounterPreOrdem++;
    list<int>::iterator i;
    for (i = this->lv[w].begin(); i != this->lv[w].end(); i++){
        if (preOrdem[*i] == -1)
            dfsR(*i, preOrdem, posOrdem);
    }
    posOrdem[w] = this->dfsCounterPosOrdem++;
}

void GrafoLista::verDfs(bool returnPreOrdem = true){
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

int* GrafoLista::bfs(int w){
    this->bfsCounter = 0;
    int* visitado = new int[this->v];
    for (int i = 0; i < this->v; i++)
        visitado[i] = -1;
    queue<int> qv;
    qv.push(w);
    visitado[w] = this->bfsCounter++;
    list<int>::iterator i;
    while (!qv.empty()){
        int atual = qv.front();
        qv.pop();
        for (i = this->lv[atual].begin(); i != this->lv[atual].end(); i++){
            if (visitado[*i] == -1){
                visitado[*i] = this->bfsCounter++;
                qv.push(*i);
            }
        }
    }
    return visitado;
}

void GrafoLista::verBfs(int w){
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

bool GrafoLista::hasCicle(){
    int* posOrdem = dfs(false);
    list<int>::iterator li;
    for (int i = 0; i < this->v; i++)
        for (li = this->lv[i].begin(); li != this->lv[i].end(); li++)
            if (posOrdem[i] < posOrdem[*li])
                return true;
    return false;
}

int* GrafoLista::getNumeracaoTopologica(){
    //Return -1 no vetor de retorno se nao houver numeração topologica
    int counterTopo = 0;
    int* topo = new int[this->v];
    int* grauEntrada = new int[this->v];
    list<int>::iterator li;
    for (int i = 0; i < this->v; i++){
        topo[i] = -1;
        grauEntrada[i] = 0;
    }
    for (int i = 0; i < this->v; i++)
        for (li = this->lv[i].begin(); li != this->lv[i].end(); li++)
            grauEntrada[*li]++;
    
    int* verticesVirtuais = new int[this->v];
    int comeco = 0, fim = 0;
    for (int i = 0; i < this->v; i++)
        if (grauEntrada[i] == 0)
            verticesVirtuais[fim++] = i;
    while (comeco < fim){
        int atual = verticesVirtuais[comeco++];
        topo[atual] = counterTopo++;
        for (li = this->lv[atual].begin(); li != this->lv[atual].end(); li++){
            grauEntrada[*li]--;
            if (grauEntrada[*li] == 0)
                verticesVirtuais[fim++] = *li;
        }
    }
    if (counterTopo < this->v)
        for (int i = 0; i < this->v; i++)
            topo[i] = -1;
    return topo;
}

void GrafoLista::verNumeracaoTopologica(){
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

bool GrafoLista::isGrafoTopologico(){
    return getNumeracaoTopologica()[0] != -1;
}

int* GrafoLista::menorCaminhoSemPeso(int o){
    int *distancia = new int[this->v];
    for (int i = 0; i < this->v; i++)
        distancia[i] = -1;
    list<int>::iterator i;
    queue<int> qv;
    qv.push(o);
    distancia[o] = 0;
    while(!qv.empty()){
        int atual = qv.front();
        qv.pop();
        for (i = this->lv[atual].begin(); i != this->lv[atual].end(); i++)
            if (distancia[*i] == -1){
                qv.push(*i);
                distancia[*i] = distancia[atual] + 1;
            }
    }
    return distancia;
}

void GrafoLista::verMenorCaminhoSemPeso(int o){
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

tuple<int, int*> GrafoLista::getComponentesConexas(){
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

void GrafoLista::getComponentesConexasR(int w, int visitado[], int cc){
    visitado[w] = cc;
    list<int>::iterator li;
    for (li = this->lv[w].begin(); li != this->lv[w].end(); li++)
        if (visitado[*li] == -1)
            getComponentesConexasR(*li, visitado, cc);
}

void GrafoLista::verComponentesConexas(){
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