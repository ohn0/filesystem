#include "wrappers.h"
int FS_write(char* write_from, struct entry* file_entry)
{
	//Wrapper around fputc.
	FS_setpos(file_entry);
	int i = 0;
	while(fputc(write_from[i++], FILESTREAM)){;}
}

int FS_setpos(struct entry* file_entry)
{
	//Wrapper around fseek.
	fseek(FILESTREAM, file_entry->start_block, SEEK_SET);
	return 0;
}

int FS_reset(){rewind(FILESTREAM);}

char FS_getc(){
	return  fgetc(FILESTREAM);
}

	

unsigned int FS_getint()
{
	unsigned int a;
	unsigned int val = 0; unsigned int i = 32;
	while(i != 0){
		i -= 8;
		a = (unsigned char)FS_getc();
		printf("A Val:%X\n", a << i);
		val |= (a << i);
	}
	return val;
}

int FS_getMiniInt(){
	int a = FS_getc();
	return (256 * a) + FS_getc();
}

int FS_putint(unsigned int val){
	// // fseek(FILESTREAM, -2, SEEK_CUR
	// printf("%X " ,(unsigned char) (val >> 16));
	// printf(" %X\n" ,(unsigned char) val);
	unsigned int int_size = 32;
	while(int_size != 0){
		int_size -= 8;
		FS_putc(val >> int_size);
	}
	return 0;
}

int FS_putMiniInt(unsigned int val){
	FS_putc(val >> 8);
	FS_putc(val);
	return 0;
}

char FS_peek(unsigned int point_loc)
{
	//Reads a byte and puts it back so the stream pointer never moved.
	char c = FS_getc();
	ungetc(c, FILESTREAM);
}

int FS_putc(unsigned char val){
	fputc(val, FILESTREAM);
	return 0;
}

int FS_read(char* read_to, struct entry* file_entry)
{
	//Wrapper around fgetc.
	FS_setpos(file_entry);
	int i = 0;
	while((read_to[i++] = fgetc(FILESTREAM)) != EOF){;}
	return 0;
}

int FS_jump(int jump_point)
{
	fseek(FILESTREAM, jump_point, SEEK_CUR);
	return 0;
}

int FS_getpos(){
	return ftell(FILESTREAM);
}
