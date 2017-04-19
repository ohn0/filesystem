#ifndef _FILE_ENTRY_H
#define _FILE_ENTRY_H
#include "filesystem.h"
int add_file(void* , char*);
int update_index_entry(int, void*, int);
int create_index_entry(char*);
int file_size(void*);
int* get_file(int*);

#endif
