#define DIR_SEPERATOR "/"
#define PROMPT " $ "
#define ROOT "myos"
#define ROOT_PATH "/myos"


extern unsigned short * memcpy(unsigned short*,const unsigned short*,int);
extern int strlen(const char*);
extern unsigned short int str_compare(unsigned char*,unsigned char*);
extern unsigned char * str_cat(unsigned char*,unsigned char *);
extern char * generate_prompt(void);
extern unsigned short * memsetw(unsigned short*,unsigned short,int);
extern unsigned char * strset(char[],char,int);
extern int str_substr_end(unsigned char*,unsigned char *);
extern int create_file(char[],char[]);
extern int create_dir(char[]);
extern int find_dir(char[]);
extern void str_copy(char[],char[]);
extern void clear_str(char[],char,int);
extern unsigned char inportb(unsigned short);
extern void outportb(unsigned short,unsigned char);
extern int mkdir(char[]);
extern void ret_convertor(int);
extern void terminal_initialize(void);
extern void terminal_setcolor(unsigned char);
extern void terminal_putentryat(unsigned char,unsigned char,int,int);
extern void terminal_newline();
extern void terminal_move_cursor();
extern void terminal_scroll(void);
extern void terminal_write(const unsigned char*,int);
extern void terminal_writestring(const unsigned char*);
extern void terminal_putchar(unsigned char c);
extern int cd(char[]);
extern int ls(void);
extern unsigned char * str_add(unsigned char*,unsigned char*);
extern int  terminal_row,terminal_col;
extern unsigned char terminal_color,current_loc;
extern unsigned short  * terminal_buffer;
extern int pwd();
extern int rmdir(char[]);
extern int text(char[]);



extern char * CURRENT_PATH;
extern char * CURRENT_DIR ;
extern int RET_VAL ;
