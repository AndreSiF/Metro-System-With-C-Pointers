#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct linhas{
	char cor[20];
	struct linhas *prox;
};

struct vizinho{
	struct estacoes *vizinho;
	struct vizinho *prox;
	int peso;
};

struct linhasEst{
	struct linhas *linha;
	struct linhasEst *prox;
};

struct estacoes{
	char nome[30];
	struct vizinho *vizinhos;
	struct linhasEst *linhas;
	struct estacoes *prox;
	int estado;
	int visit;
};

typedef struct estacoes Est;
typedef struct vizinho Viz;
typedef struct linhas Linha;
typedef struct linhasEst LinhaE;

void adicionarLinha(Est *estacao, Linha *linha){						//ADICIONA A LINHA A UMA ESTACAO
	LinhaE *novo = (LinhaE *) malloc (sizeof(LinhaE));
	novo->linha = linha;
	novo->prox = NULL;
	if(estacao->linhas != NULL){
		LinhaE *p;
		for(p = estacao->linhas; p->prox != NULL; p = p->prox);
		p->prox = novo;
	}
	else{
		estacao->linhas = novo;
	}
}

void addLinhaEst(Est *inicio, Linha *comeco, char estacao[30], char cor[20]){	//PROCURA A LINHA E A ESTACAO QUE FORAM ESCOLHIDAS PARA SEREM CONECTADAS
	if(strcmp(inicio->nome, estacao)){
		if(inicio->prox != NULL){
			addLinhaEst(inicio->prox, comeco, estacao, cor);
		}
		else{
			printf("Estacao nao encontrada.\n");
		}
	}
	else{
		Linha *p;
		for(p = comeco; strcmp(p->cor, cor) && p->prox != NULL; p = p->prox);
		if(!strcmp(p->cor, cor)){
			adicionarLinha(inicio, p);
		}
		else{
			printf("Linha nao encontrada.\n");
		}
	}
}

void inserirLinha(Linha *atual, Linha *novo){		//ADICIONA NOVA LINHA NA LISTA DE LINHAS
	if(atual->prox != NULL){
		inserirLinha(atual->prox, novo);
	}
	else{
		atual->prox = novo;
	}
}

Linha *inserirLinhas(Linha *comeco, char cor[20]){		//CRIA UMA NOVA LINHA
	Linha *novo = (Linha *) malloc (sizeof(Linha));
	strcpy(novo->cor, cor);
	novo->prox = NULL;
	if(comeco == NULL){
		return novo;
	}
	else{
		inserirLinha(comeco, novo);
	}
	return comeco;
}

Est *inserirEst(Est *inicio, char nome[30]){		//ADICIONA UMA NOVA ESTACAO NA LISTA DE ESTACOES
	Est *novo = (Est *) malloc (sizeof(Est));
	strcpy(novo->nome, nome);
	novo->estado = 1;
	novo->prox = NULL;
	novo->vizinhos = NULL;
	novo->linhas = NULL;
	if(inicio == NULL){
		return novo;
	}
	else{
		Est *p;
		for(p = inicio; p->prox != NULL; p = p->prox);
		p->prox = novo;
		return inicio;
	}
}

void inserirVizinho(Est *atual, Est *p, int peso){		//ADICIONA VIZINHO A UMA ESTACAO
	Viz *novo = (Viz *) malloc (sizeof(Viz));
	novo->vizinho = p;
	novo->prox = NULL;
	novo->peso = peso;
	if(atual->vizinhos != NULL){
		Viz *p1;
		for(p1 = atual->vizinhos; p1->prox != NULL; p1 = p1->prox);
		p1->prox = novo;
	}
	else{
		atual->vizinhos = novo;
	}
}

