#include "helpers.h"
int compare_names(char* name, char* entry_name){
	int i,n;
	for(i = 0; i < 8; i++){
		if(name[i] == '.'){
			break;
		}
		if(name[i] != entry_name[i] && name[i] != '.'){
			return 0;
		}

	}
	i++;
	for(n = 0; n < 3; n++){
		if(name[i++] != entry_name[8+n]){return 0;}
	}

	return 1;
}
