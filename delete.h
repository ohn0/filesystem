#ifndef _DELETE_H
#define _DELETE_H
#include "filesystem.h"
int delete_entry(struct index_entry*);
int delete_block_chain(int);
int delete_block_contents(int);

#endif
