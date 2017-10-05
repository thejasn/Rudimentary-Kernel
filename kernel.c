enum  vga_color  {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline unsigned int vga_entry_color(enum vga_color  fg,enum vga_color  bg)
{
    return fg | bg <<4;
}
static inline unsigned int  vga_entry(unsigned char uc, unsigned int color)
{
    return (unsigned int )uc | (unsigned int ) color<<8;
}
int  strlen(const char * str)
{
    int  len =0;
    while(str[len])
        len++;
    return len;
}
static const int  VGA_WIDTH = 80;
static const int  VGA_HEIGHT = 25;

int  terminal_row,terminal_col;
unsigned int terminal_color;
unsigned int  * terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_col = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK);
    terminal_buffer = (unsigned int *)0xB8000;
    for(int  y = 0; y < VGA_HEIGHT; y++)
        for(int  x = 0; x < VGA_WIDTH; x++)
        {
                const int  index = y * VGA_WIDTH +  x;
                terminal_buffer[index] = vga_entry(' ',terminal_color);
        }
}

void terminal_setcolor(unsigned int color)
{
    terminal_color = color;
}
void terminal_putentryat(char c , unsigned int color, int  y, int  x)
{
    const int  index = y * VGA_WIDTH + x;
    terminal_buffer[index]=vga_entry(c,color);
}
void terminal_putchar(char c)
{
    if(c=='\n')
    {
        terminal_col = 0;
        terminal_row+=1;
        c=' ';
    }    
    terminal_putentryat(c,terminal_color,terminal_row,terminal_col);
    if(++terminal_col == VGA_WIDTH)
    {    terminal_col=0;
        if(++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    }
}
void terminal_write(const char * data , int  size)
{
    for( int  i = 0 ; i< size;i++)
        terminal_putchar(data[i]);
}
void terminal_writestring(const char * data)
{
    terminal_write(data,strlen(data));
}
#if defined(__cplusplus)
extern "C"
#endif
void main_kernel(void){
    terminal_initialize();
    terminal_writestring("Hello Kernel! \n");
    terminal_writestring("\n Whats UPPPPP!!!");
}