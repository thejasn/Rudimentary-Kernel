gcc -fno-stack-protector -m32 -c keyboard_driver.c -o kc.o
gcc -fno-stack-protector -m32 -c string.c -o string.o
gcc -fno-stack-protector -m32 -c directory.c -o directory.o
gcc -fno-stack-protector -m32 -c environment.c -o environment.o
gcc -fno-stack-protector -m32 -c asm.c -o asm.o
gcc -fno-stack-protector -m32 -c terminal_init.c -o terminal_init.o
ld -m elf_i386 -T linker.ld -o kernel kasm.o kc.o terminal_init.o string.o directory.o environment.o asm.o
qemu-system-i386 -kernel kernel
