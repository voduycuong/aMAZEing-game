#--------------------------------------Makefile-------------------------------------
BLD_DIR := build
CFILES = $(wildcard ./kernel/*.c )
CMDFILES = $(wildcard ./command/*.c)
OFILES = $(CFILES:./kernel/%.c=./build/%.o)
OCMDFILES = $(CMDFILES:./command/%.c=./build/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib

all: clean uart0_build pic_build header_build kernel8.img 
uart1: clean uart1_build kernel8.img run1
uart0: clean uart0_build kernel8.img run0
run: run0

uart1_build: ./uart/uart1.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./uart/uart1.c -o ./build/uart.o

uart0_build: ./uart/uart0.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./uart/uart0.c -o ./build/uart.o

pic_build: ./graphsrc/pic.c 
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./graphsrc/pic.c -o ./build/pic.o

header_build: ./header/string.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./header/string.c -o ./build/string.o

./build/boot.o: ./kernel/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./kernel/boot.S -o ./build/boot.o

./build/%.o: ./kernel/%.c 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./build/%.o: ./command/%.c 
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: ./build/boot.o ./build/uart.o $(OCMDFILES) $(OFILES)
	aarch64-none-elf-ld -nostdlib  ./build/boot.o ./build/uart.o ./build/string.o ./build/pic.o $(OCMDFILES) $(OFILES) -T ./kernel/link.ld -o ./build/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./build/kernel8.elf kernel8.img
clean:
	del .\build\kernel8.elf .\build\*.o *.img

# Run emulation with QEMU
run1: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio 

run0: 
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial stdio 
