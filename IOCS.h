#ifndef _IOCS_H
#define _IOCS_H
#include "filesystem.h"
int create_root_dir();
int generate_block_table();


int fill_directory_table();

struct block_table
{
	int start_block;
	struct block_table* children;
	struct block_table* next_file;
};



int create_file();
int open_root();

#endif
