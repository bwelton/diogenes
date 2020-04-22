#include "FileIO.h"
#include <stdio.h>
char * FileIO_ReadFile(void * (*allocator_fun)(size_t),char * filename, uint64_t * size) {
    FILE* file=fopen(filename,"rb");
    fprintf(stderr, "Opening file %s\n", filename);
    if (file == NULL){
        fprintf(stderr, "File %s failed to open!\n",filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);
    fprintf(stderr, "Reading %lu!\n",fsize);
    char * ret = (char *)allocator_fun(fsize+1);
    fsize = fread(ret, 1, fsize, file);
    fclose(file);
    *size = fsize;
    ret[fsize] = '\000';
    fprintf(stderr, "Read %lu!\n",fsize);
    return ret;
}

int FileIO_DebugFlag = -1;
int FileIO_CheckDebug() {
    if (FileIO_DebugFlag == -1) {
        if (getenv("DIOGENES_DEBUG_OUTPUT")) {
            FileIO_DebugFlag = 1;
        } else {
            FileIO_DebugFlag = 0;
        }
    }
    return FileIO_DebugFlag;
}