void inserirVizinhos(Est *inicio, Est *atual, char nome[30], char vizinho[30], int peso){		//PROCURA VIZINHO E ESTACAO ESCOLHIDOS NA ADICAO
	if(strcmp(atual->nome, nome)){
		if(atual->prox != NULL){
			inserirVizinhos(inicio, atual->prox, nome, vizinho, peso);
		}
		else{
			printf("Estacao nao encontrada.\n", nome, vizinho);
		}
	}
	else{
		Est *p;
		for(p = inicio; strcmp(p->nome, vizinho) && p->prox != NULL; p = p->prox);
		if(!strcmp(p->nome, vizinho)){
			inserirVizinho(atual, p, peso);
			inserirVizinho(p, atual, peso);
		}
		else{
			printf("Vizinho nao encontrado.\n");
		}
	}
}

void removerVizinho(Est *estacao, Viz *vizinhoDel){			//REMOVE O VIZINHO DA ESTACAO ESCOLHIDA
	Viz *p = estacao->vizinhos;
	if(p == NULL){
		return;
	}
	if(!strcmp(p->vizinho->nome, vizinhoDel->vizinho->nome)){
		estacao->vizinhos = estacao->vizinhos->prox;
		free(p);
	}
	else if(p->prox == NULL){
		free(vizinhoDel);
		estacao->vizinhos = NULL;
	}
	else{
		while(strcmp(p->prox->vizinho->nome, vizinhoDel->vizinho->nome)){
			p = p->prox;
		}
		p->prox = vizinhoDel->prox;
		free(vizinhoDel);
	}
}

void removerVizinhos(Est* estacao, char vizinho[30]){		//PROCURA VIZINHO ESCOLHIDA PARA REMOCAO
	Viz *p;
	for(p = estacao->vizinhos; strcmp(p->vizinho->nome, vizinho) && p->prox != NULL; p = p->prox);
	if(strcmp(p->vizinho->nome, vizinho)){
		printf("Vizinho nao encontrado.");
	}
	else{
		removerVizinho(estacao, p);
	}
}

void mostrarLinhas(LinhaE *linhas){			//MOSTRA TODAS AS LINHAS REGISTRADAS, INDEPENDENTEMENTE DE ESTAREM VAZIAS OU NAO
	if(linhas != NULL){
		printf("%s ", linhas->linha->cor);
		mostrarLinhas(linhas->prox);
	}
	else{
		return;
	}
}

void mostrarVizinhos(Viz *vizinho){			//MOSTRA TODOS OS VIZINHOS DE UMA ESTACAO
	if(vizinho != NULL){
		printf("%s | ", vizinho->vizinho->nome);
		mostrarVizinhos(vizinho->prox);
	}
	else{
		return;
	}
}

void mostrarNos(Est *inicio){			//MOSTRA TODOS OS NOS DA LISTA DE ESTACOES COM VIZINHOS OU NAO
	if(inicio != NULL){
		printf("\nNome %s - Linha(s): ", inicio->nome);
		mostrarLinhas(inicio->linhas);
		printf(" - Vizinhos: | ");
		mostrarVizinhos(inicio->vizinhos);
		mostrarNos(inicio->prox);
	}
	else{
		return;
	}
}

void zerarNos(Est *inicio){			//ZERA O ATRIBUTO QUE DEFINE QUE UMA ESTACAO FOI VISITADA EM TODAS AS ESTACOES (USADA PARA ROTAS PRINCIPALMENTE)
	if(inicio != NULL){
		zerarNos(inicio->prox);
		inicio->visit = 0;
	}
}

