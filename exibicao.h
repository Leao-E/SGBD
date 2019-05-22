
void mostrar_tabela(char *nome);
int alinhamento_coluna(char** vtr,char* anterior, int col, int lin,int x);
void listar_tabelas (char *nome);

//==========================Funcoes================================================================
//exibe a tabela no terminal para o usuario.
//recebe o nome da tabela que vai ser exibida.
//nao tem retorno
void mostrar_tabela(char *nome){
	FILE *setup_tab;
	char **tabela, arquivo_setup[60];
	int colunas = 0, linhas = 0, quebra_linha = 0, alinhamento = 0;
	int maior_tamanho = 0, soma_tamanhos = 0, jump = 0, aux = 0;
	tabela = carrega_tabela(nome);

	strcpy(arquivo_setup, nome);
	strcat(arquivo_setup, ".setup");
	setup_tab = fopen(arquivo_setup, "r");
	if(setup_tab == NULL){
		fprintf(stderr, "Erro na abertura do arquivo");
	}else{
		fscanf(setup_tab, "%d %d\n", &colunas, &linhas);
	}
	//mostrando a tabela
	for(int j = 0;j < colunas;j++){
		jump = j;
		for(int i = 0;i < linhas;i++){
			if(strlen(tabela[jump]) > maior_tamanho){
				maior_tamanho = strlen(tabela[jump]);
			}
			jump += colunas;
		}
		soma_tamanhos += maior_tamanho;
		maior_tamanho = 0;
	}
	soma_tamanhos += 3*colunas+1;
	for(int i = 0;i < soma_tamanhos;i++){
		printf("-");
	}
	printf("\n");

	for(int i = 0;i < colunas*linhas;i++){
		
		if(quebra_linha != 0 ){
			alinhamento = alinhamento_coluna(tabela,tabela[i-1],colunas,linhas,quebra_linha-1);
			for(int i = 0;i < alinhamento;i++){
				printf(" ");
			}
			printf("| %s",tabela[i]);
			aux += strlen(tabela[i]) + alinhamento;
		}else{
			printf("| %s",tabela[i]);
			aux += strlen(tabela[i]);
		}
		quebra_linha++;
		if(quebra_linha == colunas){
			quebra_linha = 0;
			aux += 2*colunas+1;
			for(int x = aux; x < soma_tamanhos;x++){
				printf(" ");
			}
			aux = 0;
			printf("|");
			printf("\n");
			if(i == colunas-1){
				for(int i = 0;i < soma_tamanhos;i++){
					printf("-");
				}
				printf("\n");
			}
		}
		
	}

	for(int i = 0;i < soma_tamanhos;i++){
		printf("-");
	}
	printf("\n");

//libera a memoria da alocacao dinamica e fecha a stream
	for(int i = 0;i < colunas*linhas;i++){
		free(tabela[i]);
	}
	free(tabela);
	fclose(setup_tab);
}
//calcula a quantidade de espacos necessario para alinhar as colunas da tabela.
//recebe o vetor que contém todos os dados da tabela, a string anterior a que vai ser escrita na tela,
//a quantidade de colunas e linhas da tabela e a coluna atual(0,1,...,etc)
//retorna a quantidade de espacos.
int alinhamento_coluna(char** vtr,char* anterior, int col, int lin,int x){
	int maior_tamanho = 0, aux = 0,jump = 0;

	jump = x;
	for(int i = 0;i < lin;i++){
		if(strlen(vtr[jump]) > maior_tamanho){
			maior_tamanho = strlen(vtr[jump]);
		}
		jump += col;
	}

	aux = maior_tamanho - strlen(anterior);
	aux = abs(aux);
	

	return aux+1;

}
//Listas o nome de todas as tabelas criadas ate o monento.
void listar_tabelas (char *nome){
	char vtr[60];
	int aux, cont = 0;
	FILE *arquivo;
	arquivo = fopen(nome, "a+");
	if (arquivo == NULL){
		printf("erro na abertura\n");
	}else{
		printf("tabelas disponiveis:\n");
		do{
			aux = fscanf(arquivo, "%s", vtr);
			if (aux != (-1)){
				printf("	%s\n", vtr);
				cont++;	
			}else{
				if(cont == 0){
					printf("	nenhum arquivo encontrado\n");
				}
			}
		}while(aux != (-1));
	}
	fclose(arquivo);
}
//