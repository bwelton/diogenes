#include "CaptureProcMap.h"

void CaptureProcMap_Write(char * filename) {
	char capture[200];
	snprintf(capture, 200, "/proc/%d/maps", getpid());
	FILE * fread = fopen(capture, "rb");
	if (fread == NULL){
		fprintf(stderr, "%s %d\n", "COULD NOT OPEN PROCMAP FOR: ", getpid());
		return;
	}
	
	fseek(fread, 0, SEEK_END);
	size_t fsize = ftell(fread);
	fseek(fread, 0, SEEK_SET);

	FILE * fout = fopen(filename, "wb");
	sendfile(fileno(fout), fileno(fread), NULL, fsize);
	fclose(fout);
	fclose(fread);
}