Est *iniciarEst(Est *inicio, Linha *comeco){		//ADICIONA O MAPA QUE FOI PEDIDO NAS INSTRUCOES DO TRABALHO
	// AZUL
	inicio = inserirEst(inicio, "Reboleira");
	inicio = inserirEst(inicio, "Colegio Militar / Luz");
	inicio = inserirEst(inicio, "Jardim Zoologico");
	inicio = inserirEst(inicio, "Praca de Espanha");
	inicio = inserirEst(inicio, "S.Sebastiao"); //VERMELHO
	inicio = inserirEst(inicio, "Marques de Pombal"); //VERDE
	inicio = inserirEst(inicio, "Restauradores");
	inicio = inserirEst(inicio, "Baixa-Chiado"); //VERDE
	inicio = inserirEst(inicio, "Terreiro do Paco");
	inicio = inserirEst(inicio, "Santa Apolonia");
	// VERDE
	inicio = inserirEst(inicio, "Estrela");
	inicio = inserirEst(inicio, "Saldanha"); //VERMELHO
	inicio = inserirEst(inicio, "Campo Pequeno");
	inicio = inserirEst(inicio, "Entre Campos");
	inicio = inserirEst(inicio, "Cidade Universitaria");
	inicio = inserirEst(inicio, "Campo Grande"); //AMARELO
	inicio = inserirEst(inicio, "Roma");
	inicio = inserirEst(inicio, "Alameda"); //VERMELHO
	inicio = inserirEst(inicio, "Intendente");
	inicio = inserirEst(inicio, "Rossio");
	inicio = inserirEst(inicio, "Cais do Sodre");
	// VERMELHO
	inicio = inserirEst(inicio, "Olaias");
	inicio = inserirEst(inicio, "Olivais");
	inicio = inserirEst(inicio, "Oriente");
	inicio = inserirEst(inicio, "Encarnacao");
	inicio = inserirEst(inicio, "Aeroporto");
	// AMARELO
	inicio = inserirEst(inicio, "Telheiras");
	inicio = inserirEst(inicio, "Ameixoeira");
	inicio = inserirEst(inicio, "Senhor Roubado");
	inicio = inserirEst(inicio, "Odivelas");
	
	inserirVizinhos(inicio, inicio, "Reboleira", "Colegio Militar / Luz", 10);
	inserirVizinhos(inicio, inicio, "Colegio Militar / Luz", "Jardim Zoologico", 6);
	inserirVizinhos(inicio, inicio, "Jardim Zoologico", "Praca de Espanha", 2);
	inserirVizinhos(inicio, inicio, "Praca de Espanha", "S.Sebastiao", 2);
	inserirVizinhos(inicio, inicio, "S.Sebastiao", "Marques de Pombal", 4);
	inserirVizinhos(inicio, inicio, "S.Sebastiao", "Saldanha", 2);
	inserirVizinhos(inicio, inicio, "Marques de Pombal", "Restauradores", 4);
	inserirVizinhos(inicio, inicio, "Marques de Pombal", "Estrela", 2);
	inserirVizinhos(inicio, inicio, "Marques de Pombal", "Saldanha", 4);
	inserirVizinhos(inicio, inicio, "Restauradores", "Baixa-Chiado", 2);
	inserirVizinhos(inicio, inicio, "Baixa-Chiado", "Cais do Sodre", 2);
	inserirVizinhos(inicio, inicio, "Baixa-Chiado", "Terreiro do Paco", 2);
	inserirVizinhos(inicio, inicio, "Baixa-Chiado", "Rossio", 2);
	inserirVizinhos(inicio, inicio, "Terreiro do Paco", "Santa Apolonia", 2);
	inserirVizinhos(inicio, inicio, "Saldanha", "Alameda", 2);
	inserirVizinhos(inicio, inicio, "Saldanha", "Campo Pequeno", 2);
	inserirVizinhos(inicio, inicio, "Campo Pequeno", "Entre Campos", 2);
	inserirVizinhos(inicio, inicio, "Entre Campos", "Cidade Universitaria", 2);
	inserirVizinhos(inicio, inicio, "Cidade Universitaria", "Campo Grande", 2);
	inserirVizinhos(inicio, inicio, "Campo Grande", "Telheiras", 2);
	inserirVizinhos(inicio, inicio, "Campo Grande", "Ameixoeira", 4);
	inserirVizinhos(inicio, inicio, "Campo Grande", "Roma", 4);
	inserirVizinhos(inicio, inicio, "Roma", "Alameda", 4);
	inserirVizinhos(inicio, inicio, "Alameda", "Olaias", 2);
	inserirVizinhos(inicio, inicio, "Alameda", "Intendente", 6);
	inserirVizinhos(inicio, inicio, "Intendente", "Rossio", 4);
	inserirVizinhos(inicio, inicio, "Olaias", "Olivais", 6);
	inserirVizinhos(inicio, inicio, "Olivais", "Oriente", 4);
	inserirVizinhos(inicio, inicio, "Oriente", "Encarnacao", 4);
	inserirVizinhos(inicio, inicio, "Encarnacao", "Aeroporto", 2);
	inserirVizinhos(inicio, inicio, "Ameixoeira", "Senhor Roubado", 4);
	inserirVizinhos(inicio, inicio, "Senhor Roubado", "Odivelas", 2);
	
	//AZUL
	addLinhaEst(inicio, comeco, "Reboleira", "Azul");
	addLinhaEst(inicio, comeco, "Colegio Militar / Luz", "Azul");
	addLinhaEst(inicio, comeco, "Jardim Zoologico", "Azul");
	addLinhaEst(inicio, comeco, "Praca de Espanha", "Azul");
	addLinhaEst(inicio, comeco, "S.Sebastiao", "Azul");
	addLinhaEst(inicio, comeco, "Marques de Pombal", "Azul");
	addLinhaEst(inicio, comeco, "Restauradores", "Azul");
	addLinhaEst(inicio, comeco, "Baixa-Chiado", "Azul");
	addLinhaEst(inicio, comeco, "Terreiro do Paco", "Azul");
	addLinhaEst(inicio, comeco, "Santa Apolonia", "Azul");
	//VERDE
	addLinhaEst(inicio, comeco, "Estrela", "Verde");
	addLinhaEst(inicio, comeco, "Saldanha", "Verde");
	addLinhaEst(inicio, comeco, "Campo Pequeno", "Verde");
	addLinhaEst(inicio, comeco, "Entre Campos", "Verde");
	addLinhaEst(inicio, comeco, "Cidade Universitaria", "Verde");
	addLinhaEst(inicio, comeco, "Campo Grande", "Verde");
	addLinhaEst(inicio, comeco, "Roma", "Verde");
	addLinhaEst(inicio, comeco, "Alameda", "Verde");
	addLinhaEst(inicio, comeco, "Intendente", "Verde");
	addLinhaEst(inicio, comeco, "Rossio", "Verde");
	addLinhaEst(inicio, comeco, "Cais do Sodre", "Verde");
	addLinhaEst(inicio, comeco, "Marques de Pombal", "Verde");
	addLinhaEst(inicio, comeco, "Baixa-Chiado", "Verde");
	//VERMELHO
	addLinhaEst(inicio, comeco, "Olaias", "Vermelho");
	addLinhaEst(inicio, comeco, "Olivais", "Vermelho");
	addLinhaEst(inicio, comeco, "Oriente", "Vermelho");
	addLinhaEst(inicio, comeco, "Encarnacao", "Vermelho");
	addLinhaEst(inicio, comeco, "Aeroporto", "Vermelho");
	addLinhaEst(inicio, comeco, "S.Sebastiao", "Vermelho");
	addLinhaEst(inicio, comeco, "Saldanha", "Vermelho");
	addLinhaEst(inicio, comeco, "Alameda", "Vermelho");
	//AMARELO
	addLinhaEst(inicio, comeco, "Telheiras", "Amarelo");
	addLinhaEst(inicio, comeco, "Ameixoeira", "Amarelo");
	addLinhaEst(inicio, comeco, "Senhor Roubado", "Amarelo");
	addLinhaEst(inicio, comeco, "Odivelas", "Amarelo");
	addLinhaEst(inicio, comeco, "Campo Grande", "Amarelo");
	
	return inicio;
}
	
