
char** carrega_tabela(char *nome_tab);
int check_int(char *valor);
int check_float(char *str);
int check_char(char *str);
void mostrar_sintaxe_correta(char *comando);
int check_tabela_existe(char* nome);
int checa_pk_existe(char* str_nome, char* str_pk);

//carrega a tabela na memoria
//recebe o nome da tabela
//retorna um ponteiro de ponteiro de char(retorna um pointeiro para um vetor de string).
//retorna o vetor que contem todos os dados da tebela.
char** carrega_tabela(char *nome_tab){
	FILE *tabela, *setup_tab;
	char setup[60], **matriz;
	int colunas = 0, linhas = 0;

	strcpy(setup, nome_tab);
	strcat(setup, ".setup");
	tabela = fopen(nome_tab, "r");
	if(tabela == NULL){
		printf("Erro na abertura do arquivo!\n");
		fprintf(stderr, "Erro na abertura do arquivo");
	}
	setup_tab = fopen(setup, "r");
	if(setup_tab == NULL){
		printf("Erro na abertura do arquivo!\n");
		fprintf(stderr, "Erro na abertura do arquivo");
	}
//lendo do arquivo tabela.setup  a qtd. de colunas e linhas.
	fseek(setup_tab, 0, SEEK_SET);
	fscanf(setup_tab, "%d %d\n",&colunas, &linhas);
//alocando a memoria para a matriz
	matriz = (char**) malloc((colunas*linhas)*sizeof(char*));
	if(matriz == NULL){
		printf("Erro na memoria!\n");
		fprintf(stderr, "Erro na memoria");
	}else{
		for(int i = 0;i < colunas*linhas;i++){
			matriz[i] = (char*) malloc(1*sizeof(setup));
		}
	}
//copiando os dados da tabela para a matriz
	for(int i = 0;i < colunas*linhas;i++){
		fscanf(tabela, "%s ", matriz[i]);
	}
//fechando as streams
	fclose(tabela);
	fclose(setup_tab);

	return matriz;
}
//checa se a tabela existe
//retorna 1 se a tabela não existe
//retorna 0 caso a tabela exista
int check_tabela_existe(char* nome){
	FILE *tabelas;
	char tab_nome[60];
	int aux = 0;

	tabelas = fopen("lista_tabelas", "a+");

	if(tabelas == NULL){
		printf("Erro na abertura do arquivo\n");
	}else{
		while(aux != -1){
			aux = fscanf(tabelas, "%s\n", tab_nome);
			if(strcmp(tab_nome, nome) == 0){
				fclose(tabelas);
				return 0;
			}
		}
	}
	fclose(tabelas);
	return 1;
}
//=========================================Funcao check tipo==========================================
//checa se a string eh composta por inteiro;
// -1 > erro, 0 > eh int, 1 > nao eh int
int check_int(char *str){
	int aux = -1;
	for (int i = 0; i < strlen(str); ++i){
		if (*(str+i) >= 48 && *(str+i) <= 57){
			aux = 0;
		}
		else{
			return 1;
		}
	}
	return aux;
}
//checa se a string representa um valor float;
// -1 > erro, 0 > eh int, 1 > nao eh int
int check_float(char *str){
	int dotcont = 0, aux = -1;
	for (int i = 0; i < strlen(str); ++i){
		if (*(str+i) >= 48 && *(str+i) <= 57){
			aux = 0;
		}else{
			if (*(str+i) == 46){
				dotcont++;
			}
			else{
				return 1;
			}
		}
		if (dotcont > 1){
			return 1;
		}
	}
	return aux;
}
/*checa se a string tem um unico elemento, caso sim retorna 0.
caso contrario retorna 0. caso erro retorna -1*/
int check_char(char *str){
	if (strlen(str) != 1){
		return 1;
	}else{
		return 0;
	}
	return -1;
}
/*reduz para letra minuscula todas as letras da string passada
por referÃªncia*/
void abaixar_caixa(char *string){
	for(int i = 0; i < strlen(string); ++i){
		if(*(string+i) >= 60 && *(string+i) <= 90){
			*(string+i) += 32;
		}
	}
}

void mostrar_sintaxe_correta(char *comando){
	if(strcmp(comando, "criar_tabela") == 0){
		printf("sintaxe : criar_tabela nome_tabela tipo nome_campo1,int nome_campo2*,...,tipo nome_campoN,\n");
	}else if(strcmp(comando, "inserir_linha") == 0){
		printf("Sintaxe 1: inserir_linha nome_tabela valor1,valor2,valor3,...,valorN,\n");
		printf("Sintaxe 2: inserir_linha nome_tabela \n");
	}else if(strcmp(comando, "mostrar_tabela") == 0){
		printf("Sintaxe : mostrar_tabela nome_tabela\n");
	}else if(strcmp(comando, "buscar_valor") == 0){
		printf("Sintaxe : buscar_valor nome_tabela nome_campo\n");
	}else if(strcmp(comando, "apagar_linha") == 0){
		printf("Sintaxe: apagar_linha nome_tabela chave primaria da linha\n");
	}else if(strcmp(comando, "apagar_tabela") == 0){
		printf("Sintaxe : apagar_tabela nome_tabela\n");
	}else if(strcmp(comando, "ordenar_afb") == 0){
		printf("Sintaxe: ordenar_afb nome_tabela nome_campo\n");
	}else if(strcmp(comando, "inserir_coluna") == 0){
		printf("Sintaxe: inserir_coluna nome_tabela tipo nome_novoCampo\n");
	}else if(strcmp(comando, "sintax") == 0){
		printf("Sintaxe : sintax comando\n");
	}else if(strcmp(comando, "listar_tabelas") == 0){
		printf("Sintaxe: listar_tabelas\n");
	}else if(strcmp(comando, "limpar_tela") == 0){
		printf("Sintaxe: limpar_tela\n");
	}else if(strcmp(comando, "sair") == 0){
		printf("sair\n");
	}else if(strcmp(comando, "copiar_tabela") == 0){
		printf("Sintaxe: copiar_tabela nome_tabela novo_nome\n");
	}else if(strcmp(comando, "apagar_coluna") == 0){
		printf("Sintaxe: apagar_coluna nome_tabela nome_campo\n");
	}else if(strcmp(comando, "editar_linha") == 0){
		printf("Sintaxe: editar_linha nome_tabela nome_campo chave_primaria\n");
	}else{
		printf("comando invalido!\n");
	}
}

//1 eh erro
//0 a pk existe
int checa_pk_existe(char* str_nome, char* str_pk){
	char aux[60];
	strcpy(aux, str_nome);
	strcat(aux, ".setup");
	FILE *tabela, *setup;
	tabela = fopen(str_nome, "r");
	setup= fopen(aux, "r");
	if(tabela == NULL || setup == NULL){
		fclose(tabela);
		fclose(setup);
		return 1;
	}else{
		char campo[60];
		int int_aux, n_col, col_pk, aux_ret = 1;
		fscanf(setup, "%d", &n_col);
		for(int i = 0; i < n_col; ++i){
			fscanf(tabela, "%s", campo);
			if (strchr(campo, '*') != NULL){
				col_pk = i;
			}
		}
		do{
			for(int i = 0; i < n_col; ++i){
				int_aux = fscanf(tabela, "%s", campo);
				if (i == col_pk){
					if(strcmp(campo, str_pk) == 0){
						aux_ret = 0;
					}
				}
			}
		}while(int_aux != EOF);
		fclose(tabela);
		fclose(setup);
		return aux_ret;
	}
}