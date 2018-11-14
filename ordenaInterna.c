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

//Fun�ao de compara��o entre registros a e b, de acordo com as especifica��es do projeto
int regcmp(const void *a, const void *b) {
	const registro ra = *(registro*)a;
	const registro rb = *(registro*)b;

	int dif;
	if (dif = (ra.campo1 - rb.campo1)) return dif;
	if (dif = strcmp(ra.campo2, rb.campo2)) return dif;
	if (dif = strcmp(ra.campo3, rb.campo3)) return dif;
	dif = datecmp(ra.campo4, rb.campo4); return dif;
}

//Fun�ao de compara��o entre datas de formato dd/mm/yyyy
int datecmp(const char *a, const char *b) {
	short int da, db, ma, mb, ya, yb;
	sscanf(a, "%2hd/%2hd/%4hd", &da, &ma, &ya);
	sscanf(b, "%2hd/%2hd/%4hd", &db, &mb, &yb);
	short int dif;
	if (dif = (ya - yb)) return dif;
	if (dif = (ma - mb)) return dif;
	dif = (da - db); return dif;
}

// Fun��o principal da ordena��o interna
int ordenaInterna(char* srcname, char* destname) {
	fdados = fopen(srcname, "rb"); //Abre o arquivo de origem como leitura

	//Checa se o arquivo foi aberto e se o status � valido
	if (!fdados) return ERROR_FOPEN;
	fread(&status, 4, 1, fdados);
	if (!status) return ERROR_STATUS;

	//Loop de aloca��o e leitura. O tamanho de aloca��o cresce em PG at� que seja poss�vel carregar todo o arquivo em mem�ria
	recordsbs = LEN_INIT;
	recordsc = 0;
	while(1) {
		recordsv = realloc(recordsv, sizeof(registro) * recordsbs); //realoca o vetor de registros
		//Tenta ler os registros do arquivo. Caso todos caibam na mem�ria j� alocada, sai do loop
		if ((recordsc += fread(recordsv + recordsc, sizeof(registro), recordsbs - recordsc, fdados)) < recordsbs) {
			break;
		}
		recordsbs *= LEN_GROWTH;
	}
	fclose(fdados); //Fecha o arquivo de origem
	
	//Abre o arquivo destino como escrita, e escreve o status 0 no cabe�alho
	fdados = fopen(destname, "wb");
	status = 0;
	fwrite(&status, sizeof(int), 1, fdados);
	
	//Ordena o vetor por quicksort
	qsort(recordsv, recordsc, sizeof(registro), regcmp);

	//Escreve todos os registros no arquivo destino de uma s� vez (forma mais r�pida de escrita sequencial)
	fwrite(recordsv, sizeof(registro), recordsc, fdados);
	
	//Escreve o status 1 no cabe�alho, fecha o arquivo e libera a mem�ria alocada
	rewind(fdados);
	status = 1;
	fwrite(&status, sizeof(int), 1, fdados);
	fclose(fdados);
	free(recordsv);

	return 0;
}

