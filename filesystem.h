#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define FILE_TABLE 		0x00
#define DIRECTORY_INDEX 	0x200
#define BLOCK_SIZE 		0x200
#define ENTRY_NAME		0x000
#define ENTRY_MOD_TIME		0x01
#define ENTRY_MOD_DATE		0x02
#define ENTRY_SIZE		0x03
#define ENTRY_LOCATION		0x04
#define FILENAME_LENGTH 11
struct entry{
	int start_block;
};
struct index_entry
{
	char entry_name[FILENAME_LENGTH];
	int last_mod_timestamp;
	int size;
	int start_block_location;
	int entry_location;
};
int format();
extern FILE* FILESTREAM;
extern unsigned int fileSize;
#endif
