section .text

global crc32

crc32:
	push rbp				; saving base pointer 	


    mov     rax, 0
    crc32   rax, qword [rdi]
    crc32   rax, qword [rdi+8]
    crc32   rax, qword [rdi+16]
    crc32   rax, qword [rdi+24]

    pop rbp
    ret