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
	// char n[5]= "names";
	struct index_entry* new_file = (struct index_entry*)create_entry("names");
	printf("Name mod time: %d\n", new_file->last_mod_timestamp);
	int open_location = find_open_entry(DIRECTORY_INDEX);
	printf("First open location: %d\n", open_location);
	close(FILESTREAM);
	return 0;
}
