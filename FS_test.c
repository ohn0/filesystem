#include "filesystem.h"

FILE* FILESTREAM;
unsigned int fileSize;

int print_block(int start_block)
{
	if(start_block % 512 != 0){printf("start_block not a multiple of 512.\n"); return -1;}
	rewind(FILESTREAM);
	int i = 0;
	while(i < 512){
		printf("%.2X ", fgetc(FILESTREAM));
		i++;
		if(i % 16 == 0){printf("\n");}
	}
	printf("\n");
	return 0;
}
int main(int argc, char const *argv[])
{
	FILESTREAM = fopen("FS10MB", "r+b");
	format();
	rewind(FILESTREAM);
	int size = 32 * 512;
	printf("%d\n", add_to_file(&size));
	print_block(FILE_TABLE);
	close(FILESTREAM);
	return 0;
}
