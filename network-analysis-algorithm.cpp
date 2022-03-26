#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <graphics.h>
#include <stdbool.h>

struct Grafo
{
    int numeroVertices;
    int grauMaximo;
    bool ponderado;
    int **arestas;
    float **pesos;
    int *grau;
};

typedef struct vertex
{
    int label;
    int grau;
    float closeness;
} Vertice;

struct Grafo *CriarGrafo(int numeroVertices, int grauMaximo, bool ponderado)
{
    struct Grafo *grafo = (struct Grafo *) malloc(sizeof(struct Grafo));
    if(grafo != NULL)
    {
        grafo->numeroVertices = numeroVertices;
        grafo->grauMaximo = grauMaximo;
        grafo->ponderado = (ponderado != false)?true:false;
        grafo->arestas = (int **) malloc(numeroVertices * sizeof(int *));
        grafo->grau = (int *) calloc(numeroVertices, sizeof(int));

        int i;
        for(i = 0; i < numeroVertices; i++)
            grafo->arestas[i] = (int *) malloc(grauMaximo * sizeof(int));

        if(grafo->ponderado)
        {
            grafo->pesos = (float **) malloc(numeroVertices * sizeof(float *));
            for(i = 0; i < numeroVertices; i++)
                grafo->pesos[i] = (float *) malloc(grauMaximo * sizeof(float));
        }
    }
    return grafo;
}

void InserirAresta(struct Grafo *grafo, int verticeOrigem, int verticeDestino, bool digrafo, float peso)
{
    if(grafo != NULL)
    {
        if(verticeOrigem < 0 || verticeOrigem >= grafo->numeroVertices) return;
        else if(verticeDestino < 0 || verticeDestino >= grafo->numeroVertices) return;

        grafo->arestas[verticeOrigem][grafo->grau[verticeOrigem]] = verticeDestino;

        if(grafo->ponderado)
            grafo->pesos[verticeOrigem][grafo->grau[verticeOrigem]] = peso;
        grafo->grau[verticeOrigem]++;

        if(digrafo == 0)
            InserirAresta(grafo, verticeDestino, verticeOrigem, true, peso);
    }
}

bool BuscarElemento(struct Grafo *grafo, int verticeInicial, int *verticesVisitados, int elemento)
{
    int i, vertice, contador = 1, *filaOrdemVertices, verticeInicioFila = 0, verticeFinalFila = 1;
    Vertice *vetorVertices;

    filaOrdemVertices = (int *) malloc(grafo->numeroVertices * sizeof(int));
    vetorVertices = (Vertice *) malloc(grafo->numeroVertices * sizeof(struct vertex));

    for(i = 0; i < grafo->numeroVertices; i++)
        verticesVisitados[i] = 0;

    filaOrdemVertices[verticeFinalFila] = verticeInicial;
    verticesVisitados[verticeInicial] = 1;

    while(verticeInicioFila != verticeFinalFila)
    {
        verticeInicioFila = (verticeInicioFila + 1) % grafo->numeroVertices;
        vertice = filaOrdemVertices[verticeInicioFila];
        contador++;
        if(elemento == vertice)
        {
            free(filaOrdemVertices);
            free(vetorVertices);
            return true;
        }
        for(i = 0; i < grafo->grau[vertice]; i++)
        {
            if(!(verticesVisitados[grafo->arestas[vertice][i]]))
            {
                verticeFinalFila = (verticeFinalFila + 1) % grafo->numeroVertices;
                filaOrdemVertices[verticeFinalFila] = grafo->arestas[vertice][i];
                verticesVisitados[grafo->arestas[vertice][i]] = contador;
            }
        }
    }

    free(filaOrdemVertices);
    free(vetorVertices);
    return false;
}

