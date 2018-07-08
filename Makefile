ARCH	= i686

CC	= $(ARCH)-elf-g++
LD	= $(ARCH)-elf-gcc
AS	= nasm

CCFLAGS	= -D ARCH_$(ARCH) -ffreestanding -fno-exceptions -fno-rtti -O3 $(CINCLUDES) $(CWARNINGS)
LDFLAGS	= -T src/$(ARCH)/link.ld -ffreestanding -O3 -nostdlib -lgcc
ASFLAGS = -f elf

CINCLUDES	= -Iinclude/ -Iinclude/$(ARCH)
CWARNINGS	= -Wall -Wextra

CSOURCES	= $(wildcard src/$(ARCH)/*.cpp src/*.cpp)
COBJECTS	= $(CSOURCES:%.cpp=%.o)

ASOURCES	= $(wildcard src/$(ARCH)/*.asm src/*.asm)
AOBJECTS	= $(ASOURCES:%.asm=%.ao)

KERNEL	= quantumOS.elf
ISO	= quantumOS.iso

all: $(KERNEL)

$(ISO): $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/$(KERNEL)
	echo "set timeout=3" > iso/boot/grub/grub.cfg
	echo "menuentry \"QuantumOS\" {" >> iso/boot/grub/grub.cfg
	echo "	multiboot /boot/$(KERNEL)" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg
	grub-mkrescue iso -o $(ISO)

$(KERNEL): $(ASOURCES) $(CSOURCES) $(AOBJECTS) $(COBJECTS)
	$(LD) $(LDFLAGS) $(AOBJECTS) $(COBJECTS) -o $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

%.ao: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

qemu: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -m 16

clean:
	rm -f $(ISO)
	rm -f $(KERNEL)
	rm -f $(AOBJECTS)
	rm -f $(COBJECTS)
	rm -rf iso/
