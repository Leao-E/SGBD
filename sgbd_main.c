#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes_aux.h"
#include "criar_inserir.h"
#include "apagar.h"
#include "exibicao.h"
#include "busca.h"
#include "funcoes_extras.h"

typedef struct command{
	char comando[60];
	char nome_tabela[100];
	char campos[180];
}COMMAND;

//void waitFor (unsigned int secs);
void limpa_comando(COMMAND *aux);
void exibir_menu();

int main(){
	char comando[300], pk[60], aux_edit[60];
	COMMAND aux;
	limpa_comando(&aux);
	strcpy(pk, "NULL");
	strcpy(aux_edit, "NULL");

	/*
	system("cls");
	for(int i = 0; i < 2; ++i){
		exibir_menu();
		printf("\n\n");
		printf("O o o o o\n");
		waitFor(1);
		system("cls");
		exibir_menu();
		printf("\n\n");
		printf("o O o o o\n");
		waitFor(1);
		system("cls");
		exibir_menu();
		printf("\n\n");
		printf("o o O o o\n");
		waitFor(1);
		system("cls");
		exibir_menu();
		printf("\n\n");
		printf("o o o O o\n");
		waitFor(1);
		system("cls");
		exibir_menu();
		printf("\n\n");
		printf("o o o o O\n");
		waitFor(1);
		system("cls");
	}
	exibir_menu();
	*/

	system("clear");
	exibir_menu();
	do{
		printf("Digite o comando >>> ");
		fscanf(stdin, "%[^\n]",comando);
		setbuf(stdin, NULL);
		sscanf(comando, "%s %s %[^\n]", aux.comando, aux.nome_tabela, aux.campos);

		if(strcmp(aux.nome_tabela, "NULL") != 0){
			if(strcmp(aux.comando, "criar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					criar_tabela(aux.nome_tabela, aux.campos);
					limpa_comando(&aux);
				}else{
					printf("Tabela %s ja existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}
			}else if(strcmp(aux.comando, "inserir_linha") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(strcmp(aux.campos, "NULL") == 0){
						mostrar_campos_tipos(aux.nome_tabela);
						printf("Insira os valores da nova linha:\n");
						fscanf(stdin, "%[^\n]", aux.campos);
						setbuf(stdin, NULL);
						inserir_linha(aux.nome_tabela, aux.campos);
						mostrar_tabela(aux.nome_tabela);
						limpa_comando(&aux);
					}else{
						inserir_linha(aux.nome_tabela, aux.campos);
						mostrar_tabela(aux.nome_tabela);
						limpa_comando(&aux);
					}
				}
			}else if(strcmp(aux.comando, "mostrar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					mostrar_tabela(aux.nome_tabela);
					limpa_comando(&aux);
				}
			}else if(strcmp(aux.comando, "apagar_linha") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(apagar_linha (aux.nome_tabela, aux.campos) == 0){
						mostrar_tabela(aux.nome_tabela);
						limpa_comando(&aux);
						printf("Linha apagada com sucesso!\n");
					}else{
						printf("Chave primaria invalida\n");
					}
				}
			}else if(strcmp(aux.comando, "apagar_tabela") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(apagar_tabela("lista_tabelas", aux.nome_tabela) == 0){
						limpa_comando(&aux);
						printf("Tabela apagada com sucesso!\n");
					}
				}
			}else if(strcmp(aux.comando, "ordenar_cres") == 0 || strcmp(aux.comando, "ordenar_des") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					if(strcmp(aux.comando, "ordenar_cres") == 0){
						if(ordenar(aux.nome_tabela, aux.campos, 0)){
							limpa_comando(&aux);
						}else{
							printf("O campo escolhido nao e valido!\n");
							limpa_comando(&aux);
						}
					}else{
						if(ordenar(aux.nome_tabela, aux.campos, 1)){
							limpa_comando(&aux);
						}else{
							printf("O campo escolhido nao e valido!\n");
							limpa_comando(&aux);
						}
					}
				}
			}else if(strcmp(aux.comando, "inserir_coluna") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{
					sscanf(aux.campos, "%s %s", pk, aux_edit);
					if(strcmp(aux_edit, "NULL") != 0){
						if(inserir_coluna(aux.nome_tabela, aux.campos)){
							mostrar_tabela(aux.nome_tabela);
							printf("Coluna criada com sucesso!\n");
							limpa_comando(&aux);
						}else{
							printf("Erro de sintaxe!\n");
							mostrar_sintaxe_correta(aux.comando);
							limpa_comando(&aux);
						}
					}else{
						printf("Erro: nome da nova coluna nao inserido!\n");
					}
				}
			}else if(strcmp(aux.comando, "copiar_tabela") == 0){
				if(strcmp(aux.nome_tabela, "NULL") == 0 || strcmp(aux.campos, "NULL") == 0){
					printf("Erro de sintaxe!\n");
					mostrar_sintaxe_correta(aux.comando);
				}else{
					if(check_tabela_existe(aux.nome_tabela)){
						printf("Tabela %s nao existe\n", aux.nome_tabela);
						limpa_comando(&aux);
					}else{
						copiar_tabela(aux.nome_tabela, aux.campos);
						limpa_comando(&aux);
					}
				}
			}else if(strcmp(aux.comando, "apagar_coluna") == 0){
				if(strcmp(aux.nome_tabela, "NULL") == 0 || strcmp(aux.campos, "NULL") == 0){
					printf("Erro de sintaxe!\n");
					mostrar_sintaxe_correta(aux.comando);
				}else{
					if(check_tabela_existe(aux.nome_tabela)){
						printf("Tabela %s nao existe\n", aux.nome_tabela);
						limpa_comando(&aux);
					}else{
						if(apagar_coluna(aux.nome_tabela, aux.campos) == 0){
							mostrar_tabela(aux.nome_tabela);
							printf("coluna apagada com sucesso\n");
						}
						limpa_comando(&aux);
					}
				}
			}else if(strcmp(aux.comando, "editar_linha") == 0){
				if(sscanf(aux.campos, "%s %s", aux_edit, pk) != EOF){
					if(check_tabela_existe(aux.nome_tabela) == 0){
						if(checa_pk_existe(aux.nome_tabela, pk) == 0){
					 		editar_linha(aux.nome_tabela, aux_edit, pk);
					 		mostrar_tabela(aux.nome_tabela);
					 	}else{
					 		printf("Chave primaria invalida\n");
					 		mostrar_sintaxe_correta(aux.comando);
					 	}
					}else{
						printf("Tabela %s nao existe\n", aux.nome_tabela);
					}
				}else{
					printf("Erro de sintaxe\n");
					mostrar_sintaxe_correta(aux.comando);
				}
			}else if(strcmp(aux.comando, "buscar_valor") == 0){
				if(check_tabela_existe(aux.nome_tabela)){
					printf("Tabela %s nao existe\n", aux.nome_tabela);
					limpa_comando(&aux);
				}else{ 
					if(strcmp(aux.campos, "NULL") != 0){
						main_busca(aux.nome_tabela, aux.campos);
						limpa_comando(&aux);
					}else{
						printf("Erro: campo invalido\n");
					}
				}
			}else if(strcmp(aux.comando, "ajuda") == 0){
				ajuda(aux.nome_tabela);
				limpa_comando(&aux);
			}else{
				printf("Erro: comando invalido!\n");
			}
		}else{
			if(strcmp(aux.comando, "ajuda") == 0){
				printf("Sintaxe: ajuda comando\n");
				limpa_comando(&aux);
			}else if(strcmp(aux.comando, "listar_tabelas") == 0){
				listar_tabelas("lista_tabelas");
				limpa_comando(&aux);
			}else if(strcmp(aux.comando, "sair") == 0){
				printf("programa encerrado\n");
			}else if(strcmp(aux.comando, "limpar_tela") == 0){
				system("clear");
				limpa_comando(&aux);
				exibir_menu();
			}else{
				printf("Erro: comando invalido!\n");
				mostrar_sintaxe_correta(aux.comando);
				limpa_comando(&aux);
			}
		}

	}while(strcmp(aux.comando, "sair") != 0);

	return 0;
}
//atribui a string "NULL" para todos os campos da struct do tipo COMMAND
//recebe uma struct do tipo COMMAND
void limpa_comando(COMMAND *aux){
	strcpy(aux->comando, "NULL");
	strcpy(aux->nome_tabela, "NULL");
	strcpy(aux->campos, "NULL");
}
//exibe o menu
void exibir_menu (){
	printf("            SGBD\n=============================\n");
	printf(" 1 - criar_tabela\n");
	printf(" 2 - listar_tabelas\n");
	printf(" 3 - inserir_linha\n");
	printf(" 4 - mostrar_tabela\n");
	printf(" 5 - buscar_valor\n");
	printf(" 6 - apagar_linha\n");
	printf(" 7 - apagar_tabela\n");
	printf(" 8 - Extras\n    1 - ordenar_cres\n    2 - ordenar_des\n    3 - ajuda\n    4 - inserir_coluna\n    5 - apagar_coluna\n    6 - editar_linha\n    7 - copiar_tabela\n");
	printf(" 9 - limpar_tela\n");
	printf("10 - sair\n");
	printf("=============================\n");
}

/*
void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}
*/
