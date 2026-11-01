#include<stdio.h>
#include "types.h"
#include "view.h"
#include<string.h>
#include<stdlib.h>
#include<math.h>
Status check_file_extn(char *argv[],vinfo *vstruct)
{  char *address;
    if(argv[2]!=NULL)
    {address=strstr(argv[2],".");
    }
    if(argv[2]!=NULL&&address!=NULL)
    {
        if(strcmp(address,".mp3")==0)
        {
            vstruct->file_name=argv[2];
            printf("%s\n",vstruct->file_name);
           return e_success;

        }
        else
        {
            return e_failure;
        }
    }
      
    return e_failure;
}
Status open_File(vinfo *vstruct)
{
    vstruct->fptr_file=fopen(vstruct->file_name,"r");
    if(vstruct->fptr_file==NULL)
    {
        return e_failure;
    }
    return e_success;
}
Status ID3(vinfo *vstruct)
{ 
    fread(vstruct->ID3_data,3,sizeof(char),vstruct->fptr_file);
    vstruct->ID3_data[3]='\0';
    if(strcmp(vstruct->ID3_data,"ID3")==0)
    {
        
       
        return e_success;
    }
    return e_failure;
}
Status file_version(vinfo *vstruct)
{
    char version[2];
    if(fread(version,2,sizeof(char),vstruct->fptr_file)==1)
    {
        if(version[0]==3&&version[1]==0)
        {  
            return e_success;
        }
    }
 
  
   
    
    return e_failure;
}
/*print the tag*/
Status view_tag(vinfo *vstruct)
{  char *tag[]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
   char *meaning[]={"TITLE","ARTIST","ALBUM","YEAR","MUSIC","COMMENTS"};
printf("-----------------------selected print detial--------------------------\n");
printf("\n\n");
printf("----------------------------------------------------------------------\n");
printf("                MP3 TAG READER AND EDITOR FOR ID3V2                   \n");
printf("----------------------------------------------------------------------\n");
  
  fseek(vstruct->fptr_file,0,SEEK_END);
    uint end=ftell(vstruct->fptr_file);
     fseek(vstruct->fptr_file,10,SEEK_SET);
 for(int i=0;i<6;i++)
  {  
     fseek(vstruct->fptr_file,10,SEEK_SET);//skip the header of10bytes
     while(1)
     {  if(ftell(vstruct->fptr_file)==end)
        {
            break;
        }
      unsigned char size[4];
        if(fread(vstruct->buffer_tag,4,sizeof(char),vstruct->fptr_file)!=1)
        {//reading frame id like TIT2.... //4 bytes
            break;
        }
         vstruct->buffer_tag[4]='\0';
        
       fread(size,4,sizeof(char),vstruct->fptr_file);//reading the size of title size=4bytes
       vstruct->title_size=size[0]<<24|size[1]<<16|size[2]<<8|size[3];
       
       uint flag;
       fread(&flag,2,sizeof(char),vstruct->fptr_file);//reading 2 bytes of flag
       char encoding;
       fread(&encoding,1,sizeof(char),vstruct->fptr_file);//read 1 bytes of encoding data
       
     
           char title[vstruct->title_size+1];
           fread(title,vstruct->title_size-1,sizeof(char),vstruct->fptr_file);
           title[vstruct->title_size-1]='\0';
         if(strcmp(vstruct->buffer_tag,tag[i])==0)
        {
           printf("%-20s:                   %s\n",meaning[i],title);
         break;
        }
        
    }
        
   
   }
   printf("----------------------------------------------------------------------\n");
   printf("--------------------DETAIL DISPLAY SUCCESSFULLY-----------------------\n");

   return e_success;
  
}

Status view_file_content(char *argv[],vinfo *vstruct)
{
    if(check_file_extn(argv,vstruct)==e_success)
   {
      if(open_File(vstruct)==e_success)
      {
       if(ID3(vstruct)==e_success)
       {
        
        printf("ID3check passed\n");
        /*check file version*/
        if(file_version(vstruct)==e_success)
        {
            printf("the version of the file is ID3v2.3\n");
            /* print the tag*/
            if(view_tag(vstruct)==e_success)
            {
                printf("tag print successfully\n");
            }
            else
            {
                printf("Error: to print tag\n");
                return e_failure;
            }
        }
        else
        {
            printf("Error:file version not match \n");
            return e_failure;
        }
       }
       else
       {
        printf("Error:ID3 check failed\n");
       }
      }
      else
      {
        fprintf(stderr,"Error:failed to file opened \n");
      }
   }
   else
   {
    fprintf(stderr,"ERROR: file extension must be .mp3\n");
    return e_failure;
   }

return e_success;
}
