#include <stdio.h>
#include <stdlib.h>
#include "registro.h"

#define LEN_INIT 100;
#define LEN_GROWTH 1.5;

#define ERROR_FOPEN 1;
#define ERROR_STATUS 2;

registro* recordsv;
int recordsc;
int recordsbs;

int status;

FILE* fdados;

//Funçao de comparação entre registros a e b, de acordo com as especificações do projeto
int regcmp(const void *a, const void *b) {
	const registro ra = *(registro*)a;
	const registro rb = *(registro*)b;

	int dif;
	if (dif = (ra.campo1 - rb.campo1)) return dif;
	if (dif = strcmp(ra.campo2, rb.campo2)) return dif;
	if (dif = strcmp(ra.campo3, rb.campo3)) return dif;
	dif = datecmp(ra.campo4, rb.campo4); return dif;
}

//Funçao de comparação entre datas de formato dd/mm/yyyy
int datecmp(const char *a, const char *b) {
	short int da, db, ma, mb, ya, yb;
	sscanf(a, "%2hd/%2hd/%4hd", &da, &ma, &ya);
	sscanf(b, "%2hd/%2hd/%4hd", &db, &mb, &yb);
	short int dif;
	if (dif = (ya - yb)) return dif;
	if (dif = (ma - mb)) return dif;
	dif = (da - db); return dif;
}

// Função principal da ordenação interna
int ordenaInterna(char* srcname, char* destname) {
	fdados = fopen(srcname, "rb"); //Abre o arquivo de origem como leitura

	//Checa se o arquivo foi aberto e se o status é valido
	if (!fdados) return ERROR_FOPEN;
	fread(&status, 4, 1, fdados);
	if (!status) return ERROR_STATUS;

	//Loop de alocação e leitura. O tamanho de alocação cresce em PG até que seja possível carregar todo o arquivo em memória
	recordsbs = LEN_INIT;
	recordsc = 0;
	while(1) {
		recordsv = realloc(recordsv, sizeof(registro) * recordsbs); //realoca o vetor de registros
		//Tenta ler os registros do arquivo. Caso todos caibam na memória já alocada, sai do loop
		if ((recordsc += fread(recordsv + recordsc, sizeof(registro), recordsbs - recordsc, fdados)) < recordsbs) {
			break;
		}
		recordsbs *= LEN_GROWTH;
	}
	fclose(fdados); //Fecha o arquivo de origem
	
	//Abre o arquivo destino como escrita, e escreve o status 0 no cabeçalho
	fdados = fopen(destname, "wb");
	status = 0;
	fwrite(&status, sizeof(int), 1, fdados);
	
	//Ordena o vetor por quicksort
	qsort(recordsv, recordsc, sizeof(registro), regcmp);

	//Escreve todos os registros no arquivo destino de uma só vez (forma mais rápida de escrita sequencial)
	fwrite(recordsv, sizeof(registro), recordsc, fdados);
	
	//Escreve o status 1 no cabeçalho, fecha o arquivo e libera a memória alocada
	rewind(fdados);
	status = 1;
	fwrite(&status, sizeof(int), 1, fdados);
	fclose(fdados);
	free(recordsv);

	return 0;
}

