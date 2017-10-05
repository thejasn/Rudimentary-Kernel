#include "kmap.h"

#define LINES 25
#define COLUMNS_PER_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREEN_SIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_PER_LINE * LINES

#define K_DATA_PORT 0x60
#define k_STATUS_PORT 0x64
#define IDT_SIZE 256 //interrupt descriptor table
#define INTERRUPT_GATE 0x8e //used to specify interrupt service routines
#define KERNEL_CODE_SEG_OFFSET 0x08

#define PIC1_DATA 0x21 
#define PIC1_COMMAND 0x20
#define PIC2_DATA 0xA0
#define PIC2_COMMAND 0xA1

#define ENTRY_KEY_CODE 0x1C

extern unsigned char keymap[128];
extern void keyboard_handler(void);
extern char read_from_port(unsigned short port);
extern void write_to_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

unsigned int current_cursor_location =0;

struct IDT_entry{

    unsigned short int offset_lowerbits; //offset of bits 0...15
    //offset represents the address of the entry point of the ISR
    unsigned short int selector // a code segment in GDT 
    unsigned int zero; //reserved
    unsigned short int offset_higherbits; //offset of bits 16...31
    unsigned char type_attr;
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

void init_idt(void)
{
    unsigned long keyboard_address,id_address,idt_ptr[2];
    
    //populating the IDT entry of Keyboard's interrupt

    keyboard_address = (unsigned long) keyboard_handler;
    /*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
    *Data	 0x21	0xA1
    
    x86 arch has 2 PIC with 8 input lines. PIC1 gets IRQ0-IRQ7
    and PIC2 gets IRQ9 to IRQ15

    */
    //initializing the data of PIC1
    IDT[PIC1_DATA].offset_lowerbits = keyboard_address & 0xffff;//padding with 1
    IDT[P1C1_DATA].
    IDT[P1C1_DATA].selector = KERNEL_CODE_SEG_OFFSET;
    IDT[P1C1_DATA].zero = 0;
    IDT[P1C1_DATA].type_attr = INTERRUPT_GATE;
    
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
    write_to_port(PIC2_DATA,0x20);

    //ICW3 - cascading i.e op as input to next
    //currently not using it so 0
    write_to_port(PIC1_DATA,0x00);
    write_to_port(PIC2_DATA,0x00);

    //ICW4 additional env info
    //setting the lower bit to tell we are running in 80x86 mode
    write_to_port(PIC1_DATA,0x01);
    write_to_port(PIC2_DATA,0x01);

}