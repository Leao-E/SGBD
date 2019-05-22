
void ajuda(char *comando);
int ordenar(char* nome_tabela, char *nome_campo, int modo);
int inserir_coluna(char *nome_tabela, char *campo);
int copiar_tabela(char *str_tabela, char *str_nome);
int apagar_coluna(char *str_nome, char *str_campo);
int checar_tipo_valor(char *str_tipo, char *str_valor);
int editar_linha(char *str_nome, char *str_campo, char *str_pk);

//=========================================extras====================================================
//funcao ordenar: ordena alfabeticamente a tabela
//recebe o nome da tabela e o nome do campo pelo qual a tabela vai ser ordenada
//retorna 1 se ordenor ordenar
//retorna 0 se o campo escolhido nao e valido
int ordenar(char* nome_tabela, char *nome_campo, int modo){
	FILE *tabela, *tabela_setup, *nova_tab, *nova_tab_setup, *lista_tabelas;
	FILE *tab_temporaria, *temporaria_setup;
	char **tab, **tab_copia, **pk,*aux_ptr, tab_setup[60], aux_string[16],tipo_campo_temp[16];
	char escolha_usuario[1], nome_nova_tabela[60], nova_tabela_setup[60], tipo_campo[16];
	int colunas = 0, linhas = 0;
	int pk_campo = 0,campo = 0,jump = 0, jump2 = 0, aux = 0;
	int check_campo = -1;

	strcpy(tab_setup, nome_tabela);
	strcat(tab_setup, ".setup");
//abertura dos arquivos tabela e tabela.setup
	tabela = fopen(nome_tabela, "r+");
	if(tabela == NULL){
		fprintf(stderr, "Erro na abertura do arquivo");
	} 
	tabela_setup = fopen(tab_setup, "r");
	if(tabela_setup == NULL){
		fprintf(stderr, "Erro na abertura do arquivo");
	}
//carregando os dados dos arquivos tabela e tabela.setup para as variaveis
	tab = carrega_tabela(nome_tabela);
	fscanf(tabela_setup, "%d %d\n", &colunas, &linhas);
	//verifica se o campo e valido e se a tabela tem mais linhas pra ordenar
	if(linhas == 1){
		fclose(tabela);
		fclose(tabela_setup);
		return 0;
	}
	for(int i = 0;i < colunas;i++){
		if(strcmp(tab[i], nome_campo) == 0){
			check_campo = i;
			break;
		}
	}
	if(check_campo == -1){
		fclose(tabela);
		fclose(tabela_setup);
		return 0;
	}
	for(int i = 0; i <= check_campo;i++){
		fscanf(tabela_setup, "%s ", aux_string);
	}
	fseek(tabela_setup, 0, SEEK_SET);
	fscanf(tabela_setup, "%d %d\n", &colunas, &linhas);
//criando um vetor so com as chaves primarias
	//encontrando o campo da chave primaria
	for(int i = 0;i < colunas;i++){
		for(int j = 0;j < strlen(tab[i]);j++){
			if(tab[i][j] == '*'){
				pk_campo = i;
				break;
			}
		}
		if(pk_campo != 0){
			break;
		}
	}
	//copiando as chaves primarias para o vetor pk
	pk = (char**) malloc((linhas-1)*sizeof(char*));
	if(pk ==  NULL){
		fprintf(stderr, "Erro na memoria");
	}else{
		for(int i = 0;i < linhas-1;i++){
			pk[i] = (char*) malloc(1*sizeof(tab_setup));
		}
		jump = pk_campo + colunas;
		for(int i = 0;i < linhas-1;i++){
			pk[i] = tab[jump];
			jump += colunas;
		}
	}
	//fazendo uma copia do vetor tab para o tab_copia
	tab_copia = (char**) malloc((colunas*linhas)*sizeof(char*));
	if(tab_copia == NULL){
		fprintf(stderr, "Erro na memoria");
	}else{
		for(int i = 0;i < colunas*linhas;i++){
			tab_copia[i] = (char*) malloc(1*sizeof(tab_setup));
		}
		for(int i = 0;i < colunas*linhas;i++){
			strcpy(tab_copia[i], tab[i]);
		}
	}
	//transformando todas as strings em lower case
	//encontra a posicao do campo no vetor
	for(int i = 0;i < colunas;i++){
		if(strcmp(tab_copia[i], nome_campo) == 0){
			campo = i;
			break;
		}
	}
	//transformando todos os valores do campo em lower case
	if(strcmp(aux_string, "string") == 0 || strcmp(aux_string, "char") == 0){
		jump = campo + colunas;
		for(int i = 0;i < linhas-1;i++){
			for(int j = 0;j < strlen(tab_copia[jump]);j++){
				if(tab_copia[jump][j] > 40 && tab_copia[jump][j] < 91){
					tab_copia[jump][j] = tab_copia[jump][j] + 32;
				}
			}
			jump += colunas;
		}
	}
	//comparando as strings e ordenar(crescente(modo 0) ou descrecente(modo 1))
	jump = campo + colunas;
	jump2 = campo + colunas;

	if(modo == 0){
		//ordena de forma crescente
		for(int i = 0;i < linhas-1;i++){
			for(int j = 0;j < linhas-1;j++){
				if(strcmp(tab_copia[jump],tab_copia[jump2]) < 0){
					aux_ptr = tab_copia[jump];
					tab_copia[jump] = tab_copia[jump2];
					tab_copia[jump2] = aux_ptr;
					aux_ptr = pk[i];
					pk[i] = pk[j];
					pk[j] = aux_ptr;
				}
				jump2 += colunas;
			}
			jump += colunas;
			jump2 = campo + colunas;
		}
	}else{
		//ordena de forma decrescente
		for(int i = 0;i < linhas-1;i++){
			for(int j = 0;j < linhas-1;j++){
				if(strcmp(tab_copia[jump],tab_copia[jump2]) > 0){
					aux_ptr = tab_copia[jump];
					tab_copia[jump] = tab_copia[jump2];
					tab_copia[jump2] = aux_ptr;
					aux_ptr = pk[i];
					pk[i] = pk[j];
					pk[j] = aux_ptr;
				}
				jump2 += colunas;
			}
			jump += colunas;
			jump2 = campo + colunas;
		}
	}
	//mostra a tabela ordenada para o usuario
	printf("Tabela %s ordenada:\n", nome_tabela);
	tab_temporaria = fopen("NULL", "w");
	for(int i = 0;i < colunas;i++){
			fprintf(tab_temporaria, "%s ",tab[i]);
		}
		fprintf(tab_temporaria, "\n");
		jump = pk_campo + colunas;
		for(int i = 0;i < linhas-1;i++){
			for(int j = 0;j < linhas-1;j++){
				if(strcmp(pk[i], tab[jump]) == 0){
					aux = (jump - colunas*(j+1));
					aux = jump - aux;
					for(int k = 0;k < colunas;k++){
						fprintf(tab_temporaria, "%s ",tab[aux]);
						aux++;
					}
					break;
				}
				jump += colunas;
			}
			jump = pk_campo + colunas;
			fprintf(tab_temporaria, "\n");
		}

		temporaria_setup = fopen("NULL.setup", "w");
		fprintf(temporaria_setup, "%d %d\n", colunas, linhas);
		for(int i = 0;i < colunas;i++){
			fscanf(tabela_setup, "%s ", tipo_campo);
			fprintf(temporaria_setup, "%s ", tipo_campo);
		}

		fclose(temporaria_setup);
		fclose(tab_temporaria);
		mostrar_tabela("NULL");
		remove("NULL");
		remove("NULL.setup");
		fseek(tabela_setup, 0, SEEK_SET);
		fscanf(tabela_setup, "%d %d\n", &colunas, &linhas);
	//========================================
	printf("Deseja salvar a tabela ordenada ou exporta?\ns - salva a tabela\ne - exporta a tabela\nn - nao faz nada\n");
	scanf("%s", escolha_usuario);
	setbuf(stdin, NULL);

	if(escolha_usuario[0] == 's'){
		fseek(tabela, 0, SEEK_SET);
		for(int i = 0;i < colunas;i++){
			fprintf(tabela, "%s ",tab[i]);
		}
		fprintf(tabela, "\n");
		jump = pk_campo + colunas;
		for(int i = 0;i < linhas-1;i++){
			for(int j = 0;j < linhas-1;j++){
				if(strcmp(pk[i], tab[jump]) == 0){
					aux = (jump - colunas*(j+1));
					aux = jump - aux;
					for(int k = 0;k < colunas;k++){
						fprintf(tabela, "%s ",tab[aux]);
						aux++;
					}
					break;
				}
				jump += colunas;
			}
			jump = pk_campo + colunas;
			fprintf(tabela, "\n");
		}
		printf("Tabela ordenada e salva com sucesso!\n");
	}else if(escolha_usuario[0] == 'e'){
		do{
			printf("Digite o nome da nova tabela:\n");
			scanf("%s", nome_nova_tabela);
			setbuf(stdin, NULL);
			if(check_tabela_existe(nome_nova_tabela) && strcmp(nome_nova_tabela, "sair") != 0){
				//salvando o nome da nova tabela no arquivo lista_tabelas
				lista_tabelas = fopen("lista_tabelas", "a+");
				fprintf(lista_tabelas, "%s\n", nome_nova_tabela);
				fclose(lista_tabelas);
				//salvando os dados na nova tabela
				nova_tab = fopen(nome_nova_tabela, "w");
				for(int i = 0;i < colunas;i++){
					fprintf(nova_tab, "%s ",tab[i]);
				}
				fprintf(nova_tab, "\n");
				jump = pk_campo + colunas;
				for(int i = 0;i < linhas-1;i++){
					for(int j = 0;j < linhas-1;j++){
						if(strcmp(pk[i], tab[jump]) == 0){
							aux = (jump - colunas*(j+1));
							aux = jump - aux;
							for(int k = 0;k < colunas;k++){
								fprintf(nova_tab, "%s ",tab[aux]);
								aux++;
							}
							break;
						}
						jump += colunas;
					}
					jump = pk_campo + colunas;
					fprintf(nova_tab, "\n");
				}

				strcpy(nova_tabela_setup, nome_nova_tabela);
				strcat(nova_tabela_setup, ".setup");
				nova_tab_setup = fopen(nova_tabela_setup, "w");
				fprintf(nova_tab_setup, "%d %d\n", colunas, linhas);
				for(int i = 0;i < colunas;i++){
					fscanf(tabela_setup, "%s ", tipo_campo);
					fprintf(nova_tab_setup, "%s ", tipo_campo);
				}
				fclose(nova_tab_setup);
				fclose(nova_tab);
				printf("Tabela ordenada e exportada com sucesso!\n");
				strcpy(nome_nova_tabela, "sair");
			}else{
				if(strcmp(nome_nova_tabela, "sair") != 0){
					printf("A tabela %s ja existe\n", nome_nova_tabela);
				}
			}
		}while(check_tabela_existe(nome_nova_tabela) == 0 || strcmp(nome_nova_tabela, "sair") != 0);
		
	}
//liberando a memoria e fechando os canais de comunicacao
	//memoria
	for(int i = 0;i < linhas-1;i++){
		free(pk[i]);
	}
	free(pk);
	for(int i = 0;i < linhas-1;i++){
		free(tab_copia[i]);
	}
	free(tab_copia);
	//canais de comunicacao
	fclose(tabela);
	fclose(tabela_setup);
	return 1;
}

