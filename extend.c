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
	//FS_putMiniInt(((new_index_location[0] + virtual_offset) * BLOCK_SIZE));
	FS_putMiniInt(new_index_location[0]);
	free(new_index_location);
	return 0;
}

int write_file(void* data, struct index_entry* entry, int data_size)
{
	//Writes data into the file location of entry. If data_size is different from
	//the file associated with entry, all the blocks of the file will be reallocated 
	//and rewritten. The blocks will just be written over if data is of the same size.
	int* file_blocks =  obtain_file_blocks(entry);
	int i;
	printf("Writing to %X", *file_blocks);
	int dSize = data_size;
	int eSize;
	if(entry->size < BLOCK_SIZE){eSize = 1;}
	else{eSize = entry->size / BLOCK_SIZE;}
	//I have no idea why this is here...
//	if(data_size != entry->size){
		//if((data_size / BLOCK_SIZE) != (entry->size / BLOCK_SIZE)){
		clear_blocks(file_blocks, eSize);
		free(file_blocks);
		file_blocks = add_to_file(&dSize);
		entry->size = data_size;
		entry->start_block_location = file_blocks[0];

		write_to_disk(file_blocks, entry, data_size, data);
//	}
	entry->last_mod_timestamp = time(NULL);
	//INVALID ENTRY POINT(no virtual_offset is set)
	printf("\n%X\n", entry->entry_location);
	populate_entry(entry, entry->entry_location);
	return 0;
}

int write_to_disk(int* blocks, struct index_entry* entry, int size, void* data)
{
	int i, blockCount;
	blockCount = ceil(((double)size)/BLOCK_SIZE);
	for(i = 0; i < blockCount; i++){
		FS_reset();
		FS_jump(BLOCK_SIZE *(blocks[i] + virtual_offset));
		if(size > BLOCK_SIZE){
			size -= BLOCK_SIZE;
			write_block(data, BLOCK_SIZE);
		}else{
			write_block(data, size);
		}
		data+= BLOCK_SIZE;
	}
	return 0;
}

int* obtain_file_blocks(struct index_entry* entry)
{
	int start_block = entry->start_block_location;
	int* file_blocks = (int*) malloc(sizeof(int) * (((double) entry->size) / BLOCK_SIZE));
	int bSize = entry->size;

	int total_blocks = (double)entry->size/BLOCK_SIZE;
	if(entry->size < BLOCK_SIZE){
		total_blocks = 1;
	}
	int i;
	FS_reset();
	FS_jump(2 * start_block);
	file_blocks[0] = start_block;
	for( i = 1; i < total_blocks; i++ ){
		file_blocks[i] = FS_getMiniInt();
		FS_reset();
		FS_jump(2 * file_blocks[i]);
	}
	return file_blocks;
}
