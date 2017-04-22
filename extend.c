#include "extend.h"

int extend_directory_index(int entry_point)
{
	//Find an open block to extend the directory index.
	//This will be done very similarly to how the filesystem
	//creates a new file.
	int size = BLOCK_SIZE;
	int* new_index_location = add_to_file(&size);
	FS_reset();
	FS_jump(entry_point + 505);
	FS_putMiniInt((new_index_location[0] + virtual_offset) * BLOCK_SIZE);
	free(new_index_location);
	return 0;
}

int extend_file(struct index_entry* entry)
{
	//Holy shit this will be HARD.
}
