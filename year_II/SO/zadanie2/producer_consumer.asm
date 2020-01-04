global init
global producer
global consumer

section .bss
    ;buffer pointers
    buffer_start         resb 8
    buffer_end           resb 8
    buffer_end_ptr       resb 8
    buffer_start_ptr     resb 8
    mutex                resb 4   ;mutex semaphore
    producer_s           resb 4   ;producer semaphore
    consumer_s           resb 4   ;consumer semaphore

section .text

extern produce
extern consume
extern malloc

extern proberen
extern verhogen

align 8
init:
    xor rax, rax
    mov QWORD [buffer_end], rdi
    sub QWORD [buffer_end], 1

    cmp rdi, 0
    jne N_not_zero
N_is_zero:
    mov rax, -2
    ret
N_not_zero:
    cmp rdi, 0x07FFFFFFF
    jle N_ok
N_too_big:
    mov rax, -1
    ret
N_ok:
    call malloc wrt ..plt
    cmp rax, 0
    jne good_alloc
bad_alloc:
    mov rax, -3
    ret
good_alloc:
    mov QWORD [buffer_start], rax
    mov QWORD [buffer_end_ptr], rax
    mov QWORD [buffer_start_ptr], rax
    add QWORD [buffer_end], rax
    mov rax, 0
    mov DWORD [mutex], 1
    mov DWORD [producer_s], edi
    mov DWORD [consumer_s], 0
    ret

producer:
    mov rdi, buffer_start_ptr
    call produce

    cmp rax, 0
    jne continue_p
end_producing:
    ret
continue_p:
    mov edi, producer_s
    call proberen
    add QWORD [buffer_start_ptr], 1
    cmp QWORD [buffer_start_ptr], buffer_end
    jg go_back
    mov edi, consumer_s
    call verhogen
    ret
go_back:
    mov edi, consumer_s
    call verhogen
    mov QWORD [buffer_start_ptr], buffer_start
    jmp producer


consumer:
    mov edi, consumer_s
    call proberen
    mov rdi, [buffer_end_ptr]
    call consume
    cmp rax, 0
    jne continue_c
end_consuming:
    ret
continue_c:
    add QWORD [buffer_end_ptr], 1
    cmp QWORD [buffer_end_ptr], buffer_end
    jg go_back_c
    mov edi, producer_s
    call verhogen
    ret
go_back_c:
    mov QWORD [buffer_end_ptr], buffer_start
    mov edi, producer_s
    call verhogen
    jmp consumer
