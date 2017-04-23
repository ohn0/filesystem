filesystem: directory_index.c delete.c delete.h  file_entry.c file_entry.h helpers.h directory_index.h filesystem.h file_table.c file_table.h format.c FS_test.c wrappers.c wrappers.h extend.c extend.h IOCS.c IOCS.h
	gcc -Wall -g helpers.c delete.c  directory_index.c file_entry.c file_table.c format.c FS_test.c wrappers.c extend.c IOCS.c -lm -o FS
