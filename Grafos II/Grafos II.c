/*

	Universidade de Brasilia
	Departamento de Ciencia da Computacao
	Teoria e Aplicacao de Grafos

	Aluno: Lucas Araujo Pena - 13/0056162

	Trabalho 2 

	Sistema Operacional: Windows 7
	Compilador: C++ do Visual Studio 2019
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura auxiliar para a criacao das listas de adjacencia
typedef struct Vetor {
	char codigo[10];
	char nome[50];
	int peso;
} tVetorDisciplinas;

// Estrutura do elemento da lista de adjacencia
struct elementoListAdj{
	int id;
	char codigo[10];
	char nome[50];
	struct elementoListAdj* proximo;
};

//Estrutura da lista de adjacencia
struct listaAdj {
	int id;
	int grauSaida;
	int grauEntrada;
	char codigo[10];
	char nome[50];
	int peso;
	struct elementoListAdj* inicio;
};

//Estrutura do grafo
struct Grafo {
	int numeroVertices;
	int numeroArestas;
	int grau;
	struct listaAdj* vetorAdj;
};

//Estrutura de um elemento de uma lista
typedef struct elemento {
	int id;
	struct elemento* proximo;
} t_elemento;

//Estrutura de uma lista
typedef struct lista {
	t_elemento* inicio;
	t_elemento* fim;
} t_lista;

//Declaracao das funcoes

//Cria uma lista vazia
t_lista* criarLista();

//Verifica se uma lista esta vazia atraves da chcagem do seu inicio
int estaVazia(t_lista* lista);

//Libera o espaco de memoria ocupado por uma lista
void liberaLista(t_lista* lista);

//Insere um elemento no inicio da lista
int insereInicio(int valor, t_lista* lista);

//Insere um elemento no fim da lista
int insereFim(int valor, t_lista* lista);

//Remove um elemento do incio da lista e retorna seu valor
int removeInicio(t_lista* lista);

//Remove um elemento do fim da lista e retorna seu valor
int removeFim(t_lista* lista);

//Funcao que le o arquivo de dados e determina o numero de vertices do grafo
void numeroVertices(FILE* arquivo, int* pNumeroVertices);

//Carrega dados iniciais em um vetor para auxiliar na contucao do grafo
void carregarDados(FILE* arquivo, int pNumeroVertices, tVetorDisciplinas* disciplinas);

//Funcao que cria um novo grafo
struct Grafo* criarGrafo(int pNumeroVertices);

//Funcao para carregar as informacoes do grafo criado
void inicializarGrafo(struct Grafo* Grafo, tVetorDisciplinas *disciplinas);

//Funcao que cria novo no na lista de adjacencias
struct elementoListAdj* adicionarElemento(int id, char* nome, char* codigo);

//Funcao para inserir arestas
void inserirAresta(struct Grafo* Grafo, int origem, int id, char* nome, char* codigo);

//Funcao que retorna o id de um vertice correspondende ao codigo
int retornaId(char* codigo, tVetorDisciplinas* disciplinas, int pNumeroVertices);

//Funcao que percorre o arquivo txt e adiciona os elementos
void preencherGrafo(FILE* arquivo, tVetorDisciplinas * disciplinas, struct Grafo* Grafo);

//Funcao para imprimir a lista de adjacencia e seus elementos
void imprimirGrafo(struct Grafo* Grafo);

//Analisa o grafo e adiciona os vertices com grau de entrada a lista
void preencherListaZero(struct Grafo* Grafo, t_lista* listaZero, int* vetorGrau);

//Acha uma ordenacao topologica no grafo
void ordenacaoTopologica(struct Grafo* Grafo, t_lista* listaZero, t_lista* listaTopologica, int* vetorGrau);

//Funcao para identificar o caminho critico do DAG
void caminhoCritico(struct Grafo* Grafo, t_lista* listaTopologica, int* vetorDistancia);


//Funcao principal do programa
int main() {
	int vNumeroVertices = 0, i;
	FILE* arquivo;
	t_lista* listaZero = criarLista();
	t_lista* listaTopologica = criarLista();

	printf("Teoria e Aplicacao de Grafos - Projeto 2 \n\n");

	numeroVertices(arquivo, &vNumeroVertices);
	
	tVetorDisciplinas disciplinas[vNumeroVertices];
	int vetorGrau[vNumeroVertices];
	int vetorDistancia[vNumeroVertices];
	struct Grafo* Grafo = criarGrafo(vNumeroVertices);

	carregarDados(arquivo, vNumeroVertices, disciplinas);
	inicializarGrafo(Grafo, disciplinas);
	preencherGrafo(arquivo, disciplinas, Grafo);
	imprimirGrafo(Grafo);
	preencherListaZero(Grafo, listaZero, vetorGrau);
	ordenacaoTopologica(Grafo, listaZero, listaTopologica, vetorGrau);
	caminhoCritico(Grafo, listaTopologica, vetorDistancia);

	return(0);
}

t_lista* criarLista() {
	t_lista* lista = (t_lista*)malloc(sizeof(t_lista));

	if (lista != NULL)	{
		lista->inicio = NULL;
		lista->fim = NULL;
	}

	return(lista);
}

int estaVazia(t_lista* lista) {
	if (lista->inicio == NULL)	{
		return 1;
	}

	return 0;
}

void liberaLista(t_lista* lista) {
	if (lista->inicio != NULL) {
		t_elemento* removido;
		
		while (lista -> inicio != NULL ) {
			removido = lista->inicio;
			lista->inicio = lista->inicio->proximo;
			free(removido);
		}

		free(lista);
	}

	else {
		free(lista);
	}
}

int insereInicio(int valor, t_lista* lista) {
	t_elemento* novoPrimeiro = (t_elemento*)malloc(sizeof(t_elemento));

	novoPrimeiro->id = valor;
	novoPrimeiro->proximo - lista->inicio;
	lista->inicio = novoPrimeiro;

	if (lista->fim == NULL) {
		lista->fim = novoPrimeiro;
	}

	return 0;
}

int insereFim(int valor, t_lista* lista) {
	t_elemento* novoUltimo = (t_elemento*)malloc(sizeof(t_elemento));

	novoUltimo->id = valor;
	novoUltimo->proximo = NULL;

	if (lista->fim == NULL)	{
		lista->inicio = novoUltimo;
		lista->fim = novoUltimo;
	}

	t_elemento* tmp = lista->fim;

	tmp->proximo = novoUltimo;
	lista->fim = novoUltimo;

	return 0;
}

int removeInicio(t_lista* lista) {
	if (estaVazia(lista)) {
		return -1;
	}

	int tmp = lista->inicio->id;
	t_elemento* removido = lista->inicio;
	lista->inicio = lista->inicio->proximo;
	free(removido);

	if (lista->inicio == NULL) {
		lista->fim = NULL;
	}

	return tmp;
}

int removeFim(t_lista* lista) {
	t_elemento* removido;
	t_elemento* novoUltimo;

	if (estaVazia(lista)) {
		return -1;
	}

	removido = lista->inicio;

	while (removido->proximo != NULL) {
		novoUltimo = removido->proximo;
		removido = novoUltimo->proximo;
	}

	int tmp = removido->id;
	novoUltimo->proximo = NULL;
	lista->fim = novoUltimo;

	if (lista->inicio == NULL) {
		lista->fim = NULL;
	}

	return tmp;
}

void numeroVertices(FILE* arquivo, int* pNumeroVertices) {
	int count = 1; //Para compensar o EoF
	char caracter;

	arquivo = fopen("Dados.txt", "r");

	if (!arquivo) {
		printf("\nErro ao ler o arquivo.\n\n");
		return;
	}

	while (!feof(arquivo)) {
		caracter = fgetc(arquivo);

		if (caracter == '\n') {
			count++;
		}
	}

	fclose(arquivo);
	*pNumeroVertices = count;
}

void carregaDados(FILE* arquivo, int pNumeroVertices, tVetorDisciplinas* disciplinas) {
	int i, j, k;
	char string[300], aux[300], caractere;

	arquivo = fopen("Dados.txt", "r");

	if (!arquivo) {
		printf("\nErro ao ler o arquivo.\n\n");
		return;
	}

	for (i = 0; i < pNumeroVertices; i++) {
		fgets(string, 300, arquivo);

		j = 0;
		k = 0;
		aux[0] = "\0";

		//Este while varre a string até encontrar o caractere '|', que separa os dados
		while (string[j] != 0x7c){
			caractere = string[j];

			if (isdigit(caractere)) {
				aux[k] = caractere;
				aux[k + 1] = "\0";
				k++;
			}

			j++;
		}

		aux[k] = "\0";

		//Primeiro campo. Codigo da disciplina
		strcpy(disciplinas[i].codigo, aux);

		aux[0] = "\0";
		k = 0;
		j += 2;

		while (string[j] != 0x7c) {
			caractere = string[j];

			if (caractere != 0x7c) {
				aux[k] = caractere;
				aux[k + 1] = "\0";
				k++;
			}

			j++;
		}

		//Segundo campo. Nome da Disciplina
		strcpy(disciplinas[i].nome, aux);

		aux[0] = "\0";
		k = 0;
		j++;

		while (string[j] != 0x7c) {
			caractere = string[j];

			if (caractere != 0x20) {
				aux[k] = caractere;
				aux[k + 1] = "\0";
				k++;
			}

			j++;
		}

		aux[k] = "\0";

		//Terceiro campo. Peso da disciplina
		disciplinas[i].peso = atoi(aux);
	}

	fclose(arquivo);
}

struct Grafo* criarGrafo(int pNumeroVertices){
	int i;

	struct Grafo* Grafo = (struct Grafo*)malloc(sizeof(struct Grafo));
	Grafo->numeroArestas = 0;
	Grafo->numeroVertices = pNumeroVertices;
	Grafo->grau = 0;
	
	//Criacao do vetor de listas de adjacencias
	Grafo->vetorAdj = (struct listaAdj*)malloc(pNumeroVertices* sizeof(struct listaAdj));

	for (i = 0; i < pNumeroVertices; ++i){
		Grafo->vetorAdj[i].inicio = NULL;
		Grafo->vetorAdj[i].grauEntrada = 0;
		Grafo->vetorAdj[i].grauSaida = 0;
	}

	return Grafo;
};

void inicializarGrafo(struct Grafo* Grafo, tVetorDisciplinas* disciplinas) {
	int i;

	for (i = 0; i < Grafo->numeroVertices; i++) {
		Grafo->vetorAdj[i].id = i;
		strcpy(Grafo->vetorAdj[i].codigo, disciplinas[i].codigo);
		strcpy(Grafo->vetorAdj[i].nome, disciplinas[i].nome);
		Grafo->vetorAdj[i].peso = disciplinas[i].peso;
	}
}

struct elementoListAdj* adicionarElemento(int id, char* nome, char* codigo) {
	struct elementoListAdj* novoElemento = (struct elementoListAdj*)malloc(sizeof(struct  elementoListAdj));
	novoElemento->id = id;
	strcpy(novoElemento->nome, nome);
	strcpy(novoElemento->codigo, codigo);
	novoElemento->proximo = NULL;

	return novoElemento;
}

void inserirAresta(struct Grafo* Grafo, int origem, int id, char* nome, char* codigo) {
	struct elementoListAdj* novoElemento = adicionarElemento(id, nome, codigo);

	//Insere o vertice identificado pelo id no inicio da lista
	novoElemento->proximo = Grafo->vetorAdj[origem].inicio;
	Grafo->vetorAdj[origem].inicio = novoElemento;
	Grafo->vetorAdj[origem].grauSaida++;
	Grafo->vetorAdj[id].grauEntrada++;

	if (Grafo->grau < Grafo->vetorAdj[origem].grauSaida + Grafo->vetorAdj[origem].grauEntrada) {
		Grafo->grau = Grafo->vetorAdj[origem].grauSaida + Grafo->vetorAdj[origem].grauEntrada;
	}

	Grafo->numeroArestas++;
}

int retornaId(char* codigo, tVetorDisciplinas* disciplinas, int pNumeroVertices) {
	int i;

	for (i = 0; i < pNumeroVertices; i++) {
		if (strcmp(disciplinas[i].codigo, codigo) == 0)	{
			return i;
		}
	}

	return -1;
}

void preencherGrafo(FILE* arquivo, tVetorDisciplinas* disciplinas, struct Grafo* Grafo) {
	int i, j, k, idAux;
	char string[300], aux[300], caractere;

	arquivo = fopen("Dados.txt", "r");

	if (!arquivo) {
		printf("\nErro ao abrir o arquivo.\n\n");
		return;
	}

	for (i = 0; i < Grafo->numeroVertices; i++) {
		fgets(string, 300, arquivo);

		j = 0;
		k = 0;
		aux[0] = "\0";

		//Laco para chegar ate o campo de pre requisitos
		while (k != 3) {
			if (string[j] == 0x7c) {
				k++;
			}
			j++;
		}

		k = 0;
		j++;

		//Identifica e adiciona as arestas baseado nos pre requisitos
		while (string[j] != "\n") {
			caractere = string[j];

			if (caractere != 0x02) {
				aux[k] = caractere;
				aux[k + 1] = "\0";
				k++;
			}

			if (caractere == 0x20) {
				idAux = retornaId(aux, disciplinas, Grafo->numeroVertices);

				if (idAux != -1) {
					inserirAresta(Grafo, idAux, i, disciplinas[i].nome, disciplinas[i].codigo);
				}

				aux[0] = "\0";
				k = 0;
			}

			j++;
		}

		aux[0] = "\0";
		k = 0;
	}

	fclose(arquivo);
}

void imprimirGrafo(struct Grafo* Grafo) {
	int v, j, grau;
	struct elementListAdj* vertice = Grafo->vetorAdj[v].inicio;

	printf("Dados do grafo\nNumero de Vertices: %d\tNumero de Arestas: %d \n", Grafo->numeroVertices, Grafo->numeroArestas);
	printf("Grau do Grafo: %d \n\n", Grafo->grau);
	printf("Curso: 370 - Ciencia da Computacao\n\n");
	printf("(Os elementos da lista de Adjacencia sao as disciplinas das quais o vertice eh pre-requisito)");
	printf("\nLista de Adjacencia: \n");

	grau = Grafo->grau;

	for (v = 0; v < Grafo->numeroVertices; ++v)	{
		for (j = 0; j < Grafo->numeroVertices; j++) {
			vertice = Grafo->vetorAdj[j].inicio;

			if (Grafo->vetorAdj[j].grauSaida == grau) {
				printf("\n%s - %s (ID: %d | Grau entrada: %d | Grau saida: %d):\n", Grafo->vetorAdj[j].codigo, Grafo->vetorAdj[j].nome, j, Grafo->vetorAdj[j].grauEntrada, Grafo->vetorAdj[j].grauSaida);

				if (Grafo->vetorAdj[j].grauSaida == 0) {
					printf("Disciplina nao e pre-requisito de nenhuma outra.");
				}

				else {
					printf("| ");
				}

				while (vertice) {
					printf("%s | ", vertice->codigo);
					vertice = vertice->proximo;
				}

				printf("\n");
			}
		}

		grau--;
	}
}

void preencherListaZero(struct Grafo* Grafo, t_lista* listaZero, int* vetorGrau) {
	int i;

	for (i = 0; i < Grafo->numeroVertices; i++) {
		vetorGrau[i] = Grafo->vetorAdj[i].grauEntrada;

		if (Grafo->vetorAdj[i].grauEntrada == 0) {
			insereFim(Grafo->vetorAdj[i].id, listaZero);
		}
	}
}

//Imprime a lista contendo a ordenacao topologica do grafo
void imprimirListaTopologica(struct Grafo* Grafo, t_lista* listaTopologica){
	t_elemento* tmp = listaTopologica->inicio;
	int i;

	printf("\n\nOrdenacao topologica do grafo com a seguinte ordem: \n\n");
	printf("%s - %s\n", Grafo->vetorAdj[tmp->id].codigo, Grafo->vetorAdj[tmp->id].nome);

	for (i = 0; i < (Grafo->numeroVertices - 1); i++) {
		tmp = tmp->proximo;
		printf("%s - %s\n", Grafo->vetorAdj[tmp->id].codigo, Grafo->vetorAdj[tmp->id].nome);
	}
}

void ordenacaoTopologica(struct Grafo* Grafo, t_lista* listaZero, t_lista* listaTopologica, int* vetorGrau) {
	int id, count = 0;
	struct elementoListAdj* tmp;

	while (listaZero->inicio != NULL) {
		id = removeInicio(listaZero);
		count++;
		insereFim(id, listaTopologica);

		if (Grafo->vetorAdj[id].grauSaida != 0)	{
			tmp = Grafo->vetorAdj[id].inicio;

			while (tmp) {
				if (vetorGrau[tmp->id] != 0) {
					vetorGrau[tmp->id]--;
				}

				if (vetorGrau[tmp->id] == 0) {
					insereFim(tmp->id, listaZero);
				}

				tmp = tmp->proximo;
			}
		}
	}

	imprimirListaTopologica(Grafo, listaTopologica);
}

//Funcao para definir os vertices do caminho critico
int criarCaminho(struct Grafo* Grafo, int origem, int destino, int* caminho, int sequencia) {
	int maxPeso = -1;
	int maxPesoRetorno;
	struct elementoListAdj* tmp;

	if (origem == destino) {
		caminho[destino] = sequencia;
		return 7;
	}

	caminho[origem] = 1;

	for (tmp = Grafo->vetorAdj[origem].inicio; tmp != NULL; tmp = tmp->proximo)	{
		if (caminho[tmp->id == 0]) {
			maxPesoRetorno = criarCaminho(Grafo, tmp->id, destino, caminho, sequencia + 1);

			if (maxPesoRetorno != -1 && maxPeso < Grafo->vetorAdj[tmp->id].peso + maxPesoRetorno) {
				maxPeso = Grafo->vetorAdj[tmp->id].peso + maxPesoRetorno;
				caminho[tmp->id] = sequencia + 1;
			}
		}
	}

	if (caminho[origem] == 1) caminho[origem] = 0;

	return maxPeso;
}

void caminhoCritico(struct Grafo* Grafo, t_lista* listaTopologica, int* vetorDistancia) {

	int i, id, j, verticeInicio = 0, verticeFim = 0, aux = 0;
	struct listaAdj* vertice;
	struct elementoListAdj* tmp;
	t_elemento* elem;
	int caminho[Grafo->numeroVertices], controle = 0;

	for (j = 0; j < Grafo->numeroVertices; j++) {
		id = j;

		//Inicializacao do vetor de distancias
		for (i = 0; i < Grafo->numeroVertices; i++)	{
			vetorDistancia[i] = -1;
		}

		elem = listaTopologica->inicio;
		vetorDistancia[id] = 0;

		for (i = 0; i < Grafo->numeroVertices; i++) {
			if (vetorDistancia[elem->id] != -1)	{
				for (tmp = Grafo->vetorAdj[elem->id].inicio; tmp != NULL; tmp = tmp->proximo) {
					if (vetorDistancia[elem->id] + Grafo->vetorAdj[elem->id].peso > vetorDistancia[tmp->id]) {
						if (vetorDistancia[tmp->id] != -1) {
							vetorDistancia[tmp->id] = 0 + Grafo->vetorAdj[elem->id].peso;
						}

						else {
							vetorDistancia[tmp->id] = vetorDistancia[elem->id];
						}
					}
				}
			}

			elem = elem->proximo;
		}

		for (i = 0; i < Grafo->numeroVertices; i++) {
			if (aux < vetorDistancia[i]) {
				verticeInicio = id;
				verticeFim = i;
				aux = vetorDistancia[i];
			}
		}
	}

	//Inicializacao do Vetor Caminho
	for (i = 0; i < Grafo->numeroVertices; i++){
		caminho[i] = 0;
	}

	criarCaminho(Grafo, verticeInicio, verticeFim, caminho, 1);
	aux = 1;

	printf("\n\nCaminho critico identificado seguindo a seguinte ordem: \n\n");

	for (i = 0; i < Grafo->numeroVertices; i++) {
		for (j = 0; j < Grafo->numeroVertices; j++)	{
			if (caminho[j] == aux) {
				printf("| %s - %s | ", Grafo->vetorAdj[j].codigo, Grafo->vetorAdj[j].nome);

				if (j != verticeFim) {
					printf("-> \n");
				}
			}
		}

		aux++;
	}

	printf("\n\n");
}
