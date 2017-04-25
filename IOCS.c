#include "IOCS.h"
int create_root_dir()
{
	//Creates a root directory within the file system.
	FS_reset();
	struct open_dir* root_dir = (struct open_dir*) malloc(sizeof(struct open_dir));
	root_dir->dir_entry = add_file("ROOT    DIR", "ROOT.DIR", FILENAME_LENGTH, ENTRY_TYPE_DIR);
	root_dir->parent = NULL;
	root_dir->child_list = NULL;
	root_dir->child_count = 0;	
//	struct index_entry* entry = find_entry("ROOT.ROT", DIRECTORY_INDEX);
//	char* buf = read_file(entry);
	create_file("FIRST.DIR", FILENAME_LENGTH, root_dir->dir_entry->entry_name,root_dir, ENTRY_TYPE_DIR);
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

int create_file(char* name, int size, void* buf, struct open_dir* parent, int entry_type)
{
	struct index_entry* new_file = add_file(buf, name, size, entry_type);
	add_file_to_parent_dir(new_file, parent);
	return 0;
}

int add_file_to_parent_dir(struct index_entry* new_file, struct open_dir* parent)
{
	struct children* child_iterator = parent->child_list;	
	while(child_iterator != NULL){
		child_iterator = child_iterator->next_child;
	}
	parent->child_count++;
	child_iterator = (struct children*) malloc(sizeof(struct children));
	child_iterator->next_child = NULL;
	child_iterator->parent = parent;
	child_iterator->data = new_file;
	child_iterator->file_buf = read_file(new_file);
	update_dir_on_disk(parent);
	return 0;
}

int update_dir_on_disk(struct open_dir* parent)
{
	struct children* child_ite = parent->child_list;
	int child_count = 0;
	while(child_ite != NULL){
		child_count++;
		child_ite = child_ite->next_child;
	}
	child_ite = parent->child_list;
	int i, j;
	j = 0;
	char* file_buf = (char*) malloc(sizeof(char) * (FILENAME_LENGTH * child_count));
	while(child_ite != NULL){
		for(i = 0; i < FILENAME_LENGTH; i++){
			file_buf[j++] = child_ite->data->entry_name[i];
		}
		child_ite = child_ite->next_child;
	}
	write_file(file_buf, parent->dir_entry, FILENAME_LENGTH * child_count);
	return 0;
}


struct open_dir* open_directory(char* name, struct open_dir* parent)
{
	FS_reset();
	struct index_entry* dir_entry = find_entry(name,  DIRECTORY_INDEX);
	printf("Displaying root's contents:\n");
	//display_dir_entries(root);
	struct open_dir* new_dir = (struct open_dir*) malloc(sizeof(struct open_dir));
	new_dir->parent = parent;
	new_dir->dir_entry = dir_entry;
	parent->child_list = NULL;
//	update_dir_contents(new_dir);
}

int update_dir(struct open_dir* dir)
{
//	reload_children();

}

int* open_dir(struct index_entry* dir)
{
	FS_reset();
	FS_jump(dir->start_block_location);
	int* child = (int*) malloc(25 * sizeof(char));
		
}

int write_to_file(char* data, int data_size, struct children* child)
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
	free(child->file_buf);
	child->file_buf = updated_buf;
	return 0;


}
