#include "delete.h"
int delete_entry(char* fileName)
{
	//Use the information provided from entry to set the first value
	//of the directory entry to 00, marking it as deleted, then delete
	//the block chain and the contents at those block locations.
	struct index_entry* entry = find_entry(fileName, DIRECTORY_INDEX);
	if(entry == NULL){printf("No file with specified name.\n"); return 0;}
	printf("%X", entry->entry_location/BLOCK_SIZE);
	update_index_count(BLOCK_SIZE* (entry->entry_location / BLOCK_SIZE), REMOVE);
	FS_reset();
	FS_jump(entry->entry_location);
	FS_putc(0x00);
	delete_block_chain(entry->start_block_location);
	return 0;
}

int delete_block_chain(int start_block)
{
	//Beginning at start block, walk through the block chain until
	//0xFFFF is encountered while deleting any encountered
	//file blocks.
	FS_reset(); int i = 0;
	printf("Starting block: %X\n", BLOCK_SIZE * start_block);
	FS_jump(2 * start_block);
	unsigned int next_block = FS_getMiniInt();
	delete_block_contents((start_block + virtual_offset) * BLOCK_SIZE);
	int fpos;
	while(next_block != 0xFFFF){
		delete_block_contents((next_block + virtual_offset) * BLOCK_SIZE);
		FS_reset();
		FS_jump((2 * next_block));
		next_block = FS_getMiniInt();
		fpos = FS_getpos();
		FS_reset();
		FS_jump(fpos - 2);
		FS_putMiniInt(0x0000);
	}
	FS_reset();
	FS_jump(2 * start_block);
	FS_putMiniInt(0x0000);
	return 0;

}

int delete_block_contents(int start_block)
{
	int i;
	FS_reset();
	FS_jump(start_block);
	for(i = 0; i < BLOCK_SIZE; i++){ 
		FS_putc(0x00);
	}
	return 0;
}
