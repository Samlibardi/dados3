//*******************************************************
// Trabalho Prático de Estruturas de Dados III          *
// Grupo: 17                                            *
// Samuel Libardi Godoy                 nºUSP:          *
// Luis Eduardo Prado Santini           nºUSP: 9065750  *
// Rafael Guilherme da Silva Tiburcio   nºUSP:          *
//*******************************************************
#include <stdlib.h>
#include <stdio.h>
#include "registro.h"
#include "geraDados.h"
#include "listaDados.h"
#include "ordenaInterna.h"
#include "merge.h"
#include "split.h"
#include "math.h"
#include "ordenaExterna.h"
#include "match.h"

#define ERROR_ARG 1

int err = -1;

int main(int argc, char** argv) {
	if (argc < 2) return ERROR_ARG;

	// Processar parametros de entrada do programa


	// Funcionalidade 1
	if (!strcmp(argv[1],"1")) {
		if (argc < 4) return ERROR_ARG;
		if (err = !genDataFile(atoi(argv[3]), argv[2])) {
			printf("Arquivo gerado.\n");
			return 0;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

	// Funcionalidade 2
	else if (!strcmp(argv[1], "2")) {
		if (argc < 3) return ERROR_ARG;
		if (err = !listaDados(argv[2])) {
			return 0;
		}
		else if(err == 3){
			printf("Arquivo vazio.\n");
			return err;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

	// Funcionalidade 3
	else if (!strcmp(argv[1], "3")) {
		if (argc < 4) return ERROR_ARG;
		if (err = !ordenaInterna(argv[2],argv[3])) {
			printf("Arquivo gerado.\n");
			return 0;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

    // Funcionalidade 4
	else if (!strcmp(argv[1], "4")) {
		if (argc < 5) return ERROR_ARG;
		if (err = !merge(2, argv + 2, argv[4])) {
			printf("Arquivo gerado.\n");
			return 0;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

	// Funcionalidade 5
	else if (!strcmp(argv[1], "5")) {
		if (argc < 5) return ERROR_ARG;
		if (err = !match(2, argv + 2, argv[4])) {
			//Ordena o arquivo de match usando a ordenação externa da funcionalidade 7
			if (err = !ordenaExterna(argv[4], argv[4])) {
				printf("Arquivo gerado.\n");
				return 0;
			}
			return err;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

    // Funcionalidade 6
	else if (!strcmp(argv[1], "6")) {
		if (argc < 5) return ERROR_ARG;
		if (err = !merge(argc-3, argv + 2, argv[argc-1])) {
			printf("Arquivo gerado.\n");
			return 0;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}
    // Funcionalidade 7
	else if (!strcmp(argv[1], "7")) {
		if (argc < 4) return ERROR_ARG;

		if (err = !ordenaExterna(argv[2], argv[3])) {
			printf("Arquivo gerado.\n");
			return 0;
		}
		else {
			printf("Falha no processamento.\n");
			return err;
		}
	}

	return 0;
}
