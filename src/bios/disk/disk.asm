BITS 16

global bsv_disk
bsv_disk:
    ; read sectors from drive
	cmp ah, 0x02
	je disk_read_sector

    ; write sectors to drive
	cmp ah, 0x03
	je disk_write_sector

	;cmp ah, 0x0c
	;je disk_seek_cylinder
	;mov ax, 0x01
	iret

global bsv_irq_disk
bsv_irq_disk:
	cli
	pusha
	mov al, 0x66
	out 0x20, al
	mov dx, 0x03f4
	in al, dx
	test al, 0x80
	jz irq_disk_end
	mov dx, 0x03f5
	test al, 0x40
	jz irq_disk_read
	mov al, [es:bx]
	out  dx, al
	jmp irq_disk_end
irq_disk_read:
	in  al, dx
	mov [es:bx], al
irq_disk_end:
	popa
	inc bx
	iret


; Parameters
; AL	amount of sectors to read
; CH	cylinder
; CL	sector
; DH	head
; DL	drive number
; ES:BX	Buffer Address Pointer

; Results
; CF	Set On Error, Clear If No Error
; AH	Return Code
; AL	Actual Sectors Read Count
extern read_sectors_from_drive
disk_read_sector:
	pusha

	mov ah, al ; ah: count
	xchg dh, ch
	xchg ch, cl
	; ch : sector
	; cl : head
    ; dh : cylinder

read_sector:
	push 0xff00
	push 0x1202
	push cx
	push dx
	call fdd_read_data

read_busywait:
    ; checking to see if the value was read
	mov dx, 0x03f4
	in al, dx

	test al, 0x80
	jz read_busywait ; didn't read value yet, repeat and busy wait until read


    mov di, bx
    add di, 0x200
    ; di now holds the last address in the buffer that we would write the sector into

	mov dx, 0x03f5
read_loop:
	in al, dx ; get the value read from drive into al

	mov [es:bx], al ; write to buffer

	inc bx ; increment the index in the buffer

	cmp bx, di
	jl read_loop ; if didn't reached the last index, read another one

    ; finished reading the sector, get status
	mov cx, 0
read_status:
	in al, dx

	inc cx
	cmp cx, 7
	jl read_status


	pop dx
	pop cx
	add sp, 0x04

	add ch, 1 ; increment the current sector pointer
	sub ah, 1 ; decreament the count of sectors left to read
	test ah, ah
	jne read_sector ; if count is not 0, read another sector


	mov di, sp
	add di, 0x14	
	mov ax, [ss:di]
	and ax, 0xfffe
	mov [ss:di], ax

	popa
	iret


; disk_write_sector
disk_write_sector:

	mov ah, al ; ah: count
	xchg dh, ch
	xchg ch, cl
    ; ch : sector
    ; cl : head
    ; dh : cylinder

write_sector:
	mov di, bx
	add di, 0x200
	; di now holds the last address in the buffer that we would write the sector into

	push 0xff00
	push 0x1202
	push cx
	push dx
	call fdd_write_data

write_busywait:
	mov al, [es:bx]
	out dx, al
	inc bx
	cmp bx, di
	jl write_busywait

    ; finished writing the sector, get status
	mov dx, 0x03f5
	mov cx, 0
write_status:
	in al, dx

	inc cx
	cmp cx, 7
	jl write_status


	pop dx
	pop cx
	add sp, 0x04

	add ch, 1 ; increment the current sector pointer
	sub ah, 1 ; decreament the count of sectors left to write
	test ah, ah
	jne write_sector ; if count is not 0, write another sector

	iret


; fdd_read_track
fdd_read_track:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x02
	out dx, al
	mov cx, 0x08
	jmp fdd_args

; fdd_write_data
fdd_write_data:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x05
	out dx, al
	mov cx, 0x08
	jmp fdd_args

; fdd_read_data
fdd_read_data:
	push bp
	mov bp, sp
	mov dx, 0x03f5
	mov al, 0x06
	out dx, al
	mov cx, 0x08
	jmp fdd_args

global fdd_configure
fdd_configure:
	push bp
	mov bp, sp

	mov dx, 0x03f5
	mov al, 0x13
	out dx, al

	mov cx, 0x03
	jmp fdd_args

fdd_args:
	mov si, 0

args_loop:
	cmp si, cx
	jge args_end

	mov al, [bp+si+4]
	out dx, al

	inc si
	jmp args_loop

args_end:
	leave
	ret
