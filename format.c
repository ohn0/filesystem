#include "filesystem.h"
int format()
{
	//Clears the file system.
	fseek(FILESTREAM, 0L, SEEK_END);
	fileSize = ftell(FILESTREAM);
	printf("%d\n", fileSize);
	rewind(FILESTREAM);
	int i = 0;
	for(i = 0; i < fileSize; i++){
		fputc(0x00, FILESTREAM);
	}
	FS_reset();
	fseek(FILESTREAM, 510, SEEK_SET);
	create_file_table();
	create_directory_index();
	return 0;
}

int create_file_table()
{
	//Allocate enough blocks at the beginning of the disk that will hold enough
	//entries that can map to all the blocks on the disk.
	//Call right after format
	FS_reset();
	//Find the max number of file table entries that the current filesize requires.
	virtual_offset = ceil((ceil((double)fileSize/BLOCK_SIZE))/256);
	return 0;
}

int create_directory_index()
{
	DIRECTORY_INDEX = BLOCK_SIZE * virtual_offset;
	return 0;
}
