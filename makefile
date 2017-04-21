filesystem: directory_index.c file_entry.c file_entry.h helpers.h directory_index.h filesystem.h file_table.c file_table.h format.c FS_test.c wrappers.c wrappers.h
	gcc -Wall -g helpers.c  directory_index.c file_entry.c file_table.c format.c FS_test.c wrappers.c -lm -o FS
