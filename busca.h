int diferenca_strings (char *str_1, char *str_2);
int busca_aprox_string (char *str_orig, char *str_comp, float precisao);
int busca_aprox_float (double n_ref, double n_comp, float precisao);
int busca_aprox_int (long n_ref, long n_comp, float precisao);
int buscar_valores (char *str_nome, char *str_campo, char *str_valor, int modo);
int main_busca (char *str_nome, char *str_campo);

/*recebe duas strings e retorna a menor diferenca possivel.
O alg vai "encaixando" a menor delas dentro da maior e
comparando a quantidade de letras distintas, o menor valor
eh retornado*/
int diferenca_strings (char *str_1, char *str_2){
	int cont_1 = 0, cont_2 = 0, aux = 0, menor_dif = 0;
	char str_menor[120], str_maior[120];
	/*checando a menor string para definir o contador (cont_1)
	o contador recebe o numero de possibilidades de encaixar
	a menor string na menor, a partir do ponto 0
	O segundo contador recebe o strlen da menor string*/
	if (strlen(str_1) <= strlen(str_2)){
		cont_1 = (strlen(str_2)-strlen(str_1)+1);
		cont_2 = strlen(str_1);
		/*passando a str_1 e a str_2 para as strings auxiliares, no momento com buffer de 120
		em breve mudar para alocacao dinamica*/
		strcpy(str_menor, str_1);
		strcpy(str_maior, str_2);
	}else{
		cont_1 = (strlen(str_1)-strlen(str_2)+1);
		cont_2 = strlen(str_2);
		/*passando a str_1 e a str_2 para as strings auxiliares, no momento com buffer de 120
		em breve mudar para alocacao dinamica*/
		strcpy(str_menor, str_2);
		strcpy(str_maior, str_1);
	}
	/*checando se ha letras maiusculas para deixar todas minusculas, melhora o uso para a funcao
	achar valores proximos (strings), se for menos restritiva, ou seja, se nao for case sensitive
	eh mais provavel de achar strings similares*/
	for (int i = 0; i < strlen(str_menor); ++i){
		/*checa se eh maiuscula usando tabela ASCII, se sim transforma em minuscula*/
		if ((str_menor[i] <= 90) && (str_menor[i] >= 65)){
			/*diferenca na ASCII eh 32*/
			str_menor[i] += 32;
		}
	}
	/*faz o mesmo para a proxima string*/
	for (int i = 0; i < strlen(str_maior); ++i){
		/*checa se eh maiuscula usando tabela ASCII, se sim transforma em minuscula*/
		if ((str_maior[i] <= 90) && (str_maior[i] >= 65)){
			/*diferenca na ASCII eh 32*/
			str_maior[i] += 32;
		}
	}
	/*usando as variaveis cont para checar qual a menor numero de 
	caracteres distintos*/
	for(int i = 0; i < cont_1; ++i){
		aux = 0;
		for(int i2 = 0; i2 < cont_2; ++i2){
			if(str_menor[i2] != str_maior[i2+i]){
				aux++;
			}
		}
		/*checa se esta no primeiro ciclo do loop e executa a comparacao entre
		o numero de caracteres distintos entre as strings*/
		if (i != 0){
			if(aux <= menor_dif){
				menor_dif = aux;
			}
		}else{
			menor_dif = aux;
		}
	}
	return menor_dif;
}

/*utiliza a funcao de difereca para checar se a diferenca entre duas strings
satisfaz a condicao de precidao imposta*/
int busca_aprox_string (char *str_orig, char *str_comp, float precisao){
	float passe, fator = 1-(precisao/100);
	if (strlen(str_orig) <= strlen(str_comp)){
		passe = strlen(str_orig)*fator;
	}else{
		passe = strlen(str_comp)*fator;
	}
	if (diferenca_strings(str_orig, str_comp) <= passe){
		//printf("%s\n", str_comp);
		return 1;
	}
	return 0;
}

