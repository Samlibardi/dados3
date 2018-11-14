#include <stdlib.h>
#include <stdio.h>
#include "registro.h"

#define ERROR_FOPEN 1
#define ERROR_STATUS 2
#define ERROR_EMPTY 3

FILE* fdados;

int status;
registro curRecord;

//função de listagem de um arquivo
int listaDados(char* filename) {
	//tenta abrir o arquivo e confere o status
	if (!(fdados = fopen(filename, "rb"))) {
		return ERROR_FOPEN;
	}
	fread(&status, 4, 1, fdados);
	if (!status) return ERROR_STATUS;

	//loop de escrita. lê registro por registro e printa no formato especificado
	while(fread(&curRecord, sizeof(registro), 1, fdados)) {
		printf("%-5d %-*.*s %-*.*s %-*.*s\n", curRecord.campo1, LEN_FIELD2, LEN_FIELD2, curRecord.campo2, LEN_FIELD3, LEN_FIELD3, curRecord.campo3, LEN_FIELD4, LEN_FIELD4, curRecord.campo4);
	}

	//fecha o arquivo
	fclose(fdados);
	return 0;
}