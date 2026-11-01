#include<stdio.h>
#include "types.h"
#include "edit.h"
#include<string.h>
Status check_file_name(char *argv[],einfo *estruct)
{  if(argv[4]!=NULL)
    {
char *address=strstr(argv[4],".");
    if(argv[4]!=NULL&&address!=NULL)
    {
        if(strcmp(address,".mp3")==0)
        {
            estruct->file_name=argv[4];
            printf("%s\n",estruct->file_name);
          
          return e_success;
        }
        else
        {
            return e_failure;
        }
    }
    }

    
      
      return  e_failure;
}
Status tag_check(char *argv[],einfo *estruct)
{
   char *edit_tag[]={"-t","-a","-A","-y","-m","-c"};
   for(int i=0;i<6;i++)
   {
   if(strcmp(edit_tag[i],argv[2])==0)
   { 
     estruct->edit_tag=argv[2];
     
    return e_success;
   }
    }
    return e_failure;

}
//open file
Status open_file(einfo *estruct)
{
    estruct->fptr_file=fopen(estruct->file_name,"r+");
   // estruct->fptr_stego=fopen(estruct->stego_name,"w");
    if(estruct->fptr_file==NULL)
    {
        return e_failure;
    }
    /* if(estruct->fptr_stego==NULL)
    {
        return e_failure;
    }*/
    return e_success;

}
Status ID3_edit(einfo *estruct)
{
    ///
   
     fread(estruct->ID3_data,3,sizeof(char),estruct->fptr_file);
     estruct->ID3_data[3]='\0';
    if(strcmp(estruct->ID3_data,"ID3")==0)
    {
        
      return e_success;
    }
    return e_failure;
}
Status version(einfo *estruct)
{
    char version[2];
    fread(version,2,sizeof(char),estruct->fptr_file);
    if(version[0]==3&&version[1]==0)
    {
         fseek(estruct->fptr_file,10,SEEK_SET);
        return e_success;
    }


   
    return e_failure;
}
 
