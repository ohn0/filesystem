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

	

int FS_getint()
{
	int a = FS_getc();
	int b = FS_getc();
	return (b*256)+a;

}

int FS_putint(unsigned int val){
	// // fseek(FILESTREAM, -2, SEEK_CUR
	// printf("%X " ,(unsigned char) (val >> 16));
	// printf(" %X\n" ,(unsigned char) val);
	FS_putc((unsigned char) (val >> 16));
	FS_putc((unsigned char) val);
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
