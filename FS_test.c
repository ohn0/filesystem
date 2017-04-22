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
	FILESTREAM = fopen("FS10MB", "r+b");
	FILE* test = fopen("test", "r");
	char max_file[8 * 4096];
	int testSize = 0; int i = 0;
	for(i = 0; i < 8 * 4096; i++){
		max_file[i] = 0xFF;
	}
	i = 0;
	while(fgetc(test) != EOF){testSize++;}
	printf("%d\n", testSize);
	char* data_buf = (char*) malloc(testSize * sizeof(char));
	rewind(test);
	while((data_buf[i++] = fgetc(test)) != EOF){}
	format();
	FS_reset();
	FS_putMiniInt(0xBBBB);
	FS_putMiniInt(0xAAAA);
	FS_jump(8);
	FS_putMiniInt(0xABCD);
	FS_reset();
	add_file(data_buf, "test_longname", testSize);
	add_file(max_file, "really_big_file", 8 * 4096);
	FS_reset();
	struct index_entry* entry = find_entry("test_longname", DIRECTORY_INDEX);
//	printf("%d\n%d\n%d\n%d\n", entry->last_mod_timestamp, entry->start_block_location, entry->size, entry->entry_location );
	if(entry == NULL){printf("null entry\n"); return 0;}
//	printf("%X\n", entry->last_mod_timestamp);
	FS_reset();
	char* buf = read_file(entry);
	for(i = 0; i < entry->size; i++){
	//	printf("%c",buf[i]);
	}
	delete_entry(entry);
	entry = find_entry("really_big_file", DIRECTORY_INDEX);
	delete_entry(entry);
	//	print_block(DIRECTORY_INDEX);
	close(FILESTREAM);
	close(test);
	return 0;
}
