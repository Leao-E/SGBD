
int apagar_tabela (char *str_1, char *str_2);
int apagar_linha (char* str_name, char* str_pk);

/*retornos:
0 - execucao ok
1 - tabela nao existe
-1 - erro na abertura
str_1 eh o nome da lista de tabelas str_2 eh o nome da tabela*/
int apagar_tabela (char *str_1, char *str_2){
	FILE *file_1, *file_2;
	char tester[60], backup[60], string_aux[60];
	int aux;
	strcpy(backup, str_1);
	//abre canais
	file_1 = fopen(str_1, "r");
	file_2 = fopen(strcat(backup,".backup"), "w");
	//checa se arquivo existe
	if (file_1 == NULL){
		//comunica e fecha canais, remove backup
		printf("erro na abertura\n");
		fclose(file_1);
		fclose(file_2);
		remove(backup);
		return -1;
	}else{
		//se o arquvo existir, checa a tabela existe
		//checa o nome da tabela
		do{
			aux = fscanf(file_1, "%s", tester);
			if(strcmp(tester, str_2) == 0){
				aux = -1;
			} 
		}while(aux != (-1));
		//se a tabela existir copia para o backup as tabelas
		//exceto a excluida
		if (strcmp(tester, str_2) == 0){
			//retorna para o inicio
			fseek(file_1, 0, SEEK_SET);
			//comecando a copiar ate que chegue no fim do arquivo
			do{
				aux = fscanf(file_1, "%s", tester);
				if (strcmp(tester, str_2) != 0 && aux != EOF){
					fprintf(file_2, "%s\n", tester);
				}
			}while(aux != (-1));
			//fecha canais
			fclose(file_1);
			fclose(file_2);
			//remove a tabela e a lista
			remove(str_1);
			remove(str_2);
			//remove a .setup
			strcpy(string_aux, str_2);
			strcat(string_aux, ".setup");
			remove(string_aux);
			//renomeia o backup como o nova lista de tabelas
			rename(backup, str_1);
			return 0;
		}else{
			//se a tabela nao existir apaga backup e fecha os canais
			printf("tabela nao existe\n");
			fclose(file_1);
			fclose(file_2);
			remove(backup);
			return 1;
		}
	}
}

//fazer funcao pra checar se a pk inserida existe
//essa funcao entra com a pk na forma de string (str_pk),
//e o nome da tabela (str_name)
/*
retornos padrao 
	-1 > setup da tabela nao existe
	 1 > tabela nao existe
	 0 > RUNOK
	 2 > impossivel criar backup
*/
int apagar_linha (char* str_name, char* str_pk){
	FILE *table, *setup, *backup;
	char** lines_data;
	char aux_string[60];
	strcpy(aux_string, str_name);
	int pk_col, n_col, n_lin, aux_int, aux_pk = 1;
	//abrindo o canal setup e table
	setup = fopen(strcat(aux_string, ".setup"), "r+");
	table = fopen(str_name, "r");
	//checa abertura da file
	if ((setup == NULL) || (table == NULL)){
		printf("erro na execucao\n");
		if (setup == NULL){
			printf("setup nao existe\n");
			fclose(setup);
			fclose(table);
			return 1;
		}
		printf("tabela nao existe\n");
		fclose(setup);
		fclose(table);
		return -1;
	}else{
		//recebendo o numero de colunas
		fscanf(setup, "%d %d", &n_col, &n_lin);
		//printf("%d %d\n", n_col, n_lin); >> debug
		//alocando memoria para receber os dados
		lines_data = (char**)calloc(n_col, sizeof(char*));
		for(int i = 0; i < n_col; ++i){
			*(lines_data+i) = (char*)calloc(60, sizeof(char));
		}
		//recebendo os nomes para determinar a posicao da pk
		for(int i = 0; i < n_col; ++i){
			aux_int = fscanf(table, "%s", *(lines_data+i));
			if(strchr(*(lines_data+i), '*') != NULL){
				pk_col = i;
			}
		}
		//abrindo file backup pra transferir os dados
		backup = fopen("backup", "w");
		//caso erro, fechamento da execucao, libera memoria e fecha canais
		if (backup == NULL){
			printf("erro inesperado na execucao\n");
			for(int i = 0; i < n_col; ++i){
				free(*(lines_data+i));
			}
			free(lines_data);
			fclose(table);
			fclose(backup);
			fclose(setup);
			remove("backup");
			return 2;
		}else{
			//imprime a tabela no backup ate o EOF
			do{
				//checa se a string eh diferente da pk e de EOF, se sim imprime no backup
				if(strcmp(*(lines_data+pk_col), str_pk) != 0 && aux_int != (-1)){
					for(int i = 0; i < n_col; ++i){
						fprintf(backup, "%s ", *(lines_data+i));
					}
					for(int i = 0; i < n_col; ++i){
						aux_int = fscanf(table, "%s", *(lines_data+i));
					}
					//se for diferente de EOF pula linha
					if(aux_int != (-1)){
						fprintf(backup, "\n");
					}
				}else{
					if(strcmp(*(lines_data+pk_col), str_pk) == 0){
						//se chegar a esse ponto entao a pk existe e 
						//vai ser eliminada da tabela, logo
						//pode diminuir a as linhas do setup
						fseek(setup, 0, SEEK_SET);
						fprintf(setup, "%d %d\n", n_col, (n_lin-1));
						aux_pk = 0;
					}
					//se chegar aqui eh pq uma das condicoes nao foi satisfeita, provavelmente
					//a pk eh igual a inserida, caso contrario, EOF, o programa teria encerrado
					for(int i = 0; i < n_col; ++i){
						aux_int = fscanf(table, "%s", *(lines_data+i));
					}
				}
			}while(aux_int != (-1));
		}
		//liberando memoria e fechando canal
		for(int i = 0; i < n_col; ++i){
			free(*(lines_data+i));
		}
		free(lines_data);
		fclose(table);
		fclose(setup);
		fclose(backup);
		//tornando a backup a nova tabela
		remove(str_name);
		rename("backup", str_name);
		return aux_pk;
	}
}