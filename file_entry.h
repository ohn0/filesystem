#ifndef _FILE_ENTRY_H
#define _FILE_ENTRY_H
#include "filesystem.h"
struct index_entry* create_entry(char*);
int populate_entry_struct(struct index_entry*, int);
int populate_entry(struct index_entry*, int);
int set_filename(struct index_entry*, char*);
int add_file(void*, char*, int, int);
int write_block(char*, int);
char* read_block(int, int);
char* read_file(struct index_entry*);
#endif