//funcao sintax
//recebe o comando
//exibi na tela a sintaxe do comaando
void ajuda(char *comando){

	if(strcmp(comando, "criar_tabela") == 0){
		printf("Comando criar_tabela:\n\n");
		printf("sintaxe : criar_tabela nome_tabela tipo nome_campo1,int nome_campo2*,...,tipo nome_campoN,\n");
		printf("\nOBS: o nome da tabela nao deve conter espacos, tipo deve ser os tipos primitivos da linguagem C (char, int, float) ou string ");
		printf("os nomes dos campos não pode ter espacos e devem ser separados por virgulas(inclusive apos o ultimo campo).\n\n");
	}else if(strcmp(comando, "inserir_linha") == 0){
		printf("Comando inserir_linha:\n\n");
		printf("Sintaxe 1: inserir_linha nome_tabela valor1,valor2,valor3,...,valorN,\n");
		printf("\nOBS: os valores devem ser separados por virgulas(inclusive apos o ultimo valor) e devem estar na mesma ordem que os campos da tabela e ser do tipo do respectivos campos.\n\n");
		printf("Sintaxe 2: inserir_linha nome_tabela \n");
		printf("\nSera mostrado os campos da tabela e seus tipos, na mesma ordem em que estao na tabela\n");
		printf("inseria os valores onde se pede.\n");
		printf("\nOBS: os valores devem ser separados por virgulas(inclusive apos o ultimo valor) e devem estar na mesma ordem que os campos da tabela e ser do tipo do respectivos campos.\n\n");
	}else if(strcmp(comando, "mostrar_tabela") == 0){
		printf("Comando mostra_tabela:\n\n");
		printf("Sintaxe : mostrar_tabela nome_tabela\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos.\n\n");
	}else if(strcmp(comando, "buscar_valor") == 0){
		printf("Comando buscar_valor:\n\n");
		printf("Sintaxe : buscar_valor nome_tabela nome_campo\n\n");
	}else if(strcmp(comando, "apagar_linha") == 0){
		printf("Comando apagar_linha:\n\n");
		printf("Sintaxe: apagar_linha nome_tabela chave primaria da linha\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos e a chave primaria deve existir\n\n");
	}else if(strcmp(comando, "apagar_tabela") == 0){
		printf("Comando apagar_tabela:\n\n");
		printf("Sintaxe : apagar_tabela nome_tabela\n");
		printf("\nOBS: o nome da tabela nao pode conter espacos.\n\n");
	}else if(strcmp(comando, "ordenar_cres") == 0){
		printf("Comando ordenar_cres:\n\n");
		printf("Sintaxe: ordenar_cres nome_tabela nome_campo\n");
		printf("\nOBS: o nome da tabela e do campo nao pode conter espacos, ordena a tabela de forma crescente.\n\n");
	}else if(strcmp(comando, "ordenar_des") == 0){
		printf("Comando ordenar_des:\n\n");
		printf("Sintaxe: ordenar_des nome_tabela nome_campo\n");
		printf("\nOBS: o nome da tabela e do campo nao pode conter espacos, ordena a tabela de forma descrescente.\n\n");
	}else if(strcmp(comando, "inserir_coluna") == 0){
		printf("Comando inserir_coluna\n\n");
		printf("Sintaxe: inserir_coluna nome_tabela tipo nome_novoCampo\n");
		printf("\nOBS: o nome da tabela nao pode ter espaco, tipo deve ser um dos tipos primitivos da linguagem C (char, int, float) ou string, ");
		printf("o tipo e o nome do novo campo devem ser separando por espaco. O novo campo nao pode ser defenido como chave primaria(uso do caracter '*').\n\n");
	}else if(strcmp(comando, "copiar_tabela") == 0){
		printf("Comando copiar_tabela\n\n");
		printf("Sintaxe: copiar_tabela nome_tabela novo_nome\n");
		printf("\nOBS: nome_tabela ou novo_nome nao pode conter espacos\n");
	}else if(strcmp(comando, "apagar_coluna") == 0){
		printf("Comando apagar_coluna\n\n");
		printf("Sintaxe: apagar_coluna nome_tabela nome_campo\n");
		printf("\nOBS: nome_campo nao pode ser o campo referente a chave primaria\n");
	}else if(strcmp(comando, "editar_linha") == 0){
		printf("Comando editar_linha\n\n");
		printf("Sintaxe: editar_linha nome_tabela nome_campo chave_primaria\n");
		printf("\nOBS: o campo da chave primaria nao pode ser editado, e chave_primaria e nome_campo devem existir\n");
	}else{
		printf("Comando inexistente\n");
	}
}
//funcao inserir_coluna
//recebe o nome da tabela e o novo campo
//retorna 1 se a coluna foi criada com sucesso, 0 em caso de erro

