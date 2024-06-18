BITS 16


global set_es
set_es:
    push ebp
    mov ebp, esp

    ; save edi
    push edi

    mov di, word [ebp + 0x8]
    mov es, di

    ; restore edi
    pop edi

    o32 leave
    o32 ret

global write_es_byte, write_es_word, write_es_dword, copy_es_word
write_es_byte:
	push ebp
	mov ebp, esp

    ; save edi
	push edi

	mov di, word [ebp + 0x8]
	mov eax, dword [ebp + 0xc]

	mov byte [es:di], al

    ; restore edi
	pop edi

	o32 leave
	o32 ret

write_es_word:
	push ebp
	mov ebp, esp

    ; save edi
	push edi

	mov di, word [ebp + 0x8]
	mov eax, dword [ebp + 0xc]

	mov word [es:di], ax

    ; restore edi
	pop edi

	o32 leave
	o32 ret

write_es_dword:
	push ebp
	mov ebp, esp

    ; save edi
	push edi

	mov di, word [ebp + 0x8]
	mov eax, dword [ebp + 0xc]

	mov dword [es:di], eax

    ; restore edi
	pop edi

	o32 leave
	o32 ret

copy_es_word:
	push ebp
	mov ebp, esp
	push esi
	push edi
	mov di, word [ebp+0x8]
	mov si, word [ebp+0xc]
	mov ax, word [es:si]
	mov word [es:di], ax
	pop edi
	pop esi
	o32 leave
	o32 ret

global in_port, out_port
in_port:
	push ebp
	mov ebp, esp

	push edx ; save edx

	mov edx, dword [ebp + 0x8]
	xor eax, eax ; clear eax

	in al, dx ; get input from port number dx into register al

	pop edx ; restore edx

	o32 leave
	o32 ret

out_port:
	push ebp
	mov ebp, esp

    ; save eax and edx
	push eax
	push edx

	mov edx, dword [ebp + 0x8] ; get the port number
	mov eax, dword [ebp + 0xc] ; get the value to output

	out dx, al ; output the value at al to port number dx

    ; restore eax and edx
	pop edx
	pop eax

	o32 leave
	o32 ret


global disable_intr, enable_intr
disable_intr:
	cli
	o32 ret
	
enable_intr:
	sti
	o32 ret
