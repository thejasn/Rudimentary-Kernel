#include "system.h"

#define MAX_DIR 200
#define MAX_FILE 100
#define MAX_FILE_NAME 64
#define MAX_SUB_DIR 100
#define MAX_DIR_NAME 64



struct filesystem
{
    char current_path[255];
    char directory_name[MAX_DIR][MAX_DIR_NAME];
    char files[MAX_DIR][MAX_FILE][MAX_FILE_NAME];
    int number_of_sub_directries,number_of_files[MAX_FILE];

};
typedef struct filesystem FILE_SYS;

FILE_SYS root;

int text(char file_name[])
{
    //terminal_writestring(CURRENT_DIR);
   int i,j;
    int dir_no = find_dir(CURRENT_DIR);
    if(dir_no != -1)
    {
        str_copy(file_name,root.files[dir_no-1][root.number_of_files[dir_no-1]]);
        root.number_of_files[dir_no-1]++;
        return 1;
    }
    return -1;
}
int create_dir(char dir_name[])
{
    str_copy(dir_name,root.directory_name[root.number_of_sub_directries]);
    root.number_of_sub_directries++;
}
int mkdir(char dir_name[])
{
    if(!str_compare(CURRENT_DIR,ROOT))
        return -1;
    if(find_dir(dir_name)==-1)
    {
        create_dir(dir_name);
        return 1;
    }
    return -1;
}
int cd(char dir_name[])
{
    if(str_compare(dir_name,".."))
    {
        str_copy(ROOT,CURRENT_DIR);
        //str_cat(CURRENT_PATH,DIR_SEPERATOR);        
        return 1;
    }
    if(find_dir(dir_name)!=-1)
    {
        str_copy(dir_name,CURRENT_DIR);
        //terminal_writestring(dir_name);
        //str_cat(CURRENT_PATH,ROOT);
        //str_cat(CURRENT_PATH,DIR_SEPERATOR);
        //str_cat(CURRENT_PATH,CURRENT_DIR);
        return 1;
    }
    return -1;
}
int pwd()
{
    terminal_writestring(CURRENT_DIR);
    terminal_putchar('\n');
}
int ls()
{
    int i,j;
    if(str_compare(CURRENT_DIR,ROOT))
    {
        for(i=0;i<root.number_of_sub_directries;i++)
        {
            terminal_writestring(root.directory_name[i]);
            terminal_putchar('\n');
        }
    }
    else{
        i = find_dir(CURRENT_DIR)-1;
        for(j=0;j<root.number_of_files[i];j++)
        {
            terminal_writestring(root.files[i][j]);
            terminal_putchar('\n');
        }
    }
    
    
    return 1;
}
int find_dir(char dir_name[])
{
    int i;
    for(i=1;i<=root.number_of_sub_directries;i++)
    {
        if(str_compare(root.directory_name[i-1],dir_name)==1)
            return i;
    }
    return -1;
}
int rmdir(char dir_name[])
{
    if(find_dir(dir_name))
    {
        root.number_of_sub_directries--;
        clear_str(root.directory_name[find_dir(dir_name)-1],'\0',strlen(root.directory_name[find_dir(dir_name)-1]));
        find_dir(dir_name);
        return 1;
    }
}
