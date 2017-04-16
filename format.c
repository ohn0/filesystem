#include "filesystem.h"
int format()
{
	fseek(FILESTREAM, 0L, SEEK_END);
	fileSize = ftell(FILESTREAM);
	printf("%d\n", fileSize);
	rewind(FILESTREAM);
	fputc(0xAB, FILESTREAM);
	int i = 0;
	for(i = 0; i < 512; i++){
		fputc(0x00, FILESTREAM);
	}
	fseek(FILESTREAM, 510, SEEK_SET);
	fputc(0xFF, FILESTREAM);
	fputc(0xFF, FILESTREAM);

}