#include "delete.h"
int delete_entry(struct index_entry* entry)
{
	//Use the information provided from entry to set the first value
	//of the directory entry to 00, marking it as deleted, then delete
	//the block chain and the contents at those block locations.
	FS_reset();
	FS_jump(entry->entry_location);
	FS_putc(0x00);
	delete_block_chain(entry->start_block_location);
	return 0;
}

int delete_block_chain(int start_block)
{
	FS_reset();
	FS_jump(2 * start_block);
	int next_block = start_block;
	while(next_block != 0xFFFF){
		printf("%X\n", next_block);
		FS_jump(2 * next_block);
		next_block = FS_getMiniInt();
		FS_jump(FS_getpos() - 2);
		FS_putMiniInt(0x0000);
		delete_block_contents(start_block * BLOCK_SIZE);
		start_block = next_block;
	}
	return 0;

}

int delete_block_contents(int start_block)
{
	int i;
	FS_jump(start_block);
	for(i = 0; i < BLOCK_SIZE; i++){ 
		FS_putc(0x00);
	}
	return 0;
}
