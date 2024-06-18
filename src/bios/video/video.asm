BITS 16

global bsv_video
bsv_video:
    ; set video mode
	cmp ah, 0x00
	je set_video_mode

	; set cursor position
	cmp ah, 0x02
	je set_cursor_pos

	; get cursor position
	cmp ah, 0x03
	je get_cursor_pos

	; scroll up some lines
	cmp ah, 0x06
	je scroll_up

	; write single character
	cmp ah, 0x0e
	je write_teletype

	; write string
	cmp ah, 0x13
	je write_string

	iret

extern set_video_mode_cpp
set_video_mode:
	call dword set_video_mode_cpp
	iret

extern cursor_x, cursor_y
set_cursor_pos:
	mov [cursor_x], dl
	mov [cursor_y], dh
	mov ax, 0x0
	iret
get_cursor_pos:
	mov dl, [cursor_x]
	mov dh, [cursor_y]
	mov ax, 0x0
	iret

extern scroll_lines
scroll_up:
    pusha

    ; al holds the number of lines to scroll
    mov ah, 0 ; clear the higher byte in ax
    push ax ; push al
    call dword scroll_lines
    add sp, 2 ; pop back the argument

    popa
	iret

extern write_char
write_teletype:
	pusha

    ; al is the character to write
    mov ah, 0 ; clear the higher byte in ax
    push ax ; push al
    call dword write_char
	add sp, 2 ; pop back the argument

	popa
	iret

extern write_string_cpp
write_string:
	pusha

    ; the address of the string is es:bp
    mov ax, es ; move es to ax
    mov dx, bp ; move bgp to dx

    ; calculate the physical address (es << 4) + bp
    shl ax, 4 ; (es << 4)
    add ax, dx ; (+bp)

    push ax ; push the address
    call dword write_string_cpp
	add sp, 2 ; pop back the argument

	popa
	iret
