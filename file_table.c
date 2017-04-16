#include "file_table.h"
int add_to_file(int * filesize)
{
	//Find filesize/512 blocks and make each point to the next.
	FS_reset();
	int num_blocks = ((*filesize)/BLOCK_SIZE);
	int i,j,k;
	i = j = k = 0;
	int block_pos[num_blocks];
	while(i < num_blocks){
		do{
			j+=2;
		}while(!(FS_getint() == 0x00));
		block_pos[k++] = ftell(FILESTREAM)-2;
		i++;
	}

	// for(i = 0;i < num_blocks; i++){printf("%d ", block_pos[i]/2);}
	// printf("\n");
	for(i = k = 0; i < num_blocks-1; i++){
		// printf("%X\n", block_pos[i+1]/2);
		fseek(FILESTREAM, block_pos[i], SEEK_SET);
		FS_putint(block_pos[i+1]/2);
	}
	// printf("\n");
	fseek(FILESTREAM, block_pos[num_blocks-1], SEEK_SET);
	FS_putint(~0);
	return block_pos[0]/2;
}

int write_to_table(int* a)
{
	return 1;
}