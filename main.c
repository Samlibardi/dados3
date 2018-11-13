#include <stdlib.h>
#include <stdio.h>
#include "registro.h"
#include "geraDados.h"
#include "listaDados.h"
#include "ordenaInterna.h"

#define ERROR_ARG 1

int err;

int main(int argc, char** argv) {
	if (argc < 2) return ERROR_ARG;

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

	return 0;
}