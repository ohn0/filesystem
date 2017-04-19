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
	format();
	rewind(FILESTREAM);
	FS_jump(9);
	FS_putint(0xFFFF);
	FS_reset();
	int size = 32 * 512;
	printf("%d\n", add_to_file(&size));
	print_block(FILE_TABLE);
	char n[20];
	int i;
	for(i = 0; i < 20; i++){n[i] = i + 'A';}
	struct index_entry* new_file = (struct index_entry*)create_entry("names");
	printf("Name mod time: %d\n", new_file->last_mod_timestamp);
	rewind(FILESTREAM);
	FS_jump(0x200-1);
	FS_putint(0xAA);
	int open_location = find_open_entry(DIRECTORY_INDEX);
	write_entry(open_location, n);
	print_block(DIRECTORY_INDEX);
	find_entry(n, DIRECTORY_INDEX);
	close(FILESTREAM);
	return 0;
}
