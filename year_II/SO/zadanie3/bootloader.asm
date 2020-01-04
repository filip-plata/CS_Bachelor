org 0x7c00

jmp 0:start

BUFFER times 16 db 0

WELCOME_MSG: db 'Enter your name', 0xd, 0xa, 0x0
ENDLINE: db 0xd, 0xa, 0x0
HELLO_MSG: db 'Hello ', 0x0

;OK
start:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x9000

    jmp main

;OK
print_char:
    mov ah, 0xe
    int 0x10
    ret

;OK
read_char:
    mov ah, 0x0
    int 0x16
    ret

;OK
print:
    mov bx, ax
._loop_print:
    mov ax, [bx]
    cmp al, 0x0
    je .end
    inc bx
    call print_char
    jmp ._loop_print
.end:
    ret

;OK
clear_char:
    mov ax, 0x08
    call print_char

    mov ax, 0x20
    call print_char

    mov ax, 0x08
    call print_char
    ret

;WRONG
read_name:
    mov bx, 0
._loop_read:
    call read_char
    cmp al, 0x08
    jne .not_backspace

    cmp bx, 0
    je ._loop_read

    call clear_char

    dec bx
    mov al, 0x00
    mov [BUFFER + bx], al

    jmp ._loop_read
.not_backspace:
    cmp al, 0x0d
    jne .not_enter

    cmp bx, 3
    jl ._loop_read

    call print_char
    ret
.not_enter:
    cmp bx, 12
    jge ._loop_read
    mov [BUFFER + bx], al
    call print_char
    inc bx
    jmp ._loop_read

;OK
print_greeting:
    mov cx, ax
    mov ax, HELLO_MSG
    call print
    mov ax, cx
    call print
    mov ax, ENDLINE
    call print
    ret

; OK
sleep:
    mov cx, ax
    imul cx, 0xe ;approximation of one second
    xor dx, dx
    mov ah, 0x86
    int 0x15
    ret

save_login:
    mov bx, BUFFER
    mov dx, 0x0080 ; First HDD
    mov cx, 0x0003 ;cylinder 0, sector 3, hopefully what we want

    mov ax, 0x0301
    int 0x13

    jc save_login
    ret

orig_bootloader:
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00  ;cylinder
    mov cl, 0x02  ;sector
    mov dh, 0x00  ;head
    mov dl, 0x80
    mov bx, 0x7c00

    int 0x13
    jc orig_bootloader
    mov ax, 0x7c00
    jmp ax


load_copy:
    mov bx, ax
    mov ah, 0x02
    mov al, 0x01
    mov ch, 0x00  ;cylinder
    mov cl, 0x01  ;sector
    mov dh, 0x00  ;head
    mov dl, 0x80

    int 0x13
    jc load_copy
    ret


; OK
main:
    mov ax, WELCOME_MSG
    call print
    mov ax, BUFFER
    call read_name
    mov ax, BUFFER
    call print_greeting
    call save_login
    mov ax, 2  ; number of seconds
    call sleep

    mov ax, 0x7e00
    call load_copy

    lea ax, [orig_bootloader + 0x0200]
    jmp ax

times 446 - ($ - $$) db 0
