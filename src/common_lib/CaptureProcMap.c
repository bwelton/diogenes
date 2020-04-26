#include "CaptureProcMap.h"

void CaptureProcMap_Write(char * filename) {
	char capture[200];
	char buffer[4096];
	size_t copySize = 0;
	char c;
	snprintf(capture, 200, "/proc/%d/maps", getpid());
	fprintf(stderr, "Opening procmap file: %s\n", capture);
	FILE * readFile = fopen(capture, "rb");
	FILE * fout = fopen(filename, "wb");

	if (readFile == NULL){
		fprintf(stderr, "%s %d\n", "COULD NOT OPEN PROCMAP FOR: ", getpid());
		return;
	}
	c = fgetc(readFile);
	while (c != EOF && c != 0xff && feof(readFile) == 0){
		buffer[copySize] = c;
		copySize++;
		if (copySize == 4096){
			fwrite(buffer,1,4096,fout);
			copySize = 0;
		}
		c = fgetc(readFile);
	}
	if (copySize != 0){
		fwrite(buffer,1,copySize,fout);
		copySize = 0;
	}	
	fclose(fout);
	fclose(readFile);
}
