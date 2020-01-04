global verhogen
global proberen

section .data
    mutex DD 0

section .text

align 8
verhogen:
    mov rax, 1
    xadd [rdi], rax
    ret

proberen:
    DEFAULT REL
wait_mutex:
    mov eax, 1
    xchg eax, [mutex]
    test eax, eax
    jnz wait_mutex

    cmp DWORD [rdi], 0
    je wait_semaphore
    sub DWORD [rdi], 1
    mov rax, 0
    xchg eax, [mutex]
    ret
wait_semaphore:
    mov eax, 0
    xchg eax, [mutex]
    jmp wait_mutex
