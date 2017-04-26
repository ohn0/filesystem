#include "filesystem.h"

FILE* FILESTREAM;
unsigned int fileSize;
unsigned int virtual_offset;
unsigned int DIRECTORY_INDEX;
struct directory_table* entries = NULL;
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
	format();
	FS_reset(); FS_putMiniInt(0xAAAA);FS_reset();
	create_root_dir();
	FS_reset();
	char v[1];
	v[0] = 'a';
//	print_block(0);
	close(FILESTREAM);

	return 0;

}


int die(){
	FILESTREAM = fopen("FS10MB", "r+b");
	FILE* test = fopen("test", "r");
	format();
	create_file_table();
	FS_reset(); FS_putMiniInt(0xAAAA); FS_reset();
	int i;
	for(i = 0; i < 1; i++){
		add_file("First", "FIRST.DIR", 5, ENTRY_TYPE_FILE);
	}
	struct index_entry*  entry = find_entry("FIRST.DIR", DIRECTORY_INDEX);
	char* words = read_file(entry);
	for(i = 0; i < entry->size; i++){
		printf("%c", words[i]);
	}
	char big_guy[1024];
	for(i = 0; i < 1024; i++){
		big_guy[i] = 'a';
	}
	write_file(big_guy, entry,20 );
	words = read_file(entry);
	for(i = 0; i < entry->size; i++){
		printf("%c", words[i]);
	}
	close(FILESTREAM);
	close(test);
	return 0;
}
