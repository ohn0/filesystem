#include "file_table.h"
int* add_to_file(int * filesize)
{
	//Find filesize/512 blocks and make each point to the next.
	FS_reset();
	int num_blocks = ceil((double)(*filesize)/BLOCK_SIZE);
	int i,j,k;
	i = j = k = 0;
	int* block_pos = (int*) malloc(sizeof(int) * num_blocks);
	FS_jump(4);
	if(block_pos == NULL){return -1;}
	while(i < num_blocks){
		do{
			j+=2;
		}while(!(FS_getMiniInt() == 0x00));
		block_pos[k++] = ftell(FILESTREAM)-2;
		i++;
	}

	// for(i = 0;i < num_blocks; i++){printf("%d ", block_pos[i]/2);}
	// printf("\n");
	for(i = k = 0; i <= num_blocks; i++){
		// printf("%X\n", block_pos[i+1]/2);
		FS_jump(block_pos[i]);
		FS_putMiniInt(block_pos[i+1]/2);
	}
	// printf("\n");
	fseek(FILESTREAM, block_pos[num_blocks-1], SEEK_SET);
	FS_putMiniInt(0xFFFF);
	for(i = 0; i < num_blocks; i++){
		block_pos[i] = block_pos[i]/2;
		printf("Block: %d\n", block_pos[i]);
	}
	return block_pos;
}

int write_to_table(int* a)
{
	return 1;
}
