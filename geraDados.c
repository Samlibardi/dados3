#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "registro.h"
#include "string.h"

//quantidade de repetições de cada campo
#define REPEAT_FIELD1 0.15f
#define REPEAT_FIELD2 0.125f
#define REPEAT_FIELD3 0.1f
#define REPEAT_FIELD4 0.075f

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
		//gera os campos
		genField1();
		genField2();
		genField3();
		genField4();
		
		//salva o registro no arquivo
		fwrite(&curRecord, sizeof(registro), 1, fout);

		lastRecord = curRecord; //mantém o ultimo registro salvo em disco para referencia
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

//Gera o campo 1
int genField1() {
	//Caso seja determinado que o valor deve ser repetido, copiar o valor do ultimo registro salvo
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD1) {
		curRecord.campo1 = lastRecord.campo1;
		return 0;
	}
	
	curRecord.campo1 = rand() % 49999 + 1; //gera um numero aleatório entre 1 e 50000
	return 0;
}

//Gera o campo 2
int genField2() {
	//Caso seja determinado que o valor deve ser repetido, copiar o valor do ultimo registro salvo
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD2) {
		memcpy(curRecord.campo2,lastRecord.campo2, LEN_FIELD2);
		return 0;
	}

	//le um registro aleatório do arquivo de nomes de ruas
	fseek(fruas, rand() % cruas * LEN_FIELD2 + 4, SEEK_SET);
	fread(curRecord.campo2, LEN_FIELD2, 1, fruas);

	return 0;
}

//Gera o campo 3
int genField3() {
	int test = rand();
	//Caso seja determinado que o valor deve ser repetido, copiar o valor do ultimo registro salvo
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD3) {
		memcpy(curRecord.campo3, lastRecord.campo3, LEN_FIELD3);
		return 0;
	}

	//le um registro aleatório do arquivo de nomes de bairros
	fseek(fbairros, rand() % cbairros * LEN_FIELD3 + 4, SEEK_SET);
	int test2 = fread(curRecord.campo3, LEN_FIELD3, 1, fbairros);

	return 0;
}

//Gera o campo 4
int genField4() {
	//Caso seja determinado que o valor deve ser repetido, copiar o valor do ultimo registro salvo
	if (!isFirstRecord && rand() / (float)RAND_MAX < REPEAT_FIELD4) {
		strcpy(curRecord.campo4, lastRecord.campo4);
		return 0;
	}

	time_t t = ((time_t)rand() << sizeof(int) * 8 | rand()) % time(NULL); //gera um tempo aleatório antes de hoje
	char buffer[11];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y", localtime(&t)); //converte o tempo para string de data
	memcpy(curRecord.campo4, buffer, LEN_FIELD4); //salva no registro

	return 0;
}