int inserir_coluna(char *nome_tabela, char *campo){
	FILE *tab, *tab_setup;
	char **tipos, **tabela, **entrada_usuario, tipo[16], nome_campo[60], tabela_setup[60], valores[100];
	int colunas = 0, linhas = 0, espaco = 0, aux = 0, conta_colunas = 0, conta = 0, ponto = 0, tem_espaco = 0, conta_virgula = 0;

	//separando o tipo e o nome do campo nas strigs tipo e nome_campo
	sscanf(campo, "%s %s", tipo, nome_campo);
	if(tipo_campo(campo)){
		//verifica se o nome do novo campo esta definido como chave primaria
		//se sim, retorna 0 e encerra a funcao
		for(int i = 0;i < strlen(nome_campo);i++){
			if(nome_campo[i] == '*'){
				printf("O novo campo esta definido como chave primaria!\n");
				return 0;
			}
		}
		tabela = carrega_tabela(nome_tabela);
		//abrindo os canais de comunicacao com os arquivos tabela e tabela.setup
		strcpy(tabela_setup, nome_tabela);
		strcat(tabela_setup, ".setup");

		tab_setup = fopen(tabela_setup, "r+");
		if(tab_setup == NULL){
			fprintf(stderr, "Erro na abertura do arquivo!");
		}else{
			fscanf(tab_setup, "%d %d\n", &colunas, &linhas);
			tipos = (char**) malloc((colunas+1)*sizeof(char*));
			if(tipos == NULL){
				fprintf(stderr, "Erro na memoria");
			}else{
				for(int i = 0;i <= colunas;i++){
					tipos[i] = (char*) malloc(1*sizeof(tipo));
					if(i != colunas){
						fscanf(tab_setup, "%s ", tipos[i]);
					}
				}
			}
		}
		strcpy(tipos[colunas], tipo);
		//verifica se a quantidade de valores passado e igual a de campos da tabela
		if(linhas > 1){
			//alocando a memoria para guadar os valores da nova coluna
			entrada_usuario = (char**) malloc((linhas-1)*sizeof(char*));
			if(entrada_usuario == NULL){
				fprintf(stderr, "Erro na memoria\n");
				return 0;
			}else{
				for(int i = 0;i < linhas-1;i++){
					entrada_usuario[i] = malloc(1*sizeof(tabela_setup));
				}
			}

			printf("Insira os valores da nova coluna:\n");
			fscanf(stdin, "%[^\n]", valores);
			setbuf(stdin, NULL);
			//checa se a quantidade de valore passado e a mesma de linhas da tabela e se os valores estao corretos
			for(int i = 0;i < strlen(valores);i++){
				if(valores[i] == ' '){
					tem_espaco++;
					break;
				}
				if(valores[i] == ','){
					conta_virgula++;
				}
			}
			conta = checa_valor(valores, entrada_usuario, tipos, colunas);
			if(conta != linhas-1 || tem_espaco != 0){
				if(tem_espaco != 0){
					printf("Erro ao inserir os valores da nova coluna!\n");
					printf("Erro: o %dº valor contem espaco!\n", conta_virgula+1);
					printf("Dica: use '_' ao inves de espaco.\n");
				}
				printf("Erro: valores invalidos!\n");
				for(int i = 0;i < colunas;i++){
					free(tipos[i]);
				}
				free(tipos);
				for(int i = 0;i < (colunas-1)*linhas;i++){
					free(tabela[i]);
				}
				free(tabela);		
				for(int i = 0;i < linhas-1;i++){
					free(entrada_usuario[i]);
				}
				free(entrada_usuario);		
				fclose(tab_setup);
				return 0;
			}

			if(strcmp(tipo, "float") == 0){
				for(int i = 0;i < linhas-1;i++){
					for(int j = 0;j < strlen(entrada_usuario[i]);j++){
						if(entrada_usuario[i][j] == '.'){
							ponto = 1;
							break;
						}
					}
					if(ponto == 0){
						strcat(entrada_usuario[i], ".0");
					}
				}
			}
		}
		tab = fopen(nome_tabela, "w");
		//salvando a tabela com o novo campo
		if(linhas > 1){
			aux = 0;
			for(int i = 0;i < colunas*linhas;i++){
				if(i == colunas){
					fprintf(tab, "%s\n", nome_campo);
					conta_colunas = 1;
				}
				fprintf(tab, "%s ", tabela[i]);
				if(conta_colunas == colunas){
					fprintf(tab, "%s\n", entrada_usuario[aux]);
					conta_colunas = 0;
					aux++;
				}
				conta_colunas++;
			}
		}else{
			for(int i = 0;i < colunas;i++){
				fprintf(tab, "%s ", tabela[i]);
				if(i == colunas-1){
					fprintf(tab, "%s\n", nome_campo);
				}
			}
		}
		colunas++;
		//salvando a nova quantidade de colunas e o tipo no arquivo .setup da tabela
		fseek(tab_setup, 0, SEEK_SET);
		fprintf(tab_setup, "%d %d\n", colunas, linhas);
		for(int i = 0;i < colunas;i++){
			fprintf(tab_setup, "%s ", tipos[i]);
		}
		//fechando os canais de comunicacao e liberando a memoria
			//memoria
			for(int i = 0;i < colunas;i++){
				free(tipos[i]);
			}
			free(tipos);
			for(int i = 0;i < (colunas-1)*linhas;i++){
				free(tabela[i]);
			}
			free(tabela);		
			if(linhas > 1){
				for(int i = 0;i < linhas-1;i++){
					free(entrada_usuario[i]);
				}
				free(entrada_usuario);
			}			
			//streams
			fclose(tab_setup);
			fclose(tab);
	}else{
		printf("O tipo '%s' nao e valido, use os dos tipos primitivos do C (char, int, float) ou string.\n", tipo);
		return 0;
	}
	return 1;
}

