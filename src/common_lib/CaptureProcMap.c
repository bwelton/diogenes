#include "CaptureProcMap.h"

void CaptureProcMap_Write(char * filename) {
	char capture[200];
	snprintf(capture, 200, "/proc/%d/maps", getpid());
	fprintf(stderr, "Opening procmap file: %s\n", capture);
	FILE * readFile = fopen(capture, "rb");
	if (readFile == NULL){
		fprintf(stderr, "%s %d\n", "COULD NOT OPEN PROCMAP FOR: ", getpid());
		return;
	}
	rewind(readFile);
	fseek(readFile, 0, SEEK_END);
	size_t fsize = ftell(readFile);
	fseek(readFile, 0, SEEK_SET);
	fprintf(stderr, "Reading %lu bytes\n", fsize);
	char * data = (char*)malloc(fsize);
	fread(data, 1, fsize, readFile);

	FILE * fout = fopen(filename, "wb");
	fwrite(data, 1, fsize, fout);
	fclose(fout);
	fclose(readFile);
	free(data);
}