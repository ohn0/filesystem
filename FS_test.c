#include "filesystem.h"

FILE* FILESTREAM;
unsigned int fileSize;
unsigned int virtual_offset;
unsigned int DIRECTORY_INDEX;
struct directory_table* entries = NULL;
struct open_dir* root_dir;
struct index_entry* ROOT = NULL;
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
	int c, i;
	i = 0;
	i = c = 0;
	FS_reset(); FS_putMiniInt(0xAAAA);FS_reset();
	make_root();
	
	char* t1 = "Text file in root directory";
	char* t2 = "Text file in sub-directory under root.";
	struct index_entry* text_file = add_file(t1, "text1.txt", strlen(t1), ENTRY_TYPE_FILE);
	addFile(text_file);
	display_contents();
	struct index_entry* sub_dir = add_file("DIR_", "sub.DIR", 4, ENTRY_TYPE_DIR);
	open_dir("sub.DIR");
	struct index_entry* text_fileB = add_file(t2, "text2.txt", strlen(t2), ENTRY_TYPE_FILE);
	addFile(text_fileB);
	display_contents();
	delete_file("text2.txt");
	display_contents();
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