int copiar_tabela(char *str_tabela_1, char *str_tabela_2){
	if(strcmp(str_tabela_1, str_tabela_2) == 0){
		//mudar essa parte
		/*abre a lista de tabelas, checa quantas vezes o str_tabela_2
		esta presente, renomeia por str_tabela_2(n), onde n eh o 
		numero de vezes que o ngc apareceu, somado mais um*/
		printf("Erro: novo nome invalido\n");
		return 1;
	}else{
		//falta adicionar na lista de tabelas a tabela copia
		char str_setup_1[60], str_setup_2[60];
		FILE *tabela_1, *setup_1, *tabela_2, *setup_2, *lista;
		strcpy(str_setup_1, str_tabela_1);
		strcat(str_setup_1, ".setup");
		strcpy(str_setup_2, str_tabela_2);
		strcat(str_setup_2, ".setup");
		tabela_1 = fopen(str_tabela_1, "r");
		tabela_2 = fopen(str_tabela_2, "w");
		setup_1 = fopen(str_setup_1, "r");
		setup_2 = fopen(str_setup_2, "w");
		lista = fopen("lista_tabelas", "a+");
		//printf("chegou aqui\n");
		//checa se ocorreu erro na abertura
		if(tabela_1 == NULL || tabela_2 == NULL || setup_1 == NULL || setup_2 == NULL || lista == NULL){
			printf("Erro: nao foi possivel realizar a abertura do arquivo\n");
			fclose(tabela_1);
			fclose(tabela_2);
			fclose(setup_1);
			fclose(setup_2);
			fclose(lista);
			remove(str_tabela_2);
			remove(str_setup_2);
			return (-1);
		}else{
			char **linha;
			int aux, n_linhas, n_colunas;
			//pega o valor do numero de linhas e colunas no setup original
			fscanf(setup_1, "%d %d", &n_colunas, &n_linhas);
			//imediatamente joga no arquivo 2
			fprintf(setup_2, "%d %d\n",n_colunas, n_linhas );
			//alocando memoria
			linha = (char**)calloc(n_colunas, sizeof(char*));
			for (int i = 0; i < n_colunas; ++i){
				*(linha+i) = (char*)calloc(60, sizeof(char));
			}
			//memoria ja alocada, comeca a copiar os tipos do campo na setup
			for (int i = 0; i < n_colunas; ++i){
				fscanf(setup_1, "%s", *(linha+i));
				fprintf(setup_2, "%s ", *(linha+i));
			}
			//comeca a copiar e colar os valores e campos da tabela 1 na 2
			do{	
				for(int i = 0; i < n_colunas; ++i){
					aux = fscanf(tabela_1, "%s", *(linha+i));	
					if(aux != EOF){
						fprintf(tabela_2, "%s ", *(linha+i));
					}
				}
				if(aux != EOF){
					fprintf(tabela_2, "\n");
				}
			}while(aux != EOF);
			//liberando memoria
			for(int i = 0; i < n_colunas; ++i){
				free(*(linha+i));
			}
			free(linha);
			fprintf(lista, "%s\n",str_tabela_2);
			//fecha canais
			fclose(tabela_1);
			fclose(tabela_2);
			fclose(setup_1);
			fclose(setup_2);
			fclose(lista);
			return 0;
		}
	}
}

