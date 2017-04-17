#ifndef _DIRECTORY_INDEX_H
#define _DIRECTORY_INDEX_H
#include "filesystem.h"
struct index_entry* create_entry(char*);
int find_open_entry(int);
int find_entry(char*, int);
int update_index_entry(int, void*, int);
int set_filename(struct index_entry*, char*);
int add_file(void*, char*);
int delete_entry(struct index_entry*, int);

#endif