Linha *iniciarLin(Linha *comeco){			//ADICIONA AS LINHAS QUE FORAM PEDIDAS NO MAPA
	comeco = inserirLinhas(comeco, "Verde");
	comeco = inserirLinhas(comeco, "Azul");
	comeco = inserirLinhas(comeco, "Vermelho");
	comeco = inserirLinhas(comeco, "Amarelo");
	return comeco;
}

Est *procurarEst(Est *estacao, char nome[30]){			//FUNCAO PARA PROCURAR ESTACAO BASEADA EM UM NOME PASSADO PELO USUARIO
	if(strcmp(estacao->nome, nome)){
		if(estacao->prox != NULL){
			return procurarEst(estacao->prox, nome);
		}
		else{
			return NULL;
		}
	}
	else{
		return estacao;
	}
}

void ativarEstacao(Est *estacao){		//ATIVAR ESTACAO CASO ESTEJA TEMPORARIAMENTE DESATIVADA
	if(estacao->estado == 1){
		printf("Estacao ja funcionando.\n");
	}
	else{
		printf("Ativando estacao.\n");
		estacao->estado = 1;		
	}
}

void desativarEstacao(Est *estacao){		//DESATIVAR ESTACAO CASO ESTEJA ATIVADA
	if(estacao->estado == 0){
		printf("Estacao ja desativada.\n");
	}
	else{
		printf("Desativando estacao.\n");
		estacao->estado = 0;		
	}
}

