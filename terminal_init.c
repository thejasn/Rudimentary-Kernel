#include "system.h"
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
//unsigned int current_cursor_location =0;
static inline unsigned char vga_entry_color(enum vga_color  fg,enum vga_color  bg)
{
    return fg | bg <<4;
}
static inline unsigned short  vga_entry(unsigned char uc, unsigned char color)
{
    return (unsigned short)uc | (unsigned short ) color<<8;
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
unsigned char terminal_color,current_loc=0;
unsigned short  * terminal_buffer;

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_col = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE,VGA_COLOR_BLACK);
    terminal_buffer = (unsigned short *)0xB8000;
    for(int  y = 0; y < VGA_HEIGHT; y++)
        for(int  x = 0; x < VGA_WIDTH; x++)
        {
                const int  index = y * VGA_WIDTH +  x;
                terminal_buffer[index] = vga_entry(' ',terminal_color);
        }
}

void terminal_setcolor(unsigned char color)
{
    terminal_color = color;
}
void terminal_putentryat(unsigned char c , unsigned char color, int  y, int  x)
{
    const int  index = y * VGA_WIDTH + x;
    terminal_buffer[index]=vga_entry(c,color);
    terminal_col++;

}
void terminal_newline()
{
    terminal_col =0;
    terminal_row+=1;
}
void terminal_move_cursor(void)
{
    /*  http://www.brokenthorn.com/Resources/OSDev10.html 
        CRT controller uses a single data register that is mapped to 0x3d5 and the CRT uses a special register
        index register (0x3d4) to determine the type of data in data register.
        
        14 (0xE) is the cursor location high byte
        15 (0xF) is the cursor location low byte i.e the x/y offset location of the cursor
    */

    int temp = terminal_row * VGA_WIDTH + terminal_col;


    outportb(0x3D4,14);
    outportb(0x3D5, temp>>8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}
void terminal_scroll(void)
{
    int temp;
    if(terminal_row >= VGA_HEIGHT)
    {
        temp = terminal_row - VGA_HEIGHT + 1;
        /* copies all but the first line of the VGA buffer to the start of the buffer, thereby deleting the first line of the buffer.
           the last line is then filled with blank spaces to depict a new line. And the terminal_row is set to VGA_HEIGHT - 1 */
        memcpy(terminal_buffer,terminal_buffer + temp * VGA_WIDTH, ( VGA_HEIGHT - temp) * VGA_WIDTH * 2 );
        memsetw(terminal_buffer + (VGA_HEIGHT - temp) * VGA_WIDTH , vga_entry(' ',terminal_color),VGA_WIDTH);
        terminal_row = VGA_HEIGHT - 1;

    }
}
void terminal_putchar(unsigned char c)
{
    if(c == 0x09) // if tab is pressed
    {
        terminal_col +=4;
        terminal_putentryat(' ',terminal_color,terminal_row,terminal_col);
        terminal_move_cursor();
        if( terminal_col == VGA_WIDTH)
            terminal_newline();
        return;
    }
    if( c == 0x0E)
    {
        terminal_col-=1;
        terminal_putentryat(' ',terminal_color,terminal_row,terminal_col);
        terminal_move_cursor();
    }
    if(c=='\n')
    {
       terminal_col=0;
       terminal_row+=1;
      
    }    
    else
        terminal_putentryat(c,terminal_color,terminal_row,terminal_col);
    
    terminal_scroll();
    terminal_move_cursor();

}

void terminal_write(const unsigned char * data , int  size)
{
    for( int  i = 0 ; i< size;i++)
        terminal_putchar(data[i]);
    //current_cursor_location+=size;
}
void terminal_writestring(const unsigned char * data)
{
    terminal_write(data,strlen(data));
}


/*void main_kernel(void){
    terminal_initialize();
    terminal_writestring("Hello Kernel! \n");
    terminal_writestring("\n Whats UPPPPP!!!");
}*/