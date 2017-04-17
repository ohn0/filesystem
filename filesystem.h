#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define FILE_TABLE 		0x00
#define DIRECTORY_INDEX 0x200
#define BLOCK_SIZE 		0x200
#define FILE_NAME_LENGTH 11
struct entry{
	int start_block;
};
struct index_entry
{
	char entry_name[FILE_NAME_LENGTH];
	int last_mod_timestamp;
	int size;
	int start_block_location;
};
int format();
extern FILE* FILESTREAM;
extern unsigned int fileSize;
#endif
