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

int regcmp(const void *a, const void *b) {
	const registro ra = *(registro*)a;
	const registro rb = *(registro*)b;

	int dif;
	if (dif = (ra.campo1 - rb.campo1)) return dif;
	if (dif = strcmp(ra.campo2, rb.campo2)) return dif;
	if (dif = strcmp(ra.campo3, rb.campo3)) return dif;
	dif = datecmp(ra.campo4, rb.campo4); return dif;
}

int datecmp(const char *a, const char *b) {
	short int da, db, ma, mb, ya, yb;
	sscanf(a, "%2d/%2d/%4d", da, ma, ya);
	sscanf(b, "%2d/%2d/%4d", db, mb, yb);
	short int dif;
	if (dif = (ya - yb)) return dif;
	if (dif = (ma - mb)) return dif;
	dif = (da - db); return dif;
}

int ordenaInterna(char* srcname, char* destname) {
	fdados = fopen(srcname, "rb");

	if (!fdados) return ERROR_FOPEN;
	fread(&status, 4, 1, fdados);
	if (!status) return ERROR_STATUS;

	recordsbs = LEN_INIT;
	recordsc = 0;
	while(1) {
		recordsv = realloc(recordsv, sizeof(registro) * recordsbs);
		if ((recordsc += fread(recordsv + recordsc, sizeof(registro), recordsbs - recordsc, fdados)) < recordsbs) {
			break;
		}
		recordsbs *= LEN_GROWTH;
	}
	fclose(fdados);
	
	fdados = fopen(destname, "wb");
	status = 0;
	fwrite(&status, sizeof(int), 1, fdados);
	
	qsort(recordsv, recordsc, sizeof(registro), regcmp);
	fwrite(recordsv, sizeof(registro), recordsc, fdados);
	

	rewind(fdados);
	status = 1;
	fwrite(&status, sizeof(int), 1, fdados);
	fclose(fdados);
	free(recordsv);

	return 0;
}

