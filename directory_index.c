#include "directory_index.h"
struct index_entry* create_entry(char* entry_name)
{
	struct index_entry* new_entry =  malloc(sizeof(struct index_entry));
	int i;
	for(i = 0; i < FILE_NAME_LENGTH; i++){
		new_entry->entry_name[i] = entry_name[i];
	}
	new_entry->last_mod_timestamp = time(NULL);
	return (struct index_entry*) new_entry;
}

int find_open_entry(int entry_point)
{
//Returns the location of the first zero entry within the directory index.
	FS_reset();
	fseek(FILESTREAM, entry_point, SEEK_SET);
	printf("%d\n", ftell(FILESTREAM));
	int index_counter = 0;
	char c = FS_peek(ftell(FILESTREAM));
	while(c != 0x00 && c != 0xFF){
		FS_jump(20);
		c = FS_peek(ftell(FILESTREAM));
		if(index_counter == 25){
		}
	}
	return FS_getpos();
}

int find_entry(char* entry_name, int entry_point)
{
	FS_reset();
	FS_jump(entry_point);
//	FS_jump(21);
	char fileName[8]; int i;
	for( i = 0; i < 8; i++){
		fileName[i] = FS_getc();
		printf("%X ", fileName[i]);
	}
	FS_jump(12);
	printf("\n%X\n", FS_getc());

}
int update_index_entry(int new

int write_entry(int entry_point,  char* entry){
	int i;
	for(i = 0; i < 20; i++){
		printf("%X", entry[i]);
		FS_putc(entry[i]);
	}
	printf("\n");
	return 0;
}
