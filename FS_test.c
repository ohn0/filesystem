#include "filesystem.h"

FILE* FILESTREAM;
unsigned int fileSize;
unsigned int virtual_offset;
unsigned int DIRECTORY_INDEX;
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
	FILE* cent = fopen("50_cent.png", "r");
	FILE* retrieval = fopen("retrieval.png", "a+b");
	fseek(cent, 0L, SEEK_END);
	int cent_size = ftell(cent);
	char cent_buf[cent_size];
	format();
	create_file_table();
	rewind(cent);
	char max_file[8 * 4096];
	int testSize = 0; int i = 0;
	for(i = 0; i < 8 * 4096; i++){
		max_file[i] = 0xFF;
	}
	char* sinclair  = "sinclair";
	i = 0;
	while(fgetc(test) != EOF){testSize++;}
	while(i < cent_size){
		cent_buf[i++] = fgetc(cent);
	}
	i = 0;
	printf("%d\n", testSize);
	char* data_buf = (char*) malloc(testSize * sizeof(char));
	rewind(test);
	while((data_buf[i++] = fgetc(test)) != EOF){}
	FS_reset();
	FS_putMiniInt(0xAAAA);
	FS_reset();
	for( i= 0; i < 25; i++ ){
		add_file(data_buf, "test_longname", testSize);
	}
	add_file(max_file, "really_big_file", 8 * 4096);
	add_file(cent_buf, "fifty_cent", cent_size);
	FS_reset();
	struct index_entry* entry = find_entry("test_longname", DIRECTORY_INDEX);
//	printf("%d\n%d\n%d\n%d\n", entry->last_mod_timestamp, entry->start_block_location, entry->size, entry->entry_location );
	if(entry == NULL){printf("null entry\n"); return 0;}
//	printf("%X\n", entry->last_mod_timestamp);
	FS_reset();

	entry = find_entry("really_big_file", DIRECTORY_INDEX);
	write_file(sinclair, entry, 9);
	char* buf = read_file(entry);
	int bufSize = entry->size;
	for(i = 0; i < 8 * 4096; i++){
		max_file[i] = 0x01;
	}
	write_file(max_file, entry, 8 * 4096);
//	buf = read_file(entry);
	for(i = 0; i<bufSize; i++){
		printf("%c",buf[i]);
	}
//	delete_entry("really_big_file");
//	entry = find_entry("really_big_file", DIRECTORY_INDEX);
//	delete_entry(entry);
	//	print_block(DIRECTORY_INDEX);
	entry = find_entry("fifty_cent", DIRECTORY_INDEX);
	buf = read_file(entry);
	for(i = 0; i < cent_size; i++){
		fputc(buf[i], retrieval);
	}
	close(retrieval);
	close(cent);
	close(FILESTREAM);
	close(test);
	return 0;
}