Est *excluirEstacao(Est *inicio, Est *remover){			//REMOVER ESTACAO QUE O USUARIO ESCOLHEU
	Viz *p;
	for(p = remover->vizinhos; p != NULL; p = p->prox){
		removerVizinhos(p->vizinho, remover->nome);
	}
	Est *p1;
	if(!strcmp(inicio->nome, remover->nome)){
		inicio = inicio->prox;
		free(remover);
	}
	else{
		for(p1 = inicio; strcmp(p1->prox->nome, remover->nome) && p1->prox != NULL; p1 = p1->prox);
		p1->prox = remover->prox;
		free(remover);
	}
	return inicio;
}

Est *alterarEstacao(Est *inicio, Linha *comeco, char nome[30]){		//FUNCAO PARA OPCOES DE ALTERACAO DE UMA ESTACAO ESCOLHIDA PELO USUARIO
	int escolha, peso;
	char nomeV[30], cor[20];
	Est *p = procurarEst(inicio, nome);
	if(p != NULL){
		do{
			printf("1. Desativar estacao temporariamente.\n2. Ativar estacao\n3. Excluir estacao permanentemente.\n4. Inserir estacao vizinha.\n5. Remover estacao vizinha.\n6. Alterar nome da estacao.\n7. Sair.\n");
			printf("\nDigite o que deseja fazer com a estacao: ");
			scanf("%d", &escolha);
			if(escolha == 1){
				desativarEstacao(p);
			}
			else if(escolha == 2){
				ativarEstacao(p);
			}
			else if(escolha == 3){
				inicio = excluirEstacao(inicio, p);
			}
			else if(escolha == 4){
				printf("Digite o nome do vizinho que deseja adicionar a esta estacao: ");
				fflush(stdin);
				gets(nomeV);
				printf("Agora digite a distancia entre essas duas estacoes (de 2 a 10): ");
				scanf("%d", &peso);
				inserirVizinhos(inicio, inicio, p->nome ,nomeV, peso);
			}
			else if(escolha == 5){
				printf("Digite o nome do vizinho que deseja remover desta estacao: ");
				fflush(stdin);
				gets(nomeV);
				removerVizinhos(p, nomeV);
			}
			else if(escolha == 6){
				printf("Digite o novo nome da estacao: ");
				fflush(stdin);
				gets(p->nome);
			}
			else if(escolha == 7){
				printf("Saindo...\n");
			}
			else{
				printf("Tente novamente.\n");
			}
		}while(escolha != 7);
	}
	else{
		printf("Estacao nao encontrada.\n");
	}
	return inicio;
}

void melhoresPontos(Est *estacao, int maiorG){		//MOSTRA AO USUARIO AS ESTACOES COM MAIOR GRAU
	if(estacao != NULL){
		Viz *p;
		int c;
		for(p = estacao->vizinhos, c = 0; p != NULL; p = p->prox){
			c++;	
		}
		if(maiorG == c){
				printf("%s - ", estacao->nome);
		}
		melhoresPontos(estacao->prox, maiorG);
	}
	else{
		return;
	}
}

