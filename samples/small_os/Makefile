TARGET := kernel.img

TGT1 := boot.bin
BOOT_OBJS := boot.asm

TGT2 := kernel.bin
KERNEL_OBJS := startup.o \
		funcs.o \
		init.o \
		keyboard.o \
		vga.o \
		main.o \

CC	:= gcc
AS	:= nasm
CFLAGS  += -masm=intel -nostdlib -fno-asynchronous-unwind-tables -fcf-protection=none -fno-stack-protector -fno-pie -m32
LDFLAGS := --entry=start --oformat=binary -Ttext 0x10000 -melf_i386
#LDFLAGS := -T linker.ld --oformat=binary -melf_i386

all :
	make $(TARGET)

%.o : %.c Makefile
	$(CC) $(CFLAGS) -c $<

%.o : %.asm Makefile
	$(AS) -f elf $<

$(TARGET) : $(TGT1) $(TGT2)
	cat $^ > $@

$(TGT1) : $(BOOT_OBJS)
	$(AS) $< -o $@

$(TGT2) : $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

clean :
	$(RM) $(KERNEL_OBJS) $(TGT1) $(TGT2) $(TARGET)
