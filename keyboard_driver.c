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

#define ENTRY_KEY_CODE 0x1C

extern unsigned char keymap[128];
extern void keyboard_handler(void);
extern char read_from_port(unsigned short port);
extern void write_to_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);

unsigned int current_cursor_location =0;

struct IDT_entry{

    unsigned short int offset_lowerbits; //offset of bits 0...15
    unsigned short int selector // a code segment in GDT 
    unsigned int zero; //reserved
    unsigned short int offset_higherbits; //offset of bits 16...31
    unsigned char type_attr;
    /*
    dpl - descriptor priviledge level
     7                           0
    +---+---+---+---+---+---+---+---+
    | P |  DPL  | S |    GateType   |
    +---+---+---+---+---+---+---+---+
    */
}