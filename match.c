#include <stdlib.h>
#include <stdio.h>
#include "registro.h"
#include "compares.h"

#define ERROR_FOPEN 1;
#define ERROR_STATUS 2;

int status;

FILE* fdest;

registro* recordsv;

// Função merge recebe um vetor de nomes de arquivo, e um nome de destino
int match(const int srcc, const char** srcnamev, char* destname) {
	const FILE** srcv = malloc(sizeof(FILE*) * srcc);	//vetor de arquivos
	const registro* recordsv = malloc(sizeof(registro) * srcc); //vetor que mantém o primeiro registro de cada arquivo
	int srcopenc = srcc; // Número de arquivos abertos

	//Abrir toodos os arquivos, checar o cabeçalho
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

	//Enquanto tiver arquivos abertos (pelo menos 2 para haver match), 
	while (srcopenc > 1) {
		
		//Encontrar o registro de menor valor, do vetor de primeiros registros
		int imin = -1;
		for (int i = 0; i < srcc; i++) {
			if (!srcv[i]) continue; //Ignorar se o arquivo ja foi fechado
			if (imin < 0) { imin = i; continue; }
			if (regcmp(recordsv + i, recordsv + imin) < 0) {
				imin = i;
			}
		}

		//Verificar entre os registros se existe o match 
		int isMatch = 1;
		for (int i = 0; i < srcc; i++) {
			if (!srcv[i]) continue; //Ignorar se o arquivo ja foi fechado
			if (i == imin) continue;
			if ((recordsv+i)->campo1 != (recordsv+imin)->campo1) {
				isMatch = 0;
				break;
			}
		}

		//Escrever o registro no arquivo destino, caso seja um Match
		if(isMatch)
			fwrite(recordsv + imin, sizeof(registro), 1, fdest);

		//Tenta ler o proximo registro do arquivo que possuia o registro que foi escrito.
		//Caso não seja possível, fecha o arquivo
		if (!fread(recordsv + imin, sizeof(registro), 1, srcv[imin])) {
			srcv[imin] = fclose(srcv[imin]);
			srcopenc--;
		}
	}

	//Escreve o cabeçalho 1 no arquivo destino, fecha e limpa a memória usada
	rewind(fdest);
	status = 1;
	fwrite(&status, sizeof(int), 1, fdest);
	fclose(fdest);
	free(recordsv);
	free(srcv);

	return 0;
}