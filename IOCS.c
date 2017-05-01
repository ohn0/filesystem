#include "IOCS.h"


int make_root()
{
	ROOT = add_file("ROOT", "ROOT.DIR", 4, ENTRY_TYPE_DIR);
	return 0;
}

int addFile(struct index_entry* new_file)
{
	int new_location =new_file->entry_location; //BLOCK_SIZE*(virtual_offset + new_file->entry_location);
	char* buf = read_file(ROOT, DIRECTORY_INDEX);
	int zeros = 0;

	int i;
	for(i = 0; i != ROOT->size; i++){
		if(buf[i] == 0){
			zeros++;
		}
	}
	unsigned char* new_buf = (char*)malloc(sizeof(char)*(2 + (ROOT->size - zeros)));
	int j = 0;
	for(i = 0; i != ROOT->size; i++){
		if(buf[i] != 0){
			new_buf[j++] = buf[i];
		}
	}
	printf("%.4X\n", new_location);
	unsigned char MSB = new_location >> 8;
	unsigned char LSB = new_location;
	printf("%.2X\n%.2X\n", MSB, LSB);
	new_buf[j] = MSB;
	new_buf[j+1] = LSB;
	printf("NEW ROOT SIZE:-----------------------------\n");
	for(i = 0; i < ROOT->size; i++){
		printf("%X ", new_buf[i]);
	}

	FS_reset();
	//FS_jump(BLOCK_SIZE * (virtual_offset + ROOT->start_block_location));
//	while(FS_getc() != 0x00){
		;
//	}

	write_file(new_buf, ROOT, j+2);
	//ROOT->size = ROOT->size + 2;
	//	FS_jump(-1);
//	FS_putMiniInt(new_location);
}

int delete_file(char* file_name){
	struct index_entry* dFile = find_entry(file_name, DIRECTORY_INDEX);
	unsigned char MSB = (dFile->entry_location) >> 8;
	unsigned char LSB = (dFile->entry_location);
	printf("VALUES :%X %X\n", MSB, LSB);
	delete_entry(file_name);
	unsigned char* buf = read_file(ROOT);
	int zeros = 0;
	int i;
	for(i = 0; i != ROOT->size; i++){
		if(buf[i] == 0){
			zeros++;
		}
	}
	//unsigned char* new_buf = (char*)malloc(sizeof(char)*((ROOT->size - 2)));
	int j = 0;
	printf("Printing contents--------------------------------------\n");
	for(i = 0; i != ROOT->size; i++){
		printf("%X ", buf[i]);
		if(buf[i] == MSB && buf[i+1] == LSB){
			printf("found the file.\n");
			buf[i] = 0; buf[i+1] = 0;
			break;
		}
	}
	write_file(buf, ROOT, ROOT->size);
	printf("ROOT SIZE:%d\n", ROOT->size);

}

int display_contents()
{
	struct index_entry* r_file;
	unsigned char* contents = read_file(ROOT);
	int i = 4;
	unsigned char MSB, LSB;
	unsigned int content_location;
	printf("ROOT SIZE:%d\n", ROOT->size);
	if(ROOT->size == 4){
		printf("No contents within current directory.\n");
		return 0;
	}
	while(i < ROOT->size){
		printf("%X", contents[i]);
		if(contents[i] != 0 && contents[i+1] != 0){
			MSB = contents[i];
			LSB = contents[i+1];
			printf("Hex locations:\n%X %X\n", contents[i], contents[i+1]);
			content_location = (256 * MSB) + LSB;
			FS_reset();
			FS_jump(content_location);
			char buf[FILENAME_LENGTH];
			int j;
			printf("Printing directory contents:\n");
			for(j = 0; j < FILENAME_LENGTH; j++){
				buf[j] = FS_getc();
		//		printf("%c", buf[j]);
			}
			char* cName = format_file_name(buf);
			r_file = find_entry(cName, DIRECTORY_INDEX);
			free(cName);
			char* read_buf = read_file(r_file);
			printf("\nReading file:\n");
			int k;
			for(k = 0;k < r_file->size; k++){
				printf("%c", read_buf[k]);
			}
		}
		printf("\n");
		i+=2;
	}


}


