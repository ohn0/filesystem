#include "interface.h"

int application(FILE* file_to_write)
{
	char* file_buf = (char*) malloc(sizeof(char) * (fseek(file_to_write, 0, SEEK_END)));
	int c, i;
	i = 0;
	rewind(file_to_write);
	while((c = fgetc(file_to_write)) != EOF){
		file_buf[i++] = c;
	}
	char* text = "Some text that will go into the filesystem.";
	struct index_entry* dFS = add_file("DIR_", "new.DIR", 4, ENTRY_TYPE_DIR);
	struct index_entry* new_file = add_file(text, "zer0.txt", strlen(text), ENTRY_TYPE_FILE);
	addFile(new_file);
	open_dir("new.DIR");
	display_contents();
	struct index_entry* text_file = add_file(text, "text.TXT", strlen(text), ENTRY_TYPE_FILE);
	addFile(text_file);
	display_contents();
	return 0;
	
}



int show_contents(struct open_dir* parent){
	int i;
	struct children* child = parent->child_list;
	while(child != NULL){
		if(child->deleted == 0){
			for(i = 0; i < FILENAME_LENGTH; i++){
				printf("%c", child->data->entry_name[i]);
			}
			printf("\n");
	
		}

		child = child->next_child;
	}
	return 0;
}
