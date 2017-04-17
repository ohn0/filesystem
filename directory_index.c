#include "directory_index.h"
struct index_entry* create_entry(char* entry_name)
{
	struct index_entry* new_entry =  malloc(sizeof(struct index_entry));
	int i;
	for(i = 0; i < FILE_NAME_LENGTH; i++){
		new_entry->entry_name[i] = entry_name[i];
	}
	new_entry->last_mod_timestamp = time(NULL);
	return (struct index_entry*) new_entry;
}