#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registro.h"

#define SPLIT_SIZE 64000
#define SPLIT_REC_COUNT (SPLIT_SIZE - 4)/sizeof(registro)

int status;

int split(char* filename) {
	int subc = 0, readc, destnamelen;
	FILE* fsrc = fopen(filename, "rb"), *fdest;

	fread(&status, 4, 1, fsrc);
	
	char* destname = malloc(destnamelen = (strlen(filename) + 22) * sizeof(char));

	void* fbuffer = malloc(SPLIT_SIZE);

	while (readc = fread(fbuffer, sizeof(registro), SPLIT_REC_COUNT, fsrc)) {
		sprintf(destname, "sub_%d_%s", subc, filename);
		fdest = fopen(destname,"wb");
		status = 0;
		fwrite(&status, 4, 1, fdest);
		fwrite(fbuffer, sizeof(registro), readc, fdest);
		rewind(fdest);
		status = 1;
		fwrite(&status, 4, 1, fdest);
		fclose(fdest);
		subc++;
		if (readc < SPLIT_REC_COUNT) break;
	}

	char** subnames = malloc(sizeof(char*) * subc);
	/*for (int i = 0; i < subc; i++) {
		subnames[i] = malloc(destnamelen);
		strcpy(subnames[i], destname);
		subnames[i][destnamelen - 1] = '0' + i;
	}*/

	return subc;
}