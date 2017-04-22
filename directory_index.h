#ifndef _DIRECTORY_INDEX_H
#define _DIRECTORY_INDEX_H
#include "filesystem.h"
int find_open_entry(int);
struct index_entry* find_entry(char*, int);
int entry_exists(char*, int);
int update_index_entry(int, void*, int);
int update_index_count(int, unsigned int);
unsigned char get_index_count(int);
unsigned int get_next_dir_index(int);
#endif
