#ifndef _EXTEND_H
#define _EXTEND_H
#include "filesystem.h"
int extend_file_table();
int extend_directory_index();
int extend_file();
int generate_block_ID();
int update_free_blocks();

#endif
