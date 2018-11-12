#include <stdlib.h>
#include <stdio.h>
#include "registro.h"

#define ERROR_FOPEN 1
#define ERROR_STATUS 2
#define ERROR_EMPTY 3

#define STR_(X) #X
#define STR(X) STR_(X)

FILE* fdados;

int status;
registro curRecord;

int listaDados(char* filename) {
	if (!(fdados = fopen(filename, "rb"))) {
		return ERROR_FOPEN;
	}

	fread(&status, 4, 1, fdados);

	if (feof(fread)) {
		return ERROR_EMPTY;
	}
	else if (!status) {
		return ERROR_STATUS;
	}

	while(fread(&curRecord, sizeof(registro), 1, fdados)) {
		printf("%-5d %-*.*s %-*.*s %-*.*s\n", curRecord.campo1, LEN_FIELD2, LEN_FIELD2, curRecord.campo2, LEN_FIELD3, LEN_FIELD3, curRecord.campo3, LEN_FIELD4, LEN_FIELD4, curRecord.campo4);
	}

	fclose(fdados);
	return 0;
}