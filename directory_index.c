#include "directory_index.h"



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

struct index_entry* find_entry(char* entry_name, int entry_point)
{
	//Find an entry from the directory index that contains the same name.
	struct index_entry* found_entry = (struct index_entry*) malloc(sizeof(struct index_entry));
	FS_reset();
	FS_jump(entry_point);
	char fileName[FILENAME_LENGTH]; int i;
	int read_entries = 0;
	while(read_entries++ < 20){
		for( i = 0; i < FILENAME_LENGTH; i++){
			fileName[i] = FS_getc();
			printf("%c", fileName[i]);
		}
		if(compare_names(fileName, entry_name) == 1){
			FS_jump(-12);
			populate_entry_struct(found_entry, FS_getpos());
			
			return found_entry;
		}
		FS_jump(9);
	}

	return NULL;

}


int update_index_entry(int entry_location, void* new_val, int ENTRY_ATTR)
{
	//Find which piece of information the user wants to change and change it
	// to the info pointed at by new_val.
	int bytesToWrite, i;
	char* byte_writer = (char*) new_val;
	switch(ENTRY_ATTR){
	case(ENTRY_NAME):
		bytesToWrite = 8;
		break;
	case(ENTRY_MOD_TIME):
		bytesToWrite = 2;
		FS_jump(12);
		break;
	case(ENTRY_MOD_DATE):
		bytesToWrite = 2;
		FS_jump(14);
		break;
	case(ENTRY_SIZE):
		bytesToWrite = 2;
		FS_jump(16);
		break;
	case(ENTRY_LOCATION):
		bytesToWrite = 2;
		FS_jump(18);
		break;
	default:
		return -1;
	}
	for(i = 0; i < bytesToWrite; i++){
		FS_putc((*byte_writer)++);
	}
	return 0;
}




int write_entry(int entry_point,  char* entry){
	//Writes an entry into the directory index beginning at entry_point
	int i;
	for(i = 0; i < 20; i++){
		FS_putc(entry[i]);
	}
	return 0;
}


