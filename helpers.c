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

	int i, start;
	printf("\n");
	for(i = 0; i < FILENAME_LENGTH; i++){
		printf("%X", name[i]);
	}
	printf("\n");
	int new_size = 0;
	for(i = 0; i < FILENAME_LENGTH; i++){
		new_size++;
		if(name[i] == ' '){
			break;
		}
	}
	char* new_name = (char*) malloc(sizeof(char) *(new_size+3));
	int nc = 0;
	for(i = 0; name[i] != ' '; i++){
		new_name[i] = name[nc++];
	}
	new_size+=3;
	new_name[nc] = '.';
	start = FILENAME_LENGTH -1;
	new_name[new_size-1] = name[start--];
	new_name[new_size-2] = name[start--];
	new_name[new_size-3] = name[start--];
	//	for(;i != FILENAME_LENGTH; i++){
//		if(name[i] != ' '){
//			new_name[start++] = name[i];
	//	}
//	}
	
	printf("Printing new name:\n");
	for(i = 0; i != new_size; i++){
		printf("%c", new_name[i]);
	}
	printf("\n");
	return new_name;

}
