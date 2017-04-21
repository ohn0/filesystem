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
	fputc(0xFF, FILESTREAM);
	fputc(0xFF, FILESTREAM);
	return 0;
}
