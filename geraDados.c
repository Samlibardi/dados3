#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//definida a quantidade de repetições de cada campo
#define REPEAT_FIELD1 0.3f
#define REPEAT_FIELD2 0.25f
#define REPEAT_FIELD3 0.2f
#define REPEAT_FIELD4 0.15f

#define LEN_FIELD2 30
#define LEN_FIELD3 20
#define LEN_FIELD4 10

typedef struct {
	unsigned int campo1;
	char campo2[LEN_FIELD2];
	char campo3[LEN_FIELD3];
	char campo4[LEN_FIELD4];
} registro;

typedef char bool;
#define TRUE '\1';
#define FALSE 0;

registro lastRecord;
registro curRecord;

bool isFirstRecord;
int status;


FILE* fout;

FILE* fruas;
unsigned int cruas;

FILE* fbairros;
unsigned int cbairros;

int main() {
	genDataFile(4000,"out.bin");
	return 0;
}

int genDataFile(int n, char* filename) {
	srand(time(NULL));

	fout = fopen(filename, "wb");
	status = 0;
	fwrite(&status, 4, 1, fout);

	fruas = fopen("ruas.bin", "rb");
	fread(&cruas, 4, 1, fruas);

	fbairros = fopen("bairros.bin", "rb");
	fread(&cbairros, 4, 1, fbairros);

	isFirstRecord = TRUE;

	for (int i = 0; i < n; i++) {
		genField1();
		genField2();
		genField3();
		genField4();
		
		fwrite(&curRecord, sizeof(registro), 1, fout);

		lastRecord = curRecord;
		isFirstRecord = FALSE;
	}

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