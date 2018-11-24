#include <stdlib.h>
#include <stdio.h>
#include "registro.h"
#include "compares.h"

#define ERROR_FOPEN 1;
#define ERROR_STATUS 2;

int status;

FILE* fdest;

registro* recordsv;

int merge(const int srcc, const char** srcnamev, char* destname) {
	const FILE** srcv = malloc(sizeof(FILE*) * srcc);
	const registro* recordsv = malloc(sizeof(registro) * srcc);
	int srco = srcc;
	for (int i = 0; i < srcc; i++) {
		srcv[i] = fopen(srcnamev[i], "rb"); //Abre o arquivo de origem como leitura
		
		//Checa se o arquivo foi aberto e se o status é valido
		if (!srcv[i]) {
			free(recordsv);
			free(srcv);
			return ERROR_FOPEN;
		}
		fread(&status, 4, 1, srcv[i]);
		if (!status) {
			free(recordsv);
			free(srcv);
			return ERROR_STATUS;
		}

		//carrega o primeiro registro no vetor
		fread(recordsv + i, sizeof(registro), 1, srcv[i]);
	}

	//Abre o arquivo destino como escrita, e escreve o status 0 no cabeçalho
	fdest = fopen(destname, "wb");
	status = 0;
	fwrite(&status, sizeof(int), 1, fdest);

	while (srco) {
		int imin = -1;
		for (int i = 0; i < srcc; i++) {
			if (!srcv[i]) continue; //Ignorar se o arquivo ja foi fechado
			if (imin < 0) { imin = i; continue; }
			if (regcmp(recordsv + i, recordsv + imin) < 0) {
				imin = i;
			}
		}
		fwrite(recordsv + imin, sizeof(registro), 1, fdest);
		if (!fread(recordsv + imin, sizeof(registro), 1, srcv[imin])) {
			srcv[imin] = fclose(srcv[imin]);
			srco--;
		}
	}

	rewind(fdest);
	status = 1;
	fwrite(&status, sizeof(int), 1, fdest);
	fclose(fdest);
	free(recordsv);
	free(srcv);

	return 0;
}