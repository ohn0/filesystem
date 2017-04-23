#include "directory_index.h"
int find_open_entry(int entry_point)
{
	//Recursively traverse the directory index until a zero entry location is obtained.
	FS_reset();
	fseek(FILESTREAM, entry_point, SEEK_SET);
	if(get_index_count(entry_point) == DIR_INDEX_FULL_VAL){
		return find_open_entry(get_next_dir_index(entry_point));
	}
	else{
		int index_position;
		update_index_count(entry_point, ADD);
		FS_reset(); FS_jump(entry_point);
		printf("%d\n", ftell(FILESTREAM));
		char c = FS_peek(ftell(FILESTREAM));
		while(c != 0x00){
			FS_jump(20);
			c = FS_peek(ftell(FILESTREAM));
		}
		index_position = FS_getpos();
		if(get_index_count(entry_point) == DIR_INDEX_FULL_VAL){
			extend_directory_index(entry_point);
		}
		return index_position;
	}
}

struct index_entry* find_entry(char* entry_name, int entry_point)
{
	//Find an entry from the directory index that contains the same name.

	unsigned int next_dir = get_next_dir_index(entry_point);
	if(entry_exists(entry_name, entry_point)){
		struct index_entry* found_entry = (struct index_entry*) malloc(sizeof(struct index_entry));
		populate_entry_struct(found_entry, FS_getpos());
		return found_entry;
	}
	if(next_dir == 0x00){
		return NULL;
	}
	else{
		return find_entry(entry_name, next_dir);
	}
}

int entry_exists(char* name, int entry_point)
{
	FS_reset();
	FS_jump(entry_point);
	int read_entries = 0;
	int i;
	char fileName[FILENAME_LENGTH];
	while(read_entries++ < 25){
		printf("Checking entry.\n");
		for( i = 0; i < FILENAME_LENGTH; i++ ){
			fileName[i] = FS_getc();
			printf("%c", fileName[i]);
		}
		printf("\n");
		if(compare_names(name, fileName) == 1){
			FS_jump(-12);
			return 1;
		}
		FS_jump(9);
	}
	return 0;	
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

int update_index_count(int entry_point, unsigned int val)
{
	int new_val = 0;
	if(val == ADD){new_val = 1;}
	else{new_val = -1;}
	FS_reset();
	FS_jump(entry_point + 500);
	//printf("entry count: %d\n", FS_peek(FS_getpos())+ new_val);
	FS_putc(FS_peek(FS_getpos())+new_val);
	return 0;
}


unsigned char get_index_count(int entry_point)
{
	FS_reset();
	FS_jump(entry_point + 500);
	return FS_peek(FS_getpos());
}

unsigned int get_next_dir_index(int entry_point)
{
	FS_reset();
	FS_jump(entry_point + 505);
	return FS_getMiniInt();
}

