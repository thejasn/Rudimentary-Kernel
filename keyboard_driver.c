#include "kmap.h"
#include "system.h"



#define LINES 25
#define COLUMNS_PER_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREEN_SIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_PER_LINE * LINES

#define K_DATA_PORT 0x60
#define K_STATUS_PORT 0x64
#define IDT_SIZE 256 //interrupt descriptor table
#define INTERRUPT_GATE 0x8e //used to specify interrupt service routines
#define KERNEL_CODE_SEG_OFFSET 0x08 // GRUB sets up a GDB for us
// each GDB entry is 8 bytes long and the kernel code segment is in the second one
// so the offset is 0x08

#define PIC1_DATA 0x21 
#define PIC1_COMMAND 0x20
#define PIC2_DATA 0xA1
#define PIC2_COMMAND 0xA0

#define ENTER_KEY_CODE 0x1C

int command_ptr=0;
unsigned char command[255];
extern unsigned char keymap[128];
extern void keyboard_handler(void);
extern char read_from_port(unsigned short port);
extern void write_to_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

void process_command(char[]);
void command_run(char[],char[]);
unsigned int current_cursor_location =0;
struct IDT_entry{

    unsigned short int offset_lowerbits; //offset of bits 0...15
    //offset represents the address of the entry point of the ISR
    unsigned short int selector; // a code segment in GDT 
    unsigned char zero; //reserved
    unsigned char type_attr;
    unsigned short int offset_higherbits; //offset of bits 16...31
   