int procurarGrau(Est *estacao, int c){		//PROCURA QUAL E O MAIOR GRAU ENTRE TODAS AS ESTACOES E SALVA
	Viz *p;
	for(p = estacao->vizinhos; p != NULL; p = p->prox){
		c++;
	}
	return c;
}

void maiorGrau(Est *inicio){		//FUNCAO QUE INICIA A PESQUISA POR MAIOR GRAU ENTRE AS ESTACOES
	int c, maiorG = 0;
	Est *p;
	Viz *p1;
	for(p = inicio; p != NULL; p = p->prox){
		c = procurarGrau(p, 0);
		if(c > maiorG){
			maiorG = c;
		}
	}
	printf("Melhores estacoes para turismo - ");
	melhoresPontos(inicio, maiorG);
	printf("\n");
}

int procurarCaminhoEuler(Est *est1, Est *est2){		//FUNCAO QUE TESTA SUBGRAFO DE EULER
	if(est1 == NULL){
		return 0;
	}
	est1->visit = 1;
	if(!strcmp(est1->nome, est2->nome)){
		if((procurarGrau(est1, 0) % 2) == 0){
			return 1;
		}
	}
	else{
		Viz *vizinhos;
		for(vizinhos = est1->vizinhos; vizinhos != NULL; vizinhos = vizinhos->prox){
			if(!vizinhos->vizinho->visit){
				if(procurarCaminhoEuler(vizinhos->vizinho, est2)){
					if((procurarGrau(est1, 0) % 2) == 0){
						return 1;
					}
				}
			}
		}
		return 0;
	}
}

void euler(Est *inicio){		//FUNCAO QUE MOSTRA AO USUARIO SE DUAS ESTACOES FORMAM UM GRAFO DE EULER
	char nome[30], nome1[30];
	printf("Digite o nome de duas estacoes: ");
	fflush(stdin);
	gets(nome);
	fflush(stdin);
	gets(nome1);
	Est *p, *eu = NULL, *eu1 = NULL;
	for(p = inicio; p != NULL; p = p->prox){
		if(!strcmp(nome, p->nome)){
			eu = p;
		}
		if(!strcmp(nome1, p->nome)){
			eu1 = p;
		}
	}
	if(eu == eu1 && eu != NULL && eu1 != NULL){
		printf("Estacoes iguais.\n");
		return;
	}
	else if(eu == NULL || eu1 == NULL){
		printf("Estacao(oes) nao encontrada(s).\n");
	}
	else{
		zerarNos(inicio);
		if(procurarCaminhoEuler(eu, eu1)){
			printf("Essas duas estacoes formam um Grafo de Euler. \n");
		}
		else{
			printf("Essas duas estacoes nao formam um Grafo de Euler. \n");
		}
	}
}

void procurarLinhas(Est *estacao, char cor[20]){ 		//FUNCAO QUE MOSTRA AO USUARIO ESTACOES PERTENCENTES A UMA LINHA
	LinhaE *p;
	if(estacao != NULL){
		for(p = estacao->linhas; p != NULL; p = p->prox){
			if(!strcmp(p->linha->cor, cor)){
				printf("%s\n", estacao->nome);
			}
		}
		procurarLinhas(estacao->prox, cor);
	}
	else{
		return;
	}
}

int rotaEst(Est *est1, Est *est2, int i, int c){		//FUNCAO QUE MOSTRA AO USUARIO ROTA DISPONIVEL
	if(est1 == NULL){
		return 0;
	}
	est1->visit = 1;
	if(!strcmp(est1->nome, est2->nome)){
		if(c < 2){
			printf("A distancia pode ser percorrida a pe.\n");
		}
		printf("Tempo de viagem: %d\n%s - ", i, est1->nome);
		return 1;
	}
	else{
		Viz *vizinhos;
		for(vizinhos = est1->vizinhos; vizinhos != NULL; vizinhos = vizinhos->prox){
			if(!vizinhos->vizinho->visit){
				i = i + vizinhos->peso;
				if(rotaEst(vizinhos->vizinho, est2, i, c+1)){
					if(est1->estado != 0){
						printf("%s - ", est1->nome);
					}
					return 1;
				}
			}
		}
		return 0;
	}
}

