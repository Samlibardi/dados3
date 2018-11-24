#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_OPEN_FILES 256


int ordenaExterna(char* srcname, char* destname) {
	int splitc;
	int err;

	if (splitc = split(srcname)) {
		int destnamelen;
		char** splitv = malloc(sizeof(char*) * splitc);
		for (int i = 0; i < splitc; i++) {
			char* destname = malloc(destnamelen = (strlen(srcname) + 22) * sizeof(char));
			sprintf(destname, "sub_%d_%s", i, srcname);
			splitv[i] = destname;
			ordenaInterna(destname, destname);
		}

		char **srcsubv = splitv, **destsubv;
		int srcsubc = splitc, destsubc, totalsubs = splitc;

		do {
			destsubc = ceilf(srcsubc / (float)MAX_OPEN_FILES);
			destsubv = malloc(sizeof(char*) * destsubc);
			for (int i = 0; i < destsubc; i++, totalsubs++) {
				char* destname = destsubv[i] = malloc(22 * sizeof(char));
				sprintf(destname, "sub_%d_%s", totalsubs, srcname);
				if (err = merge(min(256, srcsubc - i * MAX_OPEN_FILES), srcsubv + i * MAX_OPEN_FILES, destname))
					return err;
			}
			for (int i = 0; i < srcsubc; i++) free(srcsubv[i]);
			free(srcsubv);
			srcsubv = destsubv;

			srcsubc = destsubc;
		} while (destsubc > 1);

		remove(destname);
		rename(destsubv[0], destname);

		//for (int i = 0; i < splitc; i += 256) {
		//	char* destname = malloc(20);
		//	sprintf(destname, "sub_s%s.part%d", srcname, i);
		//	if (err = merge(min(256, splitc - i), splitv + i, destname))
		//		return err;
		//}
		return 0;
	}
}