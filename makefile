filesystem: directory_index.c directory_index.h filesystem.h file_table.c file_table.h format.c FS_test.c wrappers.c wrappers.h
	gcc -g directory_index.c file_table.c format.c FS_test.c wrappers.c -o FS
