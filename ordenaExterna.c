#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "asprintf.h"
#include "merge.h"

#define MAX_OPEN_FILES 256

int ordenaExterna(char* srcname, char* destname) {
	int splitc;
	int err;

	char** splitv = NULL;

	//Quebra o arquivo e ordena internamente.
	if (split(srcname, &splitv, &splitc)) {
		int destnamelen;

		char **srcsubv = splitv, **destsubv;
		int srcsubc = splitc, destsubc, totalsubs = splitc;

		//Para cada nivel, sao abertos MAX_OPEN_FILES de cada vez, e mergidos em 1.
		//No total, em cada nivel, existem srcsbubc subarquivos de origem e srcsubc/MAX_OPEN_FILES subarquivos de destino
		//Isso se repete ate que apenas 1 subarquivo destino seja escrito (arquivo final)
		do {
			//Calcula quantos os arquivos de destino e aloca espaço
			destsubc = ceilf(srcsubc / (float)MAX_OPEN_FILES);
			destsubv = malloc(sizeof(char*) * destsubc);

			//Faz o merge de um bloco de arquivos de origem.
			for (int i = 0; i < destsubc; i++, totalsubs++) {
				char* destname;
				asprintf(&destname, "sub_%d_%s", totalsubs, srcname);
				destsubv[i] = destname;
				if (err = merge((int)fmin(MAX_OPEN_FILES, srcsubc - i * MAX_OPEN_FILES), srcsubv + i * MAX_OPEN_FILES, destname))
					return err;
			}
			
			//Libera o espaço utilizado pela lista de subarquivos de origem
			for (int i = 0; i < srcsubc; i++) {
				free(srcsubv[i]);
				srcsubv[i] = NULL;
			};
			free(srcsubv);

			// A lista de subarquivos de origem é agora a lista de subarquivos destino que acabaram de ser escritos
			srcsubv = destsubv;
			srcsubc = destsubc;
			
			// Repete essa operação até que se escreva apenas o arquivo final
		} while (destsubc > 1);

		// Renomeia o ultimo subarquivo gerado pelo nome de destino
		remove(destname);
		rename(destsubv[0], destname);

		return 0;
	}
}