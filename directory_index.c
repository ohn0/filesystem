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
//	FILESTREAM = entry_point;
	int index_counter = 0;
	char c = FS_getc();
	while(c != 0x00 && c != 0xFF){
		printf("%X ",c);
		if(index_counter++ % 20 == 0){
			printf("\n");}
		c = FS_getc();
		FS_jump(20);
		if(index_counter == 25){
		}
	}
	printf("%X ", c);

	return FS_getpos();
	
}