void OrdenarGrau(struct Grafo *grafo, int verticeInicial, int *verticesVisitados)
{
    int i, j, vertice, contador = 1, *filaOrdemVertices, verticeInicioFila = 0, verticeFinalFila = 1;
    Vertice *vetorVertices, aux;

    filaOrdemVertices = (int *) malloc(grafo->numeroVertices * sizeof(int));
    vetorVertices = (Vertice *) malloc(grafo->numeroVertices * sizeof(struct vertex));

    for(i = 0; i < grafo->numeroVertices; i++)
        verticesVisitados[i] = 0;

    filaOrdemVertices[verticeFinalFila] = verticeInicial;
    verticesVisitados[verticeInicial] = 1;

    while(verticeInicioFila != verticeFinalFila)
    {
        verticeInicioFila = (verticeInicioFila + 1) % grafo->numeroVertices;
        vertice = filaOrdemVertices[verticeInicioFila];
        vetorVertices[vertice].label = vertice;
        vetorVertices[vertice].grau = grafo->grau[vertice];
        contador++;
        for(i = 0; i < grafo->grau[vertice]; i++)
        {
            if(!(verticesVisitados[grafo->arestas[vertice][i]]))
            {
                verticeFinalFila = (verticeFinalFila + 1) % grafo->numeroVertices;
                filaOrdemVertices[verticeFinalFila] = grafo->arestas[vertice][i];
                verticesVisitados[grafo->arestas[vertice][i]] = contador;
            }
        }
    }

    for(i = 0; i < (grafo->numeroVertices - 1); i++)
            for(j = 0; j < (grafo->numeroVertices - 1 - i); j++)
                if(vetorVertices[j].grau < vetorVertices[j + 1].grau)
                {
                    aux = vetorVertices[j];
                    vetorVertices[j] = vetorVertices[j + 1];
                    vetorVertices[j + 1] = aux;
                }

    printf("[Vertice, Grau]\n");
    for(i = 0; i < grafo->numeroVertices; i++)
        printf("[%d, %d]\n", vetorVertices[i].label, vetorVertices[i].grau);
    printf("\n");

    free(filaOrdemVertices);
    free(vetorVertices);
}

int MenorDistancia(int *distancia, int *verticesVisitados, int numeroVertices)
{
    int i, menorDistancia = -1, primeiro = 1;
    for(i = 0; i < numeroVertices; i++)
    {
        if(distancia[i] >= 0 && verticesVisitados[i] == 0)
        {
            if(primeiro)
            {
                menorDistancia = i;
                primeiro = 0;
            }
            else if(distancia[menorDistancia] > distancia[i])
                menorDistancia = i;
        }
    }
    return menorDistancia;
}

void Dijkstra(struct Grafo *grafo, int verticeInicial, int *antecessor, int *distancia, int *vetorDistancias)
{
    int i, j = 0, contador, indice, *verticesVisitados, vertice;
    contador = grafo->numeroVertices;
    verticesVisitados = (int *) malloc(grafo->numeroVertices * sizeof(int));
    for(i = 0; i < grafo->numeroVertices; i++)
    {
        antecessor[i] = -1;
        distancia[i] = -1;
        verticesVisitados[i] = 0;
    }
    for(i = 0; i < grafo->numeroVertices; i++)
        vetorDistancias[i] = 0;
    distancia[verticeInicial] = 0;
    while(contador > 0)
    {
        vertice = MenorDistancia(distancia, verticesVisitados, grafo->numeroVertices);
        vetorDistancias[j] = vertice;
        if(vertice == -1) break;

        verticesVisitados[vertice] = 1;
        contador--;
        for(i = 0; i < grafo->grau[vertice]; i++)
        {
            indice = grafo->arestas[vertice][i];
            if(distancia[indice] < 0)
            {
               distancia[indice] = distancia[vertice] + 1;
               antecessor[indice] = vertice;
            }
            else if(distancia[indice] > distancia[vertice] + 1)
            {
                    distancia[indice] = distancia[vertice] + 1;
                    antecessor[indice] = vertice;
            }
        }
        j++;
    }
    free(verticesVisitados);
}

