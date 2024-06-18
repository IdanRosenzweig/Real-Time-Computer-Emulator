extern setup_ivt, bios_main
global start
section .text
BITS 16
extern write_char
start:
	mov ax, cs
	mov ds, ax
	mov ax, 0x400
	mov ss, ax
	mov sp, 0x2000

	xor ax, ax
	mov es, ax

    ; configure hardware
	;call init_pic
	call init_fdd
	call init_vga

    ; setup ISRs in the IVT
	call dword setup_ivt

	; call bios main
	call dword bios_main

    ; mov al, 0x3
    ; mov ah, 0x6
    ; int 0x10
;
	; call dword bios_main
;
    ; mov bp, str
    ; mov ah, 0x13
    ; int 0x10

    ; hlt

	 call load_mbr ; load the master boot sector
     jmp 0x0:0x7c00 ; jumping to the OS

extern pic_configure
init_pic:
    call dword pic_configure
	ret

extern fdd_configure_cpp
init_fdd:
	call dword fdd_configure_cpp
	ret

extern vga_configure
init_vga:
	call dword vga_configure
	ret

load_mbr:
    ; address to write to es:eb
	mov ax, 0x0
	mov es, ax
	mov bx, 0x7c00

	mov ah, 0x2	; read

	mov al, 0x1	; amount of sectors to read

	mov ch, 0x0	; cylinder
	mov cl, 0x1	; sector
	mov dh, 0x0	; head

	mov dl, 0x0	; drive number

	int 0x13
	ret

str:
	db "test str", 0x00