int open_dir(char* dir_name){
	ROOT = find_entry(dir_name, DIRECTORY_INDEX);
	display_contents();
}

int print_contents(){
	char* buf = read_file(ROOT);

}

int create_root_dir()
{
	//Creates a root directory within the file system.
	FS_reset();
	root_dir = (struct open_dir*) malloc(sizeof(struct open_dir));
	root_dir->dir_entry = add_file("ROOT    DIR", "ROOT.DIR", FILENAME_LENGTH, ENTRY_TYPE_DIR);
	root_dir->parent = NULL;
	root_dir->child_list = NULL;
	
	return 0;
}

int del_file(struct children* delFile){
	delFile->deleted = 1;

	return 0;
}

int clear_dir(struct open_dir* parent, char* dir_name)
{
	char* fName = format_file_name(dir_name);
	
	struct index_entry* entry = find_entry(fName, DIRECTORY_INDEX);
	char* buf = read_file(entry);
	int i;
	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%c", fName[i]);
	}
	printf("\n");
	printf("DIRECTORIES TO DELETE:\n");
	for(i = FILENAME_LENGTH; buf[i] != -1; i++){
		if(i % FILENAME_LENGTH == 0 && i < entry->size){
			//struct index_entry* IZ = find_entry(buf[i], DIRECTORY_INDEX);
			char* fName = format_file_name(buf[i]);
			delete_entry(fName);
		}
		printf("%c", buf[i]);
	}
	return 0;
}

int cleanup_parent(struct open_dir* parent){
	if(parent == NULL){return 0;}
	else{
		struct children* child = parent->child_list;
		while(child != NULL){
			if(child->data->entry_type == 'D'){
				if(child->deleted == 1){
					clear_dir(NULL, child->data->entry_name);
				}
			}
			else{
				if(child->deleted == 1){
					char* fName = format_file_name(child->data->entry_name);
					delete_entry(fName);
					free(fName);
				}

			}
			child = child->next_child;
		}
		//clear_dir(parent->dir_entry->entry_name, );
	}
	return 0;
}


struct open_dir* openDir(struct open_dir* parent, char* dir_name)
{
	//Free the parent open_dir
	//Create a pointer to all of the new dir's contents
	//and return it.
	struct children* child;
	if(parent != NULL){
		child = parent->child_list;
	}else{
		child = NULL;
	}

	int i; char new_name[FILENAME_LENGTH];
	if(child == NULL){printf("No contents in current dir.\n");}
	while(child != NULL && compare_names(dir_name, child->data->entry_name) == 0){

		child = child->next_child;
	}
	if(child== NULL){
		printf("Dir does not exist here.\n");
		return 0;
	}

	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%c", child->data->entry_name[i]);
		new_name[i] = child->data->entry_name[i];
	}
	char* file_name = format_file_name(new_name);
	struct index_entry* entry = find_entry(file_name, DIRECTORY_INDEX);
	printf("stuff going here\n");
	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%c", entry->entry_name[i]);
	}
	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%c", file_name[i]);
	}
	printf("\n");
	char* buf = read_file(entry);
	printf("Printing contents:\n");
	for(i = 0; i < entry->size; i++){
		printf("%c\n", buf[i]);}
//	cleanup_parent(parent);
	struct open_dir* new_dir = (struct open_dir*) malloc(sizeof(struct open_dir));
	new_dir->dir_entry = entry;
	new_dir->parent = parent;
	new_dir->child_list = (struct children*) malloc(sizeof(struct children));
	new_dir->child_list->next_child = NULL;
	new_dir->child_list->deleted = 0;
	create_children(new_dir, buf);
	for(i = 0; i < new_dir->child_list->data->size; i++){
		printf("%c", new_dir->child_list->file_buf[i]);
	}
	return new_dir;
}

struct children* open_file(struct open_dir* parent, char* file_name)
{
	struct children* file = find_child(parent, file_name);
	if(file->data->entry_type == 'D'){
		return NULL;
	}else{
		return file;
	}
}

