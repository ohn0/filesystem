#ifndef _IOCS_H
#define _IOCS_H
#include "filesystem.h"
int create_root_dir();
int generate_block_table();
struct open_dir* openDir(struct open_dir*, char*);

int fill_directory_table();

struct block_table
{
	int start_block;
	struct block_table* children;
	struct block_table* next_file;
};



int create_file();
int open_root();
struct children* find_child(struct open_dir*, char*);
#endif
