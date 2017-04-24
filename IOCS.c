#include "IOCS.h"
int create_root_dir()
{
	//Creates a root directory within the file system.
	FS_reset();
	add_file("hi :D", "ROOT.ROT", 5, ENTRY_TYPE_DIR);
//	struct index_entry* entry = find_entry("ROOT.ROT", DIRECTORY_INDEX);
//	char* buf = read_file(entry);
	int i;
	for(i = 0; i < 5; i++){
//		printf("%c", buf[i]);
	}
	printf("\n");
	return 0;
}

int generate_children(char* parentName)
{
	//Called when a folder is opened, a linked list is 
	//generated containing all the children of the folder.
	//The previous folder's children list is closed as well.
	FS_reset();
	struct index_entry* parent_dir = find_entry(parentName, DIRECTORY_INDEX);
	if(parent_dir == NULL){printf("Not a valid folder name.\n"); return -1;}
	char* children = read_file(parent_dir);
	return 0;

}

int generate_block_table()
{
	if(entries == NULL){
		entries = (struct directory_table*) malloc(sizeof(struct directory_table));
	}
	unsigned int local_dir = DIRECTORY_INDEX;
	struct directory_table* dir_entry = entries;
	int next_dir;
	do{
		FS_jump(local_dir);
		char index_count = get_index_count(local_dir);
		FS_jump(local_dir + NEXT_DIR_INDEX);
		if(index_count  == DIR_INDEX_FULL_VAL){
			next_dir = get_next_dir_index(local_dir);
		}
		else{
			next_dir = 0;
		}
	
		FS_reset(); FS_jump(local_dir);
		if(local_dir != DIRECTORY_INDEX){
	//		FS_jump(1);
		}
		while(index_count-- > 0){
			dir_entry->entry = (struct index_entry*) malloc(sizeof(struct index_entry));
			populate_entry_struct(dir_entry->entry, FS_getpos());
			FS_jump(-1);
			dir_entry->next = (struct directory_table*) malloc(sizeof(struct directory_table));
			dir_entry = dir_entry->next;	
			dir_entry->next = NULL;
		}
		local_dir = next_dir;
	}while(next_dir != 0);	
	return 0;
}

int create_file(char* name, int size, void* buf, struct directory_table* parent, int entry_type)
{
//	add_file(parent->childList, add_file(buf, name, size, entry_type);
	return 0;
}


int open_root()
{
	FS_reset();
//	struct index_entry* root = find_entry("ROOT", DIRECTORY_INDEX);
	
}

int* open_dir(struct index_entry* dir)
{
	FS_reset();
	FS_jump(dir->start_block_location);
	int* child = (int*) malloc(25 * sizeof(char));
		
}

int write_to_file(void* data, int data_size, struct children* child)
{
	int new_fileSize;
	if(data_size < child->data->size){
		new_fileSize = data_size;
	}
	else{
		new_fileSize = data_size + child->data->size;
	}
	char* child_buf = child->file_buf;
	char* updated_buf = (char*) malloc(new_fileSize * sizeof(char));
	int i;
	for(i = 0; i < new_fileSize; i++){
		if(new_fileSize < child->data->size){
			updated_buf[i] = data[i];
		}
		else{
			if(i < child->data->size){
				updated_buf[i] = child->file_buf[i];
			}else{
				updated_buf[i] = data[i - child->data->size];
			}
		}
	}


}