int save_file(struct children* file, char* write_contents, int WRITE_TYPE)
{
	int new_size = 0;
	int i;
	char* new_contents = NULL;
	if(WRITE_TYPE == APPEND){
		int c = 0;;
		while(write_contents[c++] != -1){
			new_size++;
		}
		new_size+=file->data->size;
		char* new_contents = (char*) malloc(sizeof(char) * new_size);
		for(i = 0; i < new_size; i++){
			if(i < file->data->size){
				new_contents[i] = file->file_buf[i];
			}else{
				new_contents[i] = write_contents[i];
			}
		}
		free(file->file_buf);
		file->file_buf = new_contents;
	}
	else{
		while(write_contents[new_size] != -1){
			new_size++;
		}
		char* new_contents = (char*) malloc(sizeof(char) * new_size);
		for(i = 0; i < new_size; i++){
			new_contents[i] = write_contents[i];
		}
		free(file->file_buf);
		file->file_buf = new_contents;
	}
	write_file(new_contents, file->data, new_size);
	return 0;
}

int create_children(struct open_dir* parent, char* child_names)
{
	int i;
	struct children* current_child = NULL;
	struct index_entry* child_data = NULL;
	char* childName;
	int first_child = 1;

	for(i = 0; child_names[i] != -1; i++){
		if(i % FILENAME_LENGTH == 0 && child_names[i] != 0){
			printf("\n NEW CHILD ENTRY\n");
			int j;
			for(j = 0; j < FILENAME_LENGTH; j++){
				printf("%X ", child_names[i+j]);
			}
			current_child = (struct children*)malloc(sizeof(struct children));
			if(first_child){
				parent->child_list = current_child;
				first_child = 0;
			}
			childName = format_file_name(&child_names[i]);
			child_data = find_entry(childName, DIRECTORY_INDEX);
			printf("Printing child's entry type:%c\n", child_data->entry_type);
			current_child->next_child = NULL;
			current_child->parent=parent;
			current_child->data = child_data;
			current_child->file_buf = read_file(child_data);
			current_child->deleted = 0;
			current_child = current_child->next_child;
			free(childName);
		}	
	}
	return 0;
}

struct children* find_child(struct open_dir* parent, char* child_name)
{
	//char* name = format_file_name(child_name);
	char* name = child_name;
	struct children* current_child = parent->child_list;
	while(current_child != NULL){
		if(compare_names(name, current_child->data->entry_name) == 1){
		//	free(name);
			printf("Match found!\n");
			printf("%c", current_child->data->entry_type);
			int i;
			for(i = 0; i < FILENAME_LENGTH; i++){
				printf("%c", current_child->data->entry_name[i]);
			}
			return current_child;
		}else{
			current_child = current_child->next_child;
		}
	}
	return NULL;
}





int create_file(char* name, int size, void* buf, struct open_dir* parent, int entry_type)
{
	struct index_entry* new_file = add_file(buf, name, size, entry_type);
	return 0;
	printf("New_FILE's entry type:%c\n", new_file->entry_type);
	add_file_to_parent_dir(new_file, parent);
	return 0;
}

int deleteFile(struct open_dir* parent, char* name)
{
	struct children* delete_file = find_child(parent, name);
	//populate_entry_struct(delete_file->data, delete_file->data->entry_location);
	printf("\nDIR TYPE:%c\n", delete_file->data->entry_type);
	if(delete_file != NULL && delete_file->data->entry_type == 'F'){
		//char* fName = format_file_name(name);


		struct children* prev_child = NULL;
		struct children* current_child = parent->child_list;
		int i;

		while(compare_names(name, current_child->data->entry_name) != 1){
			prev_child = current_child;
			current_child = current_child->next_child;
		}
		prev_child->next_child = current_child->next_child;
		free(current_child->data);
		free(current_child->file_buf);
		free(current_child);
		delete_entry(name);
		return 0;
	}else if(delete_file->data->entry_type == 'D'){
		delete_dir(delete_file);
		return 0;
	}
}

