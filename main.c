#include <stdlib.h>
#include <stdio.h>
#include "geraDados.h"

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

	return 0;
}