/*similar a funcao acima, essa faz basicamente a mesma coisa, só que
a diferenca eh entre tipos numericos e se trata do valor absoluto*/
int busca_aprox_float (double n_ref, double n_comp, float precisao){
	float passe, fator = 1-(precisao/100);
	passe = n_ref*fator;
	if (abs(n_ref-n_comp) <= passe){
		//printf("%.2f\n", n_comp);
		return 1;
	}
	return 0;
}

/*basicamente a funcao acima para tipo long*/
int busca_aprox_int (long n_ref, long n_comp, float precisao){
	float passe, fator = 1-(precisao/100);
	passe = n_ref*fator;
	if (abs(n_ref-n_comp) <= passe){
		//printf("%ld\n", n_comp);
		return 1;
	}
	return 0;
}

/*busca valores de acordo com o modo selecionado
 a funcao tem 6 modos:
 1- maior que
 2- maior ou igual a
 3- menor que
 4- menor ou igual a
 5- igual a
 6- proximo a
 a funcao recebe o nome e o campo da string, checa se o campo existe
 e em seguida compara os valores lidos do campo/coluna com o str_valor
 de acordo com o modo selecionado*/
int buscar_valores (char *str_nome, char *str_campo, char *str_valor, int modo){
	/*canais para a tabela e o setup*/
	FILE *tabela, *setup, *result, *setup_result;
	//result sera uma tabela temporaria para salvar os resultados
	/*string aux para abrir o setup e string para salvar o tipo da variavel
	A str_linha eh alocada de acordo com o arquivo setup, ela recebe uma linha
	onde cada celula tem buffer de 60 caracteres*/
	char str_aux[60], str_tipo[16], **str_linha;
	/*variaveis para o numero de linha, coluna e coluna em que o campo se localiza
	a int aux eh usada para checar quando chega o fim do arquivo e aux_2 eh utilizada
	durante a checagem do valor inserido*/
	int n_lin, n_col, col_campo = (-1), aux, aux_2 = 0, n_lin_result = 0;
	/*atribuindo valores para str_aux*/
	strcpy(str_aux, str_nome);
	strcat(str_aux,".setup");
	/*abrindo canais*/
	tabela = fopen(str_nome, "r");
	setup = fopen(str_aux, "r");
	result = fopen("resultados", "w");
	setup_result = fopen("resultados.setup", "w");
	if(tabela == NULL || setup == NULL || result == NULL || setup_result == NULL){
		/*fechando os canais*/
		printf("erro na abertura do arquivo\n");
		fclose(tabela);
		fclose(setup);
		fclose(result);
		fclose(setup_result);
		remove("resultados");
		remove("resultados.setup");
		setbuf(stdin, NULL);
		return 1;
	}else{
		/*recebendo os valores de linhas e colunas da tabela e alocando valores para
		a variavel linha. buffer de 60*/
		fscanf(setup, "%d %d", &n_col, &n_lin);
		str_linha = (char**)calloc(n_col, sizeof(char*));
		for(int i = 0; i < n_col; ++i){
			*(str_linha+i) = (char*)calloc(60, sizeof(char));
		}
		/*checando a posicao do campo inserido
		obs: pq checar ate o final ao inves de parar ao achar
		o valor de i desejado? simples: vamos precisar ler outras linha,
		ou seja, vamos ter que comecar do inicio da proxima*/
		for(int i = 0; i < n_col; ++i){
			fscanf(tabela, "%s", *(str_linha+i));
			fprintf(result, "%s ", *(str_linha+i));
			if(strcmp(*(str_linha+i), str_campo) == 0){
				col_campo = i;
			}
		}
		fprintf(result, "\n");
		//checa se o campo existe
		if(col_campo == (-1)){
			printf("erro: campo invalido\n");
		}else{
			/*checando qual eh o tipo de variavel do campo indicado
			obs: aqui pode parar quando achar o tipo*/
			for(int i = 0; i < n_col; ++i){
				fscanf(setup, "%s", str_tipo);
				if(i == col_campo){
					break;
				}
			}
			if(strcmp("int", str_tipo) == 0){
				if(check_int(str_valor) != 0){
					printf("Erro: %s nao eh %s\n", str_valor, str_tipo);
				}else{aux_2 = 1;}
			}else if(strcmp("float", str_tipo) == 0){
				if(check_float(str_valor) != 0){
					printf("Erro: %s nao eh %s\n", str_valor, str_tipo);
				}else{aux_2 = 1;}
			}else if(strcmp("char", str_tipo) == 0){
				if(check_char(str_valor) != 0){
					printf("Erro: %s nao eh %s\n", str_valor, str_tipo);
				}else{aux_2 = 1;}
			}else if(strcmp("string", str_tipo) == 0){
				aux_2 = 1;
			}
		}
		/*checando qual o tipo descoberto para executar o respectivo algoritmo*/
		/*checando para int*/
		if((strcmp("int", str_tipo) == 0) && (aux_2 == 1)){
			/*n_ref eh o numero de referencia, n_comp eh o numero que sera compardo*/
			long n_ref, n_comp;
			float N;
			int cont = 0;
			n_ref = atol(str_valor);
			if(modo == 6){/*sempre que o modo for 6 sera solicitado
							o nivel de precisao*/
				printf("entre nivel de precisao (0 --- 100) >>> ");
				scanf("%f", &N);
				if(N<0 || N>100){
					printf("valor invalido\n");
				}
			}
			printf("Valores encontrados:\n");
			/*ate o fim do arquivo checa de acordo com o modo escolhido se os valores do
			campo satisfazem a condicao*/
			do{
				for(int i = 0; i < n_col; ++i){
					aux = fscanf(tabela, "%s", *(str_linha+i));
				}
				n_comp = atol(*(str_linha+col_campo));
				if(aux != (-1)){
					switch(modo){
						case 1:
							if(n_comp > n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 2:
							if(n_comp >= n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 3:
							if(n_comp < n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 4:
							if(n_comp <= n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 5:
							if(n_comp == n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 6:
							if(N>=0 && N<=100){
								if(busca_aprox_int(n_ref, n_comp, N) == 1){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
							}
							break;
						default:
							break;
					}
				}
			}while(aux != (-1));//-1 >>> EOF
			if (cont == 0){
				printf("nenhum valor encontrado\n");
			}else{
				fclose(result);
				fprintf(setup_result, "%d %d\n",n_col, n_lin_result+1);
				fclose(setup_result);
				mostrar_tabela("resultados");
			}
		/*checando para float*/
		}else if(strcmp("float", str_tipo) == 0 && aux_2 == 1){
			/*n_ref eh o numero de referencia, n_comp eh o numero que sera compardo*/
			double n_ref, n_comp;
			float N;
			int cont = 0;
			n_ref = atof(str_valor);
			if(modo == 6){
				printf("entre nivel de precisao (0 --- 100) >>> ");
				scanf("%f", &N);
				if(N<0 || N>100){
					printf("valor invalido\n");
				}
			}
			printf("Valores encontrados:\n");
			do{
				for(int i = 0; i < n_col; ++i){
					aux = fscanf(tabela, "%s", *(str_linha+i));
				}
				n_comp = atof(*(str_linha+col_campo));
				if(aux != (-1)){
					switch(modo){
						case 1:
							if(n_comp > n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 2:
							if(n_comp >= n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 3:
							if(n_comp < n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 4:
							if(n_comp <= n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 5:
							if(n_comp == n_ref){
								for(int i = 0; i < n_col; ++i){
									fprintf(result, "%s ", *(str_linha+i));
								}
								fprintf(result, "\n");
								n_lin_result++;
								cont++;
							}
							break;
						case 6:
							if(N>=0 && N<=100){
								if(busca_aprox_float(n_ref, n_comp, N) == 1){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
							}
							break;
						default:
							break;
					}
				}
			}while(aux != (-1));
			if (cont == 0){
				printf("nenhum valor encontrado\n");
			}else{
				fprintf(setup_result, "%d %d\n",n_col, n_lin_result+1);
				fclose(setup_result);
				fclose(result);
				mostrar_tabela("resultados");
			}
		/*caso nao seja float ou int sera string ou char, que eh uma string
		obs: BUFFER = 180*/
		}else if(aux_2 == 1){
			char str_ref[180], str_comp[180];
			float N;
			int aux_local, cont = 0;
			strcpy(str_ref, str_valor);
			abaixar_caixa(str_ref);
			if(strcmp("char", str_tipo) == 0 && modo == 6){
				printf("recursso indisponivel para char\n");
			}else{
				if(modo == 6){
					printf("entre nivel de precisao (0 --- 100) >>> ");
					scanf("%f", &N);
					if(N<0 || N>100){
						printf("valor invalido\n");
					}
				}
				printf("Valores encontrados:\n");
				do{
					for(int i = 0; i < n_col; ++i){
						aux = fscanf(tabela, "%s", *(str_linha+i));
					}
					strcpy(str_comp, *(str_linha+col_campo));
					abaixar_caixa(str_comp);
					if(aux != (-1)){
						switch(modo){
							case 1:
								if(strcmp(str_ref, str_comp) < 0){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
								break;
							case 2:
								if(strcmp(str_ref, str_comp) <= 0){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
								break;
							case 3:
								if(strcmp(str_ref, str_comp) > 0){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
								break;
							case 4:
								if(strcmp(str_ref, str_comp) >= 0){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
								break;
							case 5:
								if(strcmp(str_ref, str_comp) == 0){
									for(int i = 0; i < n_col; ++i){
										fprintf(result, "%s ", *(str_linha+i));
									}
									fprintf(result, "\n");
									n_lin_result++;
									cont++;
								}
								break;
							case 6:
								if(N>=0 && N<=100){
									if(busca_aprox_string(str_valor, *(str_linha+col_campo), N) == 1){
										for(int i = 0; i < n_col; ++i){
											fprintf(result, "%s ", *(str_linha+i));
										}
										fprintf(result, "\n");
										n_lin_result++;
										cont++;
									}
								}
								break;
							default:
								break;
						}
					}
				}while(aux != -1);
				if(cont == 0){
					printf("nenhum valor encontrado\n");
				}else{
					fprintf(setup_result, "%d %d\n",n_col, n_lin_result+1);
					fclose(setup_result);
					fclose(result);
					mostrar_tabela("resultados");
				}
			}
		}
		/*liberando memoria*/
		for(int i = 0; i < n_col; ++i){
			free(*(str_linha+i));
		}
		free(str_linha);
	}
	//fechando os canais
	fclose(tabela);
	fclose(setup);
	remove("resultados");
	remove("resultados.setup");
	setbuf(stdin, NULL);
	return 0;
}

/*essa funcao faz basicamente o gerenciamento da funcao acima, nela o
usuario digita o comando e de acordo com esse comando ela executa a 
buscar_valor, lembrando que os modos sao fixos, e a existencia do campo
eh checada na funcao buscar_valor
retorna 0 se o comando existir e 1 se nao existir*/
int main_busca (char *str_nome, char *str_campo){
	/*variaveis usadas para executar o comando*/
	char comando[60], valor_busca[60];
	/*imprimindo o menu e pedindo o comando*/
	printf("Opcoes de busca:\n	1* valores_maiores\n	2* valores_maiores_igual\n	3* valores_iguais\n	4* valores_menores\n	5* valores_menores_igual\n	6* valores_proximos\nDigite o comando >>> ");
	fscanf(stdin,"%s %s", comando, valor_busca);
	setbuf(stdin, NULL);
	/*checando o comando para ver qual eh o comando a ser executado*/
	if(strcmp("valores_maiores", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 1);
	}else if(strcmp("valores_maiores_igual", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 2);
	}else if(strcmp("valores_iguais", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 5);
	}else if(strcmp("valores_menores", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 3);
	}else if(strcmp("valores_menores_igual", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 4);
	}else if(strcmp("valores_proximos", comando) == 0){
		buscar_valores(str_nome, str_campo, valor_busca, 6);
	}else{
		printf("Comando invalido\n");
		printf("Comando valores_condicao:\n\n");
		printf("Sintaxe : valores_condicao valor_de_busca\n");
		return 1;
	}
	return 0;
}