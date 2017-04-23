#include "IOCS.h"
int create_root_dir()
{
	//Creates a root directory within the file system.
	FS_reset();
	add_file("hi :D", "ROOT.ROT", 5, ENTRY_TYPE_DIR);
	struct index_entry* entry = find_entry("ROOT.ROT", DIRECTORY_INDEX);
	if(entry == NULL){printf("NULLLLLL\n");}
	char* buf = read_file(entry);
	int i;
	for(i = 0; i < 5; i++){
		printf("%c", buf[i]);
	}
	printf("\n");
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
	
		while(index_count-- > 0){
			dir_entry->entry = (struct index_entry*) malloc(sizeof(struct index_entry));
			populate_entry_struct(dir_entry->entry, FS_getpos());
			FS_jump(20);
			dir_entry->next = (struct directory_table*) malloc(sizeof(struct directory_table));
			dir_entry = dir_entry->next;	
		}
		local_dir = next_dir;
	}while(next_dir != 0);	
}

int create_file()
{

}

int open_root()
{
	FS_reset();
	struct index_entry* root = find_entry("ROOT", DIRECTORY_INDEX);
	
}
