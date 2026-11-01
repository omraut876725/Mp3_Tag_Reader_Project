/*
===============================================================================
Project Name : MP3 Tag Reader and Editor
Name         : Om Raut
Date         : 30-Oct-2026
Batch ID     : 26019f_009
Language     : C

Description:
    The MP3 Tag Reader and Editor is a project developed in C to read,
    display, and edit metadata stored in MP3 files using the ID3v2.3
    tag format.

    It allows users to view and modify information such as song title,
    artist name, album name, release year, music genre, and comments
    according to their requirements.

    This project uses file handling to read and modify MP3 file data,
    pointers to access and manipulate data, bitwise operations to handle
    binary information, and advanced C programming concepts to implement
    the required functionality.

Key Concepts Used:
    - File Handling
    - Pointers
    - Bitwise Operations
    - Command-Line Arguments
    - Structures and Functions
    - Dynamic Memory Allocation
    - ID3v2.3 Tag Format
===============================================================================
*/
#include <stdio.h>
#include "edit.h"
#include  "mp3.h"
#include "types.h"
#include "view.h"
#include<string.h>
#include "help.h"
 int main(int argc, char *argv[])
  {
   if(read_and_validate_args(argv)==e_view)
   {  vinfo vstruct;
    printf("veiw operation is selected\n");
    // to veiw file content
    if(view_file_content(argv,&vstruct)==e_success)
    {
       // printf("Successfull\n");
    }
    else
    {
        printf("Error:failed to view content of file\n");
    }
   } 
   else if(read_and_validate_args(argv)==e_edit)
   {  einfo estruct;
    printf("edit operation is selected\n");
     if(edit_file_content(argv,&estruct)==0)
     {
        printf("edit successfull\n");
     }
     else
     {
        printf("failure in edit\n");
     }
   } 
   if(read_and_validate_args(argv)==e_help)
   {
    printf("help operation is selected\n");
    if(help()==e_success)
    {
        printf("HELP successfull\n");

    }
    else
    {
        printf("failure to help\n");
    }
   } 
   if(read_and_validate_args(argv)==unsupported)
   {
    printf("---------------------------------------------------------------------------------------\n");
    printf("Error: ./a.out INVALID ARGUMENT\n");
    printf("USAGE\n");
    printf("To view please pass like: ./a.out -v sample.mp3\n");
    printf("To edit please pass like: ./a.out -e  -t/-a/-A/-m/-y/-c  sample.mp3\n");
    printf("To get help pass like:  ./a.out --help\n");
    printf("---------------------------------------------------------------------------------------\n");

   } 




     return 0;
 }
 OperationType  read_and_validate_args(char *argv[])
 {
    if(argv[1]!=NULL&&strcmp(argv[1],"-v")==0)
    {
      return e_view;
    }
    else if(argv[1]!=NULL&&strcmp(argv[1],"-e")==0)
    {
        return e_edit;
    }
   else if(argv[1]!=NULL&&strcmp(argv[1],"--help")==0)
    {
        return e_help;
    }
    else
    {
        return unsupported;
    }

 }