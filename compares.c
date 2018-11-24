#include "registro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Funçao de comparação entre registros a e b, de acordo com as especificações do projeto
int regcmp(const void *a, const void *b) {
	const registro ra = *(registro*)a;
	const registro rb = *(registro*)b;

	int dif;
	if (dif = (ra.campo1 - rb.campo1)) return dif;
	if (dif = strncmp(ra.campo2, rb.campo2, LEN_FIELD2)) return dif;
	if (dif = strncmp(ra.campo3, rb.campo3, LEN_FIELD3)) return dif;
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
