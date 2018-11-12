#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "registro.h"

//quantidade de repetições de cada campo
#define REPEAT_FIELD1 0.3f
#define REPEAT_FIELD2 0.25f
#define REPEAT_FIELD3 0.2f
#define REPEAT_FIELD4 0.15f

//codigos de erro padrão
#define ERROR_FOPEN 1

//tipo de dados auxiliar
typedef char byte;
#define TRUE '\1';
#define FALSE 0;

registro lastRecord; //ultimo registro salvo em arquivo
registro curRecord; //registro a ser salvo em arquivo

byte isFirstRecord; //marca se o registro sendo gerado é o primeiro (impossível repetir campos)
int status; //cabeçalho do arquivo

FILE* fout;
FILE* fruas;
unsigned int cruas;
FILE* fbairros;
unsigned int cbairros;

int genDataFile(int n, char* filename) {
	srand(time(NULL)); //Inicializa a seed do PRNG

	//Abre os arquivos de dados base e de saída. 
	fout = fopen(filename, "wb");
	fruas = fopen("ruas.bin", "rb");
	fbairros = fopen("bairros.bin", "rb");
	if (!fout || !fruas || !fbairros) return ERROR_FOPEN;

	//Le os cabeçalhos dos arquivos base
	fread(&cruas, 4, 1, fruas);
	fread(&cbairros, 4, 1, fbairros);

	//Marca o cabeçalho do arquivo de saída com status 0
	status = 0;
	fwrite(&status, 4, 1, fout);

	isFirstRecord = TRUE;

	for (int i = 0; i < n; i++) {
		genField1();
		genField2();
		genField3();
		genField4();
		
		//salva o registro no arquivo
		fwrite(&curRecord, sizeof(registro), 1, fout);

		lastRecord = curRecord;
		isFirstRecord = FALSE;
	}

	//escreve o status 1 no cabeçalho do arquivo e fecha todos os arquivos
	status = 1;
	rewind(fout);
	fwrite(&status, 4, 1, fout);

	fclose(fruas);
	fclose(fbairros);
	fclose(fout);
	return 0;
}

int genField1() {
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD1) {
		curRecord.campo1 = lastRecord.campo1;
		return 0;
	}
	
	curRecord.campo1 = rand() % 49999 + 1;
	return 0;
}

int genField2() {
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD2) {
		strcpy(curRecord.campo2,lastRecord.campo2);
		return 0;
	}

	fseek(fruas, rand() % cruas * 30 + 4, SEEK_SET);
	fread(curRecord.campo2, LEN_FIELD2, 1, fruas);

	return 0;
}

int genField3() {
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD3) {
		strcpy(curRecord.campo3, lastRecord.campo3);
		return 0;
	}

	fseek(fruas, rand() % cruas * 30 + 4, SEEK_SET);
	fread(curRecord.campo3, LEN_FIELD3, 1, fbairros);

	return 0;
}

int genField4() {
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD4) {
		strcpy(curRecord.campo4, lastRecord.campo4);
		return 0;
	}

	time_t t = rand() % 48 * (time_t)31536000;
	char buffer[11];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y", localtime(&t));
	memcpy(curRecord.campo4, buffer, LEN_FIELD4);

	return 0;
}