void Closeness(struct Grafo *grafo, int verticeInicial, int *verticesVisitados)
{
    int i, soma, *distancia, *vetorDistancias, j, vertice, contador = 1, *filaOrdemVertices, verticeInicioFila = 0, verticeFinalFila = 1, *antecessor;
    double *closeness;
    Vertice *vetorVertices, aux;

    filaOrdemVertices = (int *) malloc(grafo->numeroVertices * sizeof(int));
    antecessor = (int *) malloc(grafo->numeroVertices * sizeof(int));
    distancia = (int *) malloc(grafo->numeroVertices * sizeof(int));
    vetorDistancias = (int *) malloc(grafo->numeroVertices * sizeof(int));
    closeness = (double *) malloc(grafo->numeroVertices * sizeof(double));
    vetorVertices = (Vertice *) malloc(grafo->numeroVertices * sizeof(struct vertex));

    for(i = 0; i < grafo->numeroVertices; i++)
        verticesVisitados[i] = 0;

    filaOrdemVertices[verticeFinalFila] = verticeInicial;
    verticesVisitados[verticeInicial] = 1;

    while(verticeInicioFila != verticeFinalFila)
    {
        soma = 0;
        verticeInicioFila = (verticeInicioFila + 1) % grafo->numeroVertices;
        vertice = filaOrdemVertices[verticeInicioFila];
        Dijkstra(grafo, vertice, antecessor, distancia, vetorDistancias);
        for(i = 0; i < grafo->numeroVertices; i++)
            soma += vetorDistancias[i];
        vetorVertices[vertice].label = vertice;
        vetorVertices[vertice].closeness = soma;
        contador++;
        for(i = 0; i < grafo->grau[vertice]; i++)
        {
            if(!(verticesVisitados[grafo->arestas[vertice][i]]))
            {
                verticeFinalFila = (verticeFinalFila + 1) % grafo->numeroVertices;
                filaOrdemVertices[verticeFinalFila] = grafo->arestas[vertice][i];
                verticesVisitados[grafo->arestas[vertice][i]] = contador;
            }
        }
    }

    for(i = 0; i < (grafo->numeroVertices - 1); i++)
        for(j = 0; j < (grafo->numeroVertices - 1 - i); j++)
            if(vetorVertices[j].closeness < vetorVertices[j + 1].closeness)
            {
                aux = vetorVertices[j];
                vetorVertices[j] = vetorVertices[j + 1];
                vetorVertices[j + 1] = aux;
            }

    printf("[Vertice, Closeness]\n");
    for(i = 0; i < grafo->numeroVertices; i++)
        printf("[%d, %d]\n", vetorVertices[i].label, vetorVertices[i].closeness);
    printf("\n");

    free(filaOrdemVertices);
    free(vetorVertices);
    free(antecessor);
    free(distancia);
    free(closeness);
    free(vetorDistancias);
}

void LiberarGrafo(struct Grafo *grafo)
{
    if(grafo != NULL)
    {
        int i;
        for(i = 0; i < grafo->numeroVertices; i++)
            free(grafo->arestas[i]);
        free(grafo->arestas);

        if(grafo->ponderado)
        {
            for(i = 0; i < grafo->numeroVertices; i++)
                free(grafo->pesos[i]);
            free(grafo->pesos);
        }
        free(grafo->grau);
        free(grafo);
    }
}

int main()
{
    FILE *arquivoBase = fopen("guilherme_nicolasi_base.txt", "r");
    if(!arquivoBase)
    {
        perror("");
        exit(0);
    }
    else
    {
        int linha = 0, coluna, verticeOrigem, verticeDestino, verticesVisitados[4038], opcao, elemento;
        char buffer[1024], aux[2], *delimitador;
        struct Grafo *grafo = CriarGrafo(4038, 4038, 0);
        rewind(arquivoBase);
        while(fgets(buffer, sizeof(buffer), arquivoBase))
        {
            coluna = 0;
            linha++;
            delimitador = strtok(buffer, " ");
            while(delimitador)
            {
                if(coluna == 0)
                {
                    strcpy(aux, delimitador);
                    verticeOrigem = atoi(aux);
                }
                else if(coluna == 1)
                {
                    strcpy(aux, delimitador);
                    verticeDestino = atoi(aux);
                    InserirAresta(grafo, verticeOrigem, verticeDestino, 0, 0);
                }
                delimitador = strtok(NULL, " ");
                coluna++;
            }
        }
        fclose(arquivoBase);
        do
        {
            printf("1. Imprimir o plot do grafo\n2. Imprimir lista ordenada por grau\n3. Imprimir lista ordenada por closeness\n4. Pesquisar elemento do grafo\n5. Sair\n\n");
            scanf("%d", &opcao);
            printf("\n");
            switch(opcao)
            {
                case 1:
                    initwindow(500, 500, "", 700, 200);

                    setbkcolor(WHITE);
                    cleardevice();

                    readimagefile("C:/Users/USER/Documents/guilherme_nicolasi_trabalho3/grafo_completo.jpg", 0, 0, 500, 500);
                    delay(2000);
                    closegraph();
                break;
                case 2:
                    OrdenarGrau(grafo, 0, verticesVisitados);
                break;
                case 3:
                    Closeness(grafo, 0, verticesVisitados);
                break;
                case 4:
                    printf("Digite o indice do elemento: ");
                    scanf("%d", &elemento);
                    if(BuscarElemento(grafo, 0, verticesVisitados, elemento))
                        printf("\nElemento encontrado.\n\n");
                    else
                        printf("\nElemento nao encontrado.\n\n");
                break;
            }
        } while(opcao != 5);
        LiberarGrafo(grafo);
    }
    return 0;
}
