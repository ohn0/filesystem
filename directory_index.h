#ifndef _DIRECTORY_INDEX_H
#include _DIRECTORY_INDEX_H
#include "filesystem.h"
struct index_entry
{
	char entry_name[11];
	int last_mod_date;
	int last_mod_time;
	int size;
	int start_block_location;
};
#endif