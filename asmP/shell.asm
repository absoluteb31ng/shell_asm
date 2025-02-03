BITS 64

global execute_command

section .data
    sh_path db '/bin/sh', 0
    sh_arg db '-c', 0
    error_msg db 'Error al ejecutar comando', 10, 0
    error_len equ $ - error_msg

section .text

execute_command:
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9

    lea rsi, [sh_path]
    lea rdx, [sh_arg]
    mov rcx, rdi
    sub rsp, 32
    mov [rsp], rsi
    mov [rsp+8], rdx
    mov [rsp+16], rcx
    mov qword [rsp+24], 0

    mov rdi, rsi
    mov rsi, rsp
    xor rdx, rdx
    mov rax, 59
    syscall

    mov rax, 1
    mov rdi, 1
    lea rsi, [error_msg]
    mov rdx, error_len
    syscall

    add rsp, 32
    pop r9
    pop r8
    pop rcx
    pop rdx
    pop rsi
    pop rdi
    ret
