TARGET = bios.bin

OBJS = 	main/startup.o \
		main/bios_main.o \
		main/ivt_setup.o \
		common/common_funcs.o \
		common/common.o \
		video/vga.o \
		video/video.o \
		video/video_cpp.o \
		disk/disk.o \
		disk/fdd_configure.o \
		disk/disk_cpp.o \
		keyboard/keyboard.o \
		pic/pic_configure.o \
		service.o

CC 	:= gcc
AS 	:= nasm
CFLAGS	+= -masm=intel -nostdlib -fno-asynchronous-unwind-tables -fcf-protection=none -fno-stack-protector -fno-pie -m16
#LDFLAGS	+= --entry=start --oformat=binary -Ttext 0x0 -melf_i386
LDFLAGS	+= -T linker.ld --oformat=binary -melf_i386

all :
	make $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.asm
	$(AS) -f elf $<

$(TARGET) : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
	#$(RM) $(OBJS)
