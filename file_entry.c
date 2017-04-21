#include "file_table.h"
struct index_entry* create_entry(char* entry_name)
{
	//Create a file_entry struct that we can later use to write into the
	//directory index on the disk.
	struct index_entry* new_entry =  malloc(sizeof(struct index_entry));
	int i;
	for(i = 0; i < FILENAME_LENGTH; i++){
		new_entry->entry_name[i] = ' ';
	}
	for(i = 0; i < FILENAME_LENGTH && entry_name[i] != ' '; i++){
		new_entry->entry_name[i] = entry_name[i];
	}
	new_entry->last_mod_timestamp = time(NULL);
	printf("time size;%d\n", sizeof(new_entry->last_mod_timestamp));
	return (struct index_entry*) new_entry;
}

int populate_entry_struct(struct index_entry* entry, int entry_point){
	
	//Go to the entry located at entry_point and write all of its
	//information into the entry struct.
	int i;
	entry->entry_location = FS_getpos()+1;
	for(i = 0; i <= FILENAME_LENGTH; i++){
		entry->entry_name[i] = FS_getc();
	}
	entry->last_mod_timestamp = FS_getint();
	unsigned int size_loc = FS_getint();
	entry->start_block_location = (size_loc >> 16);
	entry->size = (size_loc << 16) >> 16;
//	entry->size = FS_getint();
//	entry->start_block_location = FS_getint();
	return 1;
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
	int size_loc = ~0;
	size_loc = size_loc & (new_entry->start_block_location << 16);
	size_loc = size_loc | (new_entry->size);
	FS_putint(size_loc);
	//	FS_putint(new_entry->size);
//	FS_putint(new_entry->start_block_location);
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
	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%c", new_entry->entry_name[i]);
	}
	int entry_location = find_open_entry(DIRECTORY_INDEX);
	populate_entry(new_entry, entry_location);
	for(i = 0; i < num_blocks; i++){
		FS_reset();
		FS_jump(data_blocks[i] * BLOCK_SIZE);
		if(datasize > BLOCK_SIZE){
			datasize-= BLOCK_SIZE;
			write_block(data, BLOCK_SIZE);
		}
		else{
			write_block(data, datasize);
		}
		data += BLOCK_SIZE;
	}
	return 0;
}

int write_block(char* data, int write_amount)
{
	int i;
	for(i = 0; i < write_amount; i++){
		FS_putc(data[i]);
	}
	return 1;
}

char* read_block(int entry_point, int read_count){
        //Read read_count bytes beginning at entry_point into read_buf and return read_buf.
	char* read_buf = (char*) malloc(sizeof(char) * read_count);
        FS_reset();
	FS_jump(entry_point);
	int i;
	for(i = 0; i < read_count; i++){
		read_buf[i] = FS_getc();
						        
	}
        return read_buf;
}

char* read_file(struct index_entry* entry){
	FS_reset();
	int start_block = entry->start_block_location;
	int size = entry->size;
	char* read_buf = (char*) malloc(size * sizeof(char));
	char* block_buf;
	int i, j, readI, blockI, readCount;
	int block_count = ceil(((double)size)/BLOCK_SIZE);
	FS_jump(2 * start_block);
	int block_chain[block_count];
	block_chain[0] = start_block;
	for(i = 1; i < block_count; i++){
		
		FS_reset();
		FS_jump(2 * block_chain[i - 1]);
		block_chain[i] = FS_getMiniInt();
	}
	readI = blockI = 0;
	for(i = 0; i < block_count; i++){

		if(size > BLOCK_SIZE){
			block_buf = read_block(BLOCK_SIZE * block_chain[i], BLOCK_SIZE);
			readCount = BLOCK_SIZE;
		}else{
			block_buf = read_block(BLOCK_SIZE * block_chain[i],size);
			readCount = size;
		}
		for(blockI = 0; blockI < readCount; blockI++){
			read_buf[readI++] = block_buf[blockI];
		}
		free(block_buf);
		size -= BLOCK_SIZE;
	}
	return read_buf;
}
