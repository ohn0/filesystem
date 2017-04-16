#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define FILE_TABLE 		0x00
#define DIRECTORY_INDEX 0x200
#define BLOCK_SIZE 		0x200
struct entry{
	int start_block;
};
int format();
extern FILE* FILESTREAM;
extern unsigned int fileSize;
#endif