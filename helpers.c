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

char* format_file_name(char* name)
{
	char* new_name = (char*) malloc(sizeof(char) * FILENAME_LENGTH);
	int i, start;
	for(i = 0; name[i] != ' '; i++){
		new_name[i] = name[i];
	}
	new_name[i] = '.';
	start = i+1;
	for(;i != FILENAME_LENGTH; i++){
		if(name[i] != ' '){
			new_name[start++] = name[i];
		}
	}


	for(i = 0; i != FILENAME_LENGTH; i++){
		printf("%c", new_name[i]);
	}
	return new_name;

}
