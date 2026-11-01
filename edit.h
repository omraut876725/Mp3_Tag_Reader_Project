#ifndef EDIT
#define EDIT
#include "types.h"
typedef struct edit_info
{
FILE *fptr_file;
char *file_name;
char *edit_tag;
char ID3_data[3];
char TAG[5];
uint title_size;
FILE *fptr_tem;
char *tem_name;
uint tem_size;

}einfo;
//for edit tag content
Status edit_file_content(char *argv[],einfo *estruct);
//check file tag
Status check_file_name(char *argv[],einfo *estruct);
// tag checking
Status tag_check(char *argv[],einfo *estruct);
//open file
Status open_file(einfo *estruct);
//check ID3
Status ID3_edit(einfo *estruct);
//version checking 
Status version(einfo *estruct);
//do edit;
Status edit(char *argv[],einfo *estruct);
// copy the reaming bytes if input sting size is differet from original
Status copy_reaming_bytes(einfo *estruct);
Status resize_header_size(char *argv[],einfo *estruct);
#endif