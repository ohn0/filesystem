#ifndef _DIRECTORY_INDEX_H
#define _DIRECTORY_INDEX_H
#include "filesystem.h"
int find_open_entry(int);
struct index_entry* find_entry(char*, int);
int update_index_entry(int, void*, int);
int delete_entry(struct index_entry*, int);
int write_entry(int, char*);
#endif