int apagar_coluna(char *str_nome, char *str_campo){
	//setando os nomes
	int aux = 1;
	char str_setup[120], str_tabela_nova[120], str_setup_novo[120];
	strcpy(str_setup, str_nome);
	strcpy(str_tabela_nova, str_nome);
	strcat(str_tabela_nova, "_nova");
	strcat(str_setup, ".setup");
	strcpy(str_setup_novo, str_setup);
	strcat(str_setup_novo, "_novo");
	//abrindo canais
	FILE *tabela, *setup, *tabela_nova, *setup_novo;
	tabela = fopen(str_nome, "r");
	setup = fopen(str_setup, "r");
	tabela_nova = fopen(str_tabela_nova, "w");
	setup_novo = fopen(str_setup_novo, "w");
	//checa se o campo inserido eh a pk
	if (strchr(str_campo, '*') != NULL){
		printf("Erro: coluna pk nao pode ser removida\n");
		fclose(tabela);
		fclose(setup);
		fclose(tabela_nova);
		remove(str_tabela_nova);
		return 1;
	}
	if(tabela == NULL || setup == NULL || tabela_nova == NULL || setup_novo == NULL){
		printf("Erro: falha na abertura do arquivo\n");
		fclose(tabela);
		fclose(setup);
		fclose(tabela_nova);
		fclose(setup_novo);
		remove(str_setup_novo);
		remove(str_tabela_nova);
		return (-1);
	}else{
		int n_col, n_lin, pos_campo = (-1);
		char **linha;
		//pega numero de linhas e tabelas
		fscanf(setup, "%d %d\n", &n_col, &n_lin);
		//alocando memoria
		linha = (char**)calloc(60, sizeof(char*));
		for(int i = 0; i < n_col; ++i){
			*(linha+i) = (char*)calloc(60, sizeof(char));
		}
		//pegando primeira linha para checar posicao do campo
		for(int i = 0; i < n_col; ++i){
			fscanf(tabela, "%s", *(linha+i));
			if (strcmp(str_campo, *(linha+i)) == 0){
				pos_campo = i;
			}
		}
		if(pos_campo != (-1)){
			do{
				//checa se eh diferente de -1 e imprime no arquivo novo
				if(aux != EOF){
					for(int i = 0; i < n_col; ++i){
						if(i != pos_campo){
							fprintf(tabela_nova, "%s ", *(linha+i));
						}
					}
					fprintf(tabela_nova, "\n");
				}
				//le a prox linha pra imprimir novamente
				for(int i = 0; i < n_col; ++i){
					aux = fscanf(tabela, "%s", *(linha+i));
				}
			}while(aux != EOF);
			//copiando informacoes para o novo setup
			fprintf(setup_novo, "%d %d\n", n_col-1, n_lin);
			for(int i = 0; i < n_col; ++i){
				fscanf(setup, "%s", *(linha+i));
				if(i != pos_campo){
					fprintf(setup_novo, "%s ", *(linha+i));
				}
			}
			aux = 0;
		}else{
			printf("Erro: campo inserido nao existe\n");
			aux = 1;
		}
		//liberando memoria
		for(int i = 0; i < n_col; ++i){
			free(*(linha+i));
		}
		free(linha);
	}
	//fechando canais
	fclose(tabela);
	fclose(setup);
	fclose(tabela_nova);
	fclose(setup_novo);
	if(aux == 0){
		remove(str_nome);
		rename(str_tabela_nova, str_nome);
		remove(str_setup);
		rename(str_setup_novo, str_setup);
		return 0;
	}else{
		remove(str_tabela_nova);
		return 1;
	}
}