void procurarRotas(Est *inicio){		//FUNCAO QUE PROCURA DUAS ROTAS DISPONIVEIS PRO USUARIO
	char nome[30], nome1[30];
	Est *est1, *est2;
	printf("Digite o nome da estacao inicial: ");
	fflush(stdin);
	gets(nome);
	printf("Digite o nome da estacao destino: ");
	fflush(stdin);
	gets(nome1);
	est1 = procurarEst(inicio, nome);
	est2 = procurarEst(inicio, nome1);
	if(est1 == NULL || est2 == NULL){
		printf("Estacao(oes) invalida(s).");
	}
	else{
		zerarNos(inicio);
		if(rotaEst(est1->vizinhos->vizinho, est2, 0, 0)){
			printf("%s\n", est1->nome);
		}
		zerarNos(inicio);
		if(est1->vizinhos->prox != NULL){
			printf("Ou\n");
			if(rotaEst(est1->vizinhos->prox->vizinho, est2, 0, 0)){
				printf("%s\n", est1->nome);
			}
		}
	}
}

int main(){			//FUNCAO MAIN, POSSUI UM MENU E AS INICIACOES DO PROGRAMA
	Est *inicio = NULL;
	Linha *comeco = NULL;
	int escolha;
	char nome[30], cor[20];
	comeco = iniciarLin(comeco);
	inicio = iniciarEst(inicio, comeco);
	do{
		system("cls");
		printf("1. Adicionar nova estacao.\n2. Adicionar nova linha.\n3. Alterar Estacao.\n4. Relatorio de todas as estacoes, suas linhas e vizinhos. \n5. Estacoes mais importantes(maior grau).\n6. Euler.\n7. Caminho para linhas.\n8. Rotas.\n9. Sair.");
		printf("\nDigite sua escolha: ");
		scanf("%d", &escolha);
		if(escolha == 1){
			system("cls");
			printf("Digite o nome da estacao: ");
			fflush(stdin);
			gets(nome);
			inicio = inserirEst(inicio, nome);
			printf("Digite a cor da linha que a estacao pertence ou digite 'Nao' para criar uma estacao fora de todas as linhas: ");
			gets(cor);
			if(strcmp(cor, "Nao")){
				addLinhaEst(inicio, comeco, nome, cor);
			}
		}
		else if(escolha == 2){
			system("cls");
			printf("Digite a cor que representa a linha: ");
			fflush(stdin);
			gets(cor);
			comeco = inserirLinhas(comeco, cor);
			printf("Digite o nome de uma estacao para ser adicionada a essa nova linha ou digite 'Nao' para criar uma linha vazia: ");
			gets(nome);
			if(strcmp(nome, "Nao")){
				addLinhaEst(inicio, comeco, nome, cor);
			}
		}
		else if(escolha == 3){
			system("cls");
			printf("Digite o nome da estacao que deseja alterar: ");
			fflush(stdin);
			gets(nome);
			inicio = alterarEstacao(inicio, comeco, nome);
		}
		else if(escolha == 4){
			system("cls");
			zerarNos(inicio);
			mostrarNos(inicio);
			printf("\n");
			system("pause");
		}
		else if(escolha == 5){
			system("cls");
			maiorGrau(inicio);
			system("pause");
		}
		else if(escolha == 6){
			system("cls");
			euler(inicio);
			system("pause");
		}
		else if(escolha == 7){
			system("cls");
			printf("Digite a linha que deseja chegar: ");
			fflush(stdin);
			gets(cor);
			printf("Estacoes que sao dessa linha: \n");
			procurarLinhas(inicio, cor);
			printf("\n");
			system("pause");
		}
		else if(escolha == 8){
			system("cls");
			procurarRotas(inicio);
			system("pause");
		}
		else if(escolha == 9){
			printf("Saindo...");
		}
		else{
			printf("Tente novamente...\n");
		}
	}while(escolha != 9);
	free(comeco);
	free(inicio);
	return 0;
}