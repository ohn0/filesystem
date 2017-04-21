#include "helpers.h"
int compare_names(char* name, char* entry_name){
	int i = 0;
	while(i < 8){
		if(name[i] != entry_name[i]){
			return 0;
		}
		i++;
	}
	return 1;
}
