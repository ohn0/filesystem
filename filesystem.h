#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define BLOCK_SIZE 		0x200
#define ENTRY_NAME		0x000
#define ENTRY_MOD_TIME		0x01
#define ENTRY_MOD_DATE		0x02
#define ENTRY_SIZE		0x03
#define ENTRY_LOCATION		0x04
#define ADD			0x01
#define REMOVE			0x02
#define FILENAME_LENGTH		 11
#define DIR_INDEX_FULL_VAL	 25
#define ENTRY_TYPE_DIR 		0x01
#define ENTRY_TYPE_FILE		0x02
#define NEXT_DIR_INDEX		505
#define APPEND			0x01
#define OVERWRITE		0x10
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
	int entry_index_location;
	int entry_type;
};
struct directory_table
{
	struct index_entry* entry;
	struct directory_table* next;	
	char* data;
};
struct open_dir
{
	struct index_entry* dir_entry;
	struct open_dir* parent;
	struct children* child_list;
	//int child_count;
};


struct children
{
	struct children* next_child;
	struct parent_dir* parent;
	struct index_entry* data;
	char* file_buf;
	int deleted;
};
extern FILE* FILESTREAM;
extern unsigned int fileSize;
extern unsigned int virtual_offset;
extern unsigned int DIRECTORY_INDEX;
extern struct directory_table* entries;
extern struct open_dir* root_dir;
extern struct index_entry* ROOT;
#endif
