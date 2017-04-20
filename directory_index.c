#include "directory_index.h"
struct index_entry* create_entry(char* entry_name)
{
	//Create a file_entry struct that we can later use to write into the
	//directory index on the disk.
	struct index_entry* new_entry =  malloc(sizeof(struct index_entry));
	int i;
	for(i = 0; i < FILENAME_LENGTH; i++){
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
	//Find an entry from the directory index that contains the same name.
	FS_reset();
	FS_jump(entry_point);
	char fileName[8]; int i;
	for( i = 0; i < 8; i++){
		fileName[i] = FS_getc();
		printf("%X ", fileName[i]);
	}
	FS_jump(12);
	printf("\n%X\n", FS_getc());

}

int populate_entry(struct index_entry* new_entry, int entry_location)
{
	//Take the contents of new_entry and write them to the disk
	//beginning at entry_location.
	FS_reset();
	FS_jump(entry_location);
	int i;
	for(i = 0; i < FILENAME_LENGTH; i++){
		FS_putc(new_entry->entry_name[i]);
	}
	FS_putint(new_entry->last_mod_timestamp);
	FS_putint(new_entry->size);
	FS_putint(new_entry->start_block_location);
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

int set_filename(struct index_entry* entry, char* name)
{
	//Change the filename of the given entry
	int i;
	if(entry == NULL){return -1;}
	for(i = 0; i < FILENAME_LENGTH; i++){
		entry->entry_name[i] = name[i];
	}
	return 0;
}

int add_file(void* data, char* filename, int datasize)
{
	//Allocate enough blocks to accomodate data's size.
	// populate the entry in the directory index
	//Write the contents of data into the blocks
	if(data == NULL){return -1;}
	FS_reset();
	int* data_blocks = add_to_file(&datasize);
	int num_blocks = ceil(((double)datasize)/BLOCK_SIZE);
	int i;
	struct index_entry* new_entry = create_entry(filename);
	new_entry->size = datasize;
	new_entry->start_block_location = data_blocks[0];
	int entry_location = find_open_entry(DIRECTORY_INDEX);
	populate_entry(new_entry, entry_location);
	for(i = 0; i < num_blocks; i++){
		FS_reset();
		FS_jump(data_blocks[i] * BLOCK_SIZE);
		write_block(data);
		data += BLOCK_SIZE;
	}
	
}


int write_entry(int entry_point,  char* entry){
	//Writes an entry into the directory index beginning at entry_point
	int i;
	for(i = 0; i < 20; i++){
		printf("%X", entry[i]);
		FS_putc(entry[i]);
	}
	printf("\n");
	return 0;
}

int write_block(char* data)
{
	int i;
	for(i = 0; i < BLOCK_SIZE; i++){
		FS_putc(data[i]);
	}
	return 1;

}

char* read_block(int entry_point, int read_count)
{
	//Read read_count bytes beginning at entry_point into read_buf and return read_buf.
	char* read_buf = (char*) malloc(sizeof(char) * read_count);
	FS_reset();
	FS_jump(entry_point);
	int i;
	for(i = 0; i < BLOCK_SIZE; i++){
		read_buf[i] = FS_getc();
	}
	return read_buf;
}

int read_file(struct index_entry* entry){
	FS_reset();
	int start_block = entry->start_block_location;
	int size = entry->size;
	char read_buf[size];
	char* block_buf;
	int i, j;
	int block_count = ceil(((double)size)/BLOCK_SIZE);
	FS_jump(2 * start_block);
	int block_chain[block_count];
	for(i = 0; i < block_count; i++){
		block_chain[i] = FS_getint();
	}
	for(i = 0; i < block_count; i++){
		block_buf = read_block(BLOCK_SIZE * block_chain[i]);
		size-=BLOCK_SIZE;
		if(size < BLOCK_SIZE){
			while(j % BLOCK_SIZE <= BLOCK_SIZE - 1){
				read_bu;
			}
		}	
	}
}
