#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registro.h"
#include "compares.h"

//TODO: verificar se o split está de acordo com as especificações.
#define SPLIT_SIZE 64000									// Tamanho de cada subarquivo
#define SPLIT_REC_COUNT (SPLIT_SIZE - 4)/sizeof(registro)	// Numero de registros em um subarquivo

int status;

int split(char* filename, char*** sub_filenamesv, int* sub_filenamesc) {
	int subc = 0, readc, destnamelen;

	//Abre o arquivo origem e verifica o cabeçalho
	FILE* fsrc = fopen(filename, "rb"), *fdest;

	fread(&status, 4, 1, fsrc);

	//Buffer para o conteudo de um subarquivo, completo
	void* fbuffer = malloc(SPLIT_SIZE);

	*sub_filenamesv = NULL;

	//enquanto for possivel ler registros do arquivo original, le, ordena internamente e escreve o subarquivo
	while (readc = fread(fbuffer, sizeof(registro), SPLIT_REC_COUNT, fsrc)) {
		//Ordena o buffer por quicksort
		qsort(fbuffer, readc, sizeof(registro), regcmp);
		//Formata o nome de saida do arquivo
		char* destname;
		asprintf(&destname, "sub_%d_%s", subc, filename);
		//Salva o nome do subarquivo criado
		*sub_filenamesv = realloc(*sub_filenamesv, sizeof(char*)* (subc + 1));
		(*sub_filenamesv)[subc] = destname;
		//Escreve o cabeçalho e o buffer ordenado em disco
		fdest = fopen(destname,"wb");
		status = 0;
		fwrite(&status, 4, 1, fdest);
		fwrite(fbuffer, sizeof(registro), readc, fdest);
		rewind(fdest);
		status = 1;
		fwrite(&status, 4, 1, fdest);
		//Fecha o arquivo e aumenta a contagem de subarquivos escritos
		fclose(fdest);
		subc++;
	}
	
	//Desaloca o buffer
	free(fbuffer);

	//Retorna o numero de subarquivos gerados
	*sub_filenamesc = subc;

	return subc;
}