//editar_linha presisa checar se a tabela existe e a pk existe
int editar_linha(char *str_nome, char *str_campo, char *str_pk){
	char str_tabela_nova[60], str_setup[60];
	//nomenado a nova tabela e a setup
	strcpy(str_tabela_nova, str_nome);
	strcpy(str_setup, str_nome);
	strcat(str_tabela_nova, ".nova");
	strcat(str_setup, ".setup");
	//declaranado e abrindo canais
	FILE *tabela, *tabela_nova, *setup;
	tabela = fopen(str_nome, "r");
	tabela_nova = fopen(str_tabela_nova, "w");
	setup = fopen(str_setup, "r+");
	//checa se deu td certo no canal
	if(tabela == NULL || tabela_nova == NULL || setup == NULL){
		printf("erro na abertura do arquivo\n");
		fclose(tabela);
		fclose(setup);
		fclose(tabela_nova);
		return (-1);
	}else{
		int n_col, n_lin, aux, pos_campo =  (-1), pos_pk;
		char **linha, tipo_campo[16], valor_novo[60];
		//recebe valores
		fscanf(setup, "%d %d\n", &n_col, &n_lin);
		//alocando memoria
		linha = (char**)calloc(n_col, sizeof(char*));
		for (int i = 0; i < n_col; ++i){
			*(linha+i) = (char*)calloc(60, sizeof(char));
		}
		//recebendo posicao do campo e da pk
		for(int i = 0; i < n_col; ++i){
			fscanf(tabela, "%s", *(linha+i));
			fprintf(tabela_nova, "%s ", *(linha+i));
			if(strchr(*(linha+i), '*') != NULL){
				pos_pk = i;
			}
			if(strcmp(*(linha+i), str_campo) == 0){
				pos_campo = i;
				for(int i2 = 0; i2 < n_col; ++i2){
					fscanf(setup, "%s", *(linha+i2));
				}
				strcpy(tipo_campo, *(linha+pos_campo));
			}
		}
		fprintf(tabela_nova, "\n");
		//checa as condicoes para editar
		if(pos_campo != (-1) && pos_campo != pos_pk){
			do{
				//pega a linha
				for(int i = 0; i < n_col; ++i){
					aux = fscanf(tabela, "%s", *(linha+i));
				}
				//checa se chegou na pk da linha
				if(strcmp(*(linha+pos_pk), str_pk) == 0 && aux != (-1)){
					printf("entre novo valor(%s) >>> ", tipo_campo);
					scanf("%s", valor_novo);
					setbuf(stdin, NULL);
					//checa se o valor inserido eh do tipo do campo
					if(checar_tipo_valor(tipo_campo, valor_novo) == 0){
						//se sim, o algoritmo substitui na posicao do campo
						strcpy(*(linha+pos_campo), valor_novo);
					}else{
						//se nao, o valor nao eh alterado mas eh passado para a tabela nova
						printf("Erro: valor inserido (%s) nao e do tipo %s\n", valor_novo, tipo_campo);
					}
					//passando valores para tabela nova
					for(int i = 0; i < n_col; ++i){
						fprintf(tabela_nova, "%s ", *(linha+i));		
					}
					fprintf(tabela_nova, "\n");
				}
				else{
					if(aux != (-1)){
						for(int i = 0; i < n_col; ++i){
							fprintf(tabela_nova, "%s ", *(linha+i));		
						}
						fprintf(tabela_nova, "\n");
					}
				}
			}while(aux != EOF);
		}else{
			//dialogos erro
			if(pos_campo == pos_pk){
				printf("Erro: nao e possivel editar a pk\n");
				for(int i = 0; i < n_col; ++i){
					free(*(linha+i));
				}
				free(linha);
				fclose(tabela_nova);
				fclose(tabela);
				fclose(setup);
				remove(str_tabela_nova);
				return 1;
			}else{
				printf("Erro: campo nao exite\n");
				for(int i = 0; i < n_col; ++i){
					free(*(linha+i));
				}
				free(linha);
				fclose(tabela_nova);
				fclose(tabela);
				fclose(setup);
				remove(str_tabela_nova);
				return 1;
			}
		}
		//liberando memoria
		for(int i = 0; i < n_col; ++i){
			free(*(linha+i));
		}
		free(linha);
	}
	fclose(tabela_nova);
	fclose(tabela);
	fclose(setup);
	remove(str_nome);
	rename(str_tabela_nova, str_nome);
	return 0;
}

int checar_tipo_valor(char *str_tipo, char *str_valor){
	if(strcmp(str_tipo, "int") == 0){
		return check_int(str_valor);
	}else if(strcmp(str_tipo, "float") == 0){
		if(check_float(str_valor)==0){
			if(check_int(str_valor) == 0){
				strcat(str_valor, ".0");
			}
		}
		return check_float(str_valor);	
	}else if(strcmp(str_tipo, "char") == 0){
		return check_char(str_valor);
	}else if(strcmp(str_tipo, "string") == 0){
		return 0;
	}else{
		return 1;
	}
}
