#include "IOCS.h"
int create_root_dir()
{
	//Creates a root directory within the file system.
	FS_reset();
	add_file("hi :D", "ROOT", 5, ENTRY_TYPE_DIR);
	char* buf = read_file();
	int i;
	for(i = 0; i < 5; i++){
		printf("%c", buf[i]);
	}
	return 0;
}

int generate_block_table()
{

}

int create_file()
{

}

int open_root()
{
	FS_reset();
	struct index_entry* root = find_entry("ROOT", DIRECTORY_INDEX);
	
}
