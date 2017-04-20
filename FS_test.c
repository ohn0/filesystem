#include "filesystem.h"

FILE* FILESTREAM;
unsigned int fileSize;

int print_block(int start_block)
{
	if(start_block % 512 != 0){printf("start_block not a multiple of 512.\n"); return -1;}
	rewind(FILESTREAM);
	fseek(FILESTREAM, start_block, SEEK_CUR);
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
	printf("%d\n", sizeof(int));
	FILESTREAM = fopen("FS10MB", "r+b");
	FILE* test = fopen("test", "r");
	int testSize = 0; int i = 0;
	while(fgetc(test) != EOF){testSize++;}
	printf("%d\n", testSize);
	char* data_buf = (char*) malloc(testSize * sizeof(char));
	rewind(test);
	while((data_buf[i++] = fgetc(test)) != EOF){}
	format();
	FS_reset();
	FS_jump(9);
	FS_putint(0xCCCC);
	add_file(data_buf, "test", testSize);
	rewind(FILESTREAM);
	FS_putint(0x00AB);
	FS_putint(0x00AA);
	FS_reset();
	rewind(FILESTREAM);
	FS_jump(0x200-1);
	FS_putint(0xAA);
	close(FILESTREAM);
	close(test);
	return 0;
}
