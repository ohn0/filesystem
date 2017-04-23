#ifndef _EXTEND_H
#define _EXTEND_H
#include "filesystem.h"
int extend_directory_index();
int write_file(void*, struct index_entry*, int);
int* obtain_file_blocks(struct index_entry*);
int write_to_disk(int*, struct index_entry*, int, void*);

#endif