Status copy_reaming_bytes(einfo *estruct)
{  estruct->tem_name="tem.mp3";
    estruct->fptr_tem=fopen( estruct->tem_name,"w");
    char ch;
     estruct->tem_size=0;
    while(fread(&ch,1,sizeof(char),estruct->fptr_file)==1)
    {
       fwrite(&ch,1,sizeof(char),estruct->fptr_tem);
       estruct->tem_size++;
    }
    fclose( estruct->fptr_tem);
    return e_success;
}
Status resize_header_size(char *argv[],einfo *estruct)
{
    uint new_size = strlen(argv[3]) + 1;
    fseek(estruct->fptr_file,6,SEEK_SET);
     unsigned char size[4];
    fread(size,4,sizeof(char),estruct->fptr_file);
   uint header_size=size[0]<<24|size[1]<<16|size[2]<<8|size[3];
        header_size= header_size-estruct->title_size;
    header_size= header_size+new_size;
        fseek(estruct->fptr_file,6,SEEK_SET);
   unsigned char size_bytes[4];
   size_bytes[0]=( header_size>>21)&0x7f;
    size_bytes[1]=( header_size>>14)&0x7f;
     size_bytes[2]=( header_size>>7)&0x7f;
      size_bytes[3]= header_size&0x7f;
      fwrite(size_bytes,4,sizeof(char),estruct->fptr_file);

   fseek(estruct->fptr_file,10,SEEK_SET);
   return e_success;
}
Status edit(char *argv[],einfo *estruct)
{
     char *tag[]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
     char *edit_tag[]={"-t","-a","-A","-y","-m","-c"};
     
     for(int i=0;i<6;i++)
     { //  printf("Inside for\n");
        if(strcmp(argv[2],edit_tag[i])==0)
        {   
            while(1)
            { 
                if(fread(estruct->TAG,4,sizeof(char),estruct->fptr_file))//reading 4 bytes of tag 
               estruct->TAG[4]='\0';
              // printf("TAG = %s\n", estruct->TAG);
              uint position=ftell(estruct->fptr_file);
               unsigned char size[4];
                fread(size,4,sizeof(char),estruct->fptr_file);// read size of tag frame
                estruct->title_size=size[0]<<24|size[1]<<16|size[2]<<8|size[3];
                //printf("%d", estruct->title_size);
                char flag[2];
               fread(flag,2,sizeof(char),estruct->fptr_file);//read 2 bytes of flag
               char encoding;
               fread(&encoding,1,sizeof(char),estruct->fptr_file);//read 1 byte if encoding byte
               if(strcmp(estruct->TAG,tag[i])==0)
               {
                 
              if(strlen(argv[3])+1>estruct->title_size)
              {
                fseek(estruct->fptr_file,estruct->title_size-1,SEEK_CUR);//move the file pointer after the tag frame size
                if(copy_reaming_bytes(estruct)!=e_success)
                {
                   return e_failure;
                }
                fseek(estruct->fptr_file,position,SEEK_SET);//move file pointer back to the original position
                int size=strlen(argv[3])+1;
                unsigned char size_bytes[4];
                size_bytes[0] = (size >> 24) & 0xFF;
                size_bytes[1] = (size >> 16) & 0xFF;
                size_bytes[2] = (size >> 8) & 0xFF; 
                size_bytes[3] = size & 0xFF;

                fwrite(size_bytes, 1, 4, estruct->fptr_file);//we are writing the new tag frame size into frame size
                 fwrite(flag,2,sizeof(char),estruct->fptr_file);//write flag 
                fwrite(&encoding,1,sizeof(char),estruct->fptr_file);//write encoding data
                 fwrite(argv[3],strlen(argv[3]),sizeof(char),estruct->fptr_file);//edit the tag content 
                estruct->fptr_tem=fopen( estruct->tem_name,"r");
               char ch;
              
                while(fread(&ch,1,sizeof(char),estruct->fptr_tem)==1)//copying the reamining bytes in mp3.file
                {
                
                  fwrite(&ch,1,sizeof(char),estruct->fptr_file);
                }
                  if(resize_header_size(argv,estruct)!=e_success)//file size increse  due to increse the header size;
                  {
                  printf("Error : in resizeing of header size\n");
                  return e_failure;
                  }
                
              }
              else  if(strlen(argv[3])+1< estruct->title_size)
              {
                 fseek(estruct->fptr_file,estruct->title_size-1,SEEK_CUR);
                if(copy_reaming_bytes(estruct)!=e_success)
                {
                   return e_failure;
                }
                 fseek(estruct->fptr_file,position,SEEK_SET);
                int size=strlen(argv[3])+1;
                unsigned char size_bytes[4];
                size_bytes[0] = (size >> 24) & 0xFF;
                size_bytes[1] = (size >> 16) & 0xFF;
                size_bytes[2] = (size >> 8) & 0xFF; 
                size_bytes[3] = size & 0xFF;
                 fwrite(size_bytes, 1, 4, estruct->fptr_file);
                  fwrite(flag,2,sizeof(char),estruct->fptr_file);
                fwrite(&encoding,1,sizeof(char),estruct->fptr_file);
                fwrite(argv[3],strlen(argv[3]),sizeof(char),estruct->fptr_file);
                 estruct->fptr_tem=fopen( estruct->tem_name,"r");
               char ch;
            
                while(fread(&ch,1,sizeof(char),estruct->fptr_tem)==1)
                {
                
                  fwrite(&ch,1,sizeof(char),estruct->fptr_file);
                }
                  if(resize_header_size(argv,estruct)!=e_success)//file size increse  due to increse the header size;
                  {
                  printf("Error : in resizeing of header size\n");
                  return e_failure;
                  }
              }
              else
              {
                fwrite(argv[3],strlen(argv[3]),sizeof(char),estruct->fptr_file);
              }
              
                return e_success;

               }
               char title[estruct->title_size-1];
                fread(title, estruct->title_size-1, sizeof(char), estruct->fptr_file);
            }
        }

     }

    return e_success;
}

Status edit_file_content(char *argv[],einfo *estruct)
{
 if(check_file_name(argv,estruct)==e_success)
 {
    printf("file extension checked pass\n");
    if(tag_check(argv,estruct)==e_success)
    {
        printf("tag check passed\n");
        if(open_file(estruct)==e_success)
        {
            printf("file opened successfully\n");
            if(ID3_edit(estruct)==e_success)
            {
               printf("ID3 check passed\n");
               if(version(estruct)==e_success)
               {
                printf("version check passed\n");
              // if(add_header(argv,estruct)==e_success)
              // {
                //printf("header copy successfully\n");
                if(edit(argv,estruct)==e_success)
                {
                    printf("edit successfull\n");
                }
                else
                {
                     
                    printf("Error: in editing\n");
                     return e_failure;
                }
              
               }
               else
               {
                  
                printf("failure in version checking\n");
                 return e_failure;
               }


            }
            else
            {
                  
                printf("failure in id3 checking\n");
                 return e_failure;
            }

        }
        else
        {
             
            printf("failure in file opened\n");
             return e_failure;
        }
        
    }
    else
    {
         
        printf("failure in tag check\n");
         return e_failure;
    }
 }
 else
 {
   
    printf("File extension should be a .mp3\n");
     return e_failure;
 }
 return e_success; 
}