    /*
    dpl - descriptor priviledge level
    gatetype setting it to interupt gate
     7                           0
    +---+---+---+---+---+---+---+---+
    | P |  DPL  | S |    GateType   |
    +---+---+---+---+---+---+---+---+
    */
};
typedef struct IDT_entry IDT_entry;
IDT_entry IDT[IDT_SIZE];
//char *terminal_buffer = (char*)0xb8000;
void init_idt(void)
{
    unsigned long keyboard_address,idt_address,idt_ptr[2];
    
    //populating the IDT entry of Keyboard's interrupt

    keyboard_address = (unsigned long) keyboard_handler; //it was typecasted to unsigned long... I fucked with it
    /*     Ports
	*	     PIC1	PIC2
	*Command 0x20	0xA0
    *Data	 0x21	0xA1
    
    x86 arch has 2 PIC with 8 input lines. PIC1 gets IRQ0-IRQ7
    and PIC2 gets IRQ9 to IRQ15

    */
    //initializing the data of PIC1
    IDT[PIC1_DATA].offset_lowerbits = keyboard_address & 0xffff;//padding with 1
    
    IDT[PIC1_DATA].selector = KERNEL_CODE_SEG_OFFSET;
    IDT[PIC1_DATA].zero = 0;
    IDT[PIC1_DATA].type_attr = INTERRUPT_GATE;
    IDT[PIC1_DATA].offset_higherbits = (keyboard_address & 0xffff0000 ) >>16;
    
    /* initializing the PIC using 8 bit ICW initialization command words
    first command ICW1 0x11 makes PIC wait for 3 more words on data port
    ICW2 is the vector offset
    ICW3 is how the PICs are wired as master/slaves
    ICW4 additional ifo on environment
    */
    
    //ICW1 init
    write_to_port(PIC1_COMMAND,0x11);
    write_to_port(PIC2_COMMAND,0x11);

    //ICW2 - need to remap the affset addresses of the IDT 
    // since the in intel x86 first 32 are reserved 
    write_to_port(PIC1_DATA,0x20); //0x20 = 32(10)
    write_to_port(PIC2_DATA,0x28);

    //ICW3 - cascading i.e op as input to next
    //currently not using it so 0
    write_to_port(PIC1_DATA,0x00);
    write_to_port(PIC2_DATA,0x00);

    //ICW4 additional env info
    //setting the lower bit to tell we are running in 80x86 mode
    write_to_port(PIC1_DATA,0x01);
    write_to_port(PIC2_DATA,0x01);

    /* each PIC has a 8bit interrupt mask register stores the bitmap of IRQ
    if a bit is set PIC ignores a request. writint to the IMR sets the reg.
    initally we are disabling all interrupts
    */
    write_to_port(PIC1_DATA,0xff);
    write_to_port(PIC2_DATA,0xff);
    
    // fill the IDT

    idt_address = (unsigned long)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16 ;
    //call load IDT from asm
    load_idt(idt_ptr);


}
void kprint_newline(void)
{
	unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_PER_LINE;
	current_cursor_location = current_cursor_location + (line_size - current_cursor_location % (line_size));
}
void kb_init(void)
{
    //0xFD is 11111101 enables only IRQ1 
    write_to_port(0x21,0xFD);
}
/*
void clear_screen(void)
{
	unsigned int i = 0;
	while (i < SCREEN_SIZE) {
		terminal_buffer[i++] = ' ';
		terminal_buffer[i++] = 0x07;
	}
}
void kprint(const char *str)
{
	unsigned int i = 0;
	while (str[i] != '\0') {
		//vidptr[current_loc++] = str[i++];
		//vidptr[current_loc++] = 0x07;
	}
}*/
void process_command(char command_to_process[])
{
    //unsigned char * compare = "thejas";
    

    unsigned char command[255];
    unsigned char arg[255];
    int i,j,flag=0;
    for(i=0;i<strlen(command_to_process);i++)
    {    if(command_to_process[i]==' ')
        {
            for(j=0;j<strlen(command_to_process)-i;j++)
                arg[j] = command_to_process[i+j+1];
            flag = 1;
            break;
        }
    }
    for(i=0;i<strlen(command_to_process);i++)
    {    if(command_to_process[i]!=' ')
        {
            command[i]=command_to_process[i];
        }
    }
    if(flag==1)
        command_run(command,arg);
    else{
        if(str_compare(command_to_process,"p"))
            ret_convertor(RET_VAL);
        else if(str_compare(command,"ls"))
        {
            RET_VAL = ls();
        }
        else if(str_compare(command_to_process,"pwd"))
        {
            RET_VAL = pwd();
        }
        else terminal_writestring(" No such command found! \n");
    }
    clear_str(command,'\0',strlen(command));
    clear_str(arg,'\0',strlen(arg));
    clear_str(command_to_process,'\0',strlen(command_to_process));
}
void command_run(char command[],char arg[])
{
    /*terminal_writestring(command);
    terminal_writestring("!");
    terminal_writestring("\n");

    terminal_writestring(arg);
    create_dir(arg);*/
    if(str_compare(command,"mkdir"))
    {    
        RET_VAL = mkdir(arg);
    }
    else if(str_compare(command,"cd"))
    {
        RET_VAL = cd(arg);
    }
    else if(str_compare(command,"rmdir"))
    {
        RET_VAL = rmdir(arg);
    }
    else if(str_compare(command,"text"))
    {
        terminal_writestring("here\n");
        RET_VAL = text(arg);
    }
    else terminal_writestring(" No such command found!! \n");
    clear_str(command,'\0',strlen(command));
    clear_str(arg,'\0',strlen(arg));
    
}
void keyboard_handler_main(void)
{
    unsigned char status;
    char keycode;

    /* write EOI */
    write_to_port(0x20, 0x20);

    status = read_from_port(0x64);
    /* Lowest bit of status will be set if buffer is not empty */
    if (status & 0x01)
    {
        keycode = read_from_port(0x60);
        if (keycode < 0)
            return;

        if (keycode == ENTER_KEY_CODE)
        {
            //kprint_newline();

            terminal_putchar('\n');
            //terminal_writestring(command);
            //terminal_putchar('\n');
            command_ptr = 0;
            if (str_compare(command, "") == 1)
            {
                terminal_writestring("empty\n");
                terminal_writestring(generate_prompt());
                terminal_writestring(PROMPT);
                clear_str(command, '\0', strlen(command));
                return;
            }
            //terminal_writestring(command);

            //terminal_putchar('\n');
            process_command(command);
            clear_str(command, '\0', strlen(command));
            terminal_writestring(generate_prompt());
            terminal_writestring(PROMPT);
            clear_str(command, '\0', strlen(command));
            return;
        }
        if (keycode == 0x4B)
        {
            terminal_col -= 1;
            terminal_move_cursor();
            return;
        }
        if (keycode == 0x4D)
        {
            terminal_col += 1;
            terminal_move_cursor();
            return;
        }
        //terminal_putchar(current);
        char current = kmap[(unsigned long)keycode];
        terminal_putchar(current);
        if (current != '\b')
            command[command_ptr++] = current;
        else
            command_ptr--;
    }
}

void main_kernel(void){
    //terminal_initialize();
    //terminal_writestring("Enabling Keyboard support! \n");
    terminal_initialize();
    terminal_writestring(generate_prompt());
    terminal_writestring(PROMPT);
    //ierminal_writestring(r
    //clear_screen();
    init_idt();
    kb_init();
    while(1);
    
}