int delete_dir(struct children* child)
{
	struct children* cIte;
	struct open_dir* dDir = (struct open_dir*) malloc(sizeof(struct open_dir));
	printf("dDIR SIZE: %X", child->data->size);
	dDir->child_list = NULL; //(struct children* )malloc(sizeof(struct children));
	dDir->dir_entry = child->data;
	char* child_names = read_file(child->data, DIRECTORY_INDEX);
	int i;
	printf("Valid printing\n");
	for(i = 0; child_names[i] != -1; i++){
		printf("%X ", child_names[i]);
	}
	create_children(dDir, child_names);
	cIte = dDir->child_list;
	cIte = cIte->next_child;
	while(cIte != NULL){
		int i;
		printf("CHILDREN :DDD\n");
		for(i = 0; i < FILENAME_LENGTH; i++){
			printf("%c", cIte->data->entry_name[i]);
		}
		cIte= cIte->next_child;

	}
//	if(cIte != NULL){
//		cIte = cIte->next_child;
//	}else{
//		printf("WHY ARE YOU NULL?\n");
//	}
//		printf("You will see this.\n");
	while(cIte != NULL){
		printf("You won't see this.\n");
		if(cIte->data->entry_type == 'F'){
			char* fName = format_file_name(cIte->data->entry_name);
			deleteFile(dDir, fName);
			free(fName);
		}else{
			//delete_dir(cIte);
		}
		cIte = cIte->next_child;
	}
	update_dir_on_disk(dDir);
	char* tName = format_file_name(child->data->entry_name);
	delete_entry(tName);
	free(tName);
}

int add_file_to_parent_dir(struct index_entry* new_file, struct open_dir* parent)
{
	struct children* child_iterator = parent->child_list;	
	struct children* cPrev=NULL;
	while(child_iterator != NULL){
		printf("GOing to next child..\n");
		cPrev = child_iterator;
		child_iterator = child_iterator->next_child;
	}
	//parent->child_count++;
	child_iterator = (struct children*) malloc(sizeof(struct children));
	child_iterator->next_child = NULL;
	child_iterator->parent = parent;
	child_iterator->data = new_file;
	child_iterator->deleted = 0;
	printf("Child sizeX\n", child_iterator->data->size);
	if(parent->child_list == NULL){
		parent->child_list = child_iterator;
	}
	if(child_iterator != NULL){
		if(new_file == NULL){
			printf("uh oh");
		}else{
			child_iterator->file_buf = read_file(new_file);
		}
	}
	if(cPrev != NULL){
		cPrev->next_child = child_iterator;
	}
	update_dir_on_disk(parent);
	return 0;
}

int update_dir_on_disk(struct open_dir* parent)
{
	struct children* child_ite = parent->child_list;
	int child_count = 0;
	while(child_ite != NULL){
		if(child_ite->deleted == 0){
			child_count++;
		}

		child_ite = child_ite->next_child;
	}
	child_ite = parent->child_list;
	int i, j;
	j = 0;
	printf("COUNTER:%d", child_count);
	char* file_buf = (char*) malloc(sizeof(char)+ (sizeof(char) * (FILENAME_LENGTH * child_count)));
	printf("printing chidlren.\n");
	while(child_ite != NULL){
		if(child_ite->deleted == 0){
			for(i = 0; i < FILENAME_LENGTH; i++){
				file_buf[j++] = child_ite->data->entry_name[i];
			}
		}else{
			//child_count--;
		}
		child_ite = child_ite->next_child;
	}
	printf("File_buf's values:\n");
	for(i = 0; i < FILENAME_LENGTH * child_count; i++){
		printf("%c\n", file_buf[i]);
	}
	file_buf[i] = -1; 
	printf("Children:%X\n", child_count);
	printf("\n%X",FILENAME_LENGTH * child_count);
	//-1 at the end to indicate EOF.
	
	printf("%c",parent->dir_entry->entry_name[0] );
	parent->dir_entry->size = 1+(FILENAME_LENGTH * child_count);
	//parent->dir_entry->entry_location -= 1;
	populate_entry(parent->dir_entry, parent->dir_entry->entry_location);
	write_file(file_buf, parent->dir_entry,1+(FILENAME_LENGTH * child_count));

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
