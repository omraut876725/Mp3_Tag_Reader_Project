#ifndef VIEW
#define VIEW
#define ID3_size 4
#define VERSION_SIZE 2
#define TAG_SIZE 5
typedef struct view_info
{
FILE *fptr_file;
char *file_name;
char ID3_data[ID3_size];
char *fptr_ID3;
char vesion[VERSION_SIZE];
uint file_size;

char buffer_tag[TAG_SIZE];
uint title_size;

}vinfo;

// check user enter valid arguments
  
  OperationType read_and_validate_args(char *argc[]);

//view the content of the file
Status view_file_content(char *argv[],vinfo *vstruct);
/*check the extension of the file*/
Status check_file_extn(char *argv[],vinfo *vstruct);
/*open the file*/
Status open_File(vinfo *vstruct);
/*check whether the file is ID3 or nor*/
Status ID3(vinfo *vstruct);
/*check version of the file*/
Status file_version(vinfo *vstruct);
/* print the tag*/
Status view_tag(vinfo *vstruct);



#endif