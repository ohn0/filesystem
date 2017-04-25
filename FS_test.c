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
int main(int argc, char const *argv[]){
	FILESTREAM = fopen("FS10MB", "r+b");
	format();
	create_root_dir();
	FS_reset(); FS_putMiniInt(0xAAAA); FS_reset();
	FS_reset();
	char v[1];
	v[0] = 'a';
	print_block(0);
	close(FILESTREAM);

	return 0;

}
int die()
{
	FILESTREAM = fopen("FS10MB", "r+b");
	FILE* test = fopen("test", "r");
	FILE* retrieval = fopen("retrieval.png", "w+b");
	format();
	create_file_table();
	create_root_dir();
	char max_file[8 * 4096];
	int testSize = 0; int i = 0;
	for(i = 0; i < 8 * 4096; i++){
		max_file[i] = 0xFF;
	}
	while(fgetc(test) != EOF){testSize++;}
	i = 0;
	char* data_buf = (char*) malloc(testSize * sizeof(char));
	rewind(test);
	while((data_buf[i++] = fgetc(test)) != EOF){}
	FS_reset();
	FS_putMiniInt(0xAAAA);
	for( i= 0; i < 160; i++ ){
		add_file(data_buf, "long.z0x", testSize, ENTRY_TYPE_FILE);
	}
	add_file(max_file, "big.q3w", 8 * 4096, ENTRY_TYPE_DIR);
	FS_reset();
	struct index_entry* entry = find_entry("long.z0x", DIRECTORY_INDEX);
//	entry = find_entry("big.q3w", DIRECTORY_INDEX);
//	printf("%d\n%d\n%d\n%X\n", entry->last_mod_timestamp, entry->start_block_location, entry->size, entry->entry_location );
//	printf("----\n");
//	write_file(max_file, entry, 8 * 4096);
	entry = find_entry("ROOT.ROT", DIRECTORY_INDEX);
//	write_file("FF0F", entry, 4);
	char* buf;// = read_file(entry);
	buf = read_file(entry);
	char concatBuf[40];
	char* appendBuf = "FF0F";
	for(i = 0; i < 9; i++){
		if(i < entry->size){
			concatBuf[i] = buf[i];
		}
		else{
			concatBuf[i] = appendBuf[i - entry->size]; 
		}
	}
	int bufSize = 0;// = entry->size;
	for(i = 0; i < 8 * 4096; i++){
		max_file[i] = 0x01;
	}
	write_file(concatBuf, entry, 9);
	generate_block_table();
	struct directory_table* entries_head = entries;
	while(entries->next != NULL){
		for(i = 0; i < FILENAME_LENGTH; i++){
			printf("%c", entries->entry->entry_name[i]);
		}
		printf("\n");
		entries = entries->next;
	}
	entries = entries_head;
	FS_reset();
	FS_jump((1 + virtual_offset) * BLOCK_SIZE);
	for(i = 0; i < 100; i++){
	//	printf("%c ", buf[i]);
	}
	close(FILESTREAM);
	close(test);
	return 0;
}
