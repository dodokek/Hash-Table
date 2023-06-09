section .text

global MurMurAsm


MurMurAsm:
	push rbp				; saving base pointer 	
	
	push rbx

	; rdi - string
	; rsi - len

	mov eax, 5bd1e995h	; hash = seed ^ len
	xor rax, rsi

	mov rcx, 0			; coef1 = 0

	
.loop:
	mov rcx, [rdi]			; coef1 = data[0]
	mov rbx, [rdi + 1]		; coef1 = data[1] << 8	
	mov rdx, [rdi + 2]		; coef1 = data[1] << 8
	mov r11, [rdi + 3]		; coef1 = data[1] << 8
	
	shl rbx, 8
	shl rdx, 16
	shl r11, 24

	or rcx, rbx
	or rcx, rdx
	or rcx, r11

	add rdi, 4

	mov edx, 5bd1e995h
	mul edx				; hash *= salt
	xor rax, rcx		; hash ^= coef1

	sub rsi, 4d			; len -=4

	cmp rsi, 4
jge	.loop

	; switch (len)
	cmp rsi, 3	; case 3
	jne skip3

		mov rbx, [rdi + 2]		; hash ^= data[2] << 16
		shl rbx, 16
		xor rax, rbx

	skip3:
	cmp rsi, 2	; case 2
	jne skip2

		mov rbx, [rdi + 1]		; hash ^= data[2] << 8
		shl rbx, 8
		xor rax, rbx

	skip2:
	cmp rsi, 1	; case 1
	jne skip1

		mov rbx, [rdi]		; hash ^= data[0];
		xor rax, rbx
		mov edx, 5bd1e995h
		mul edx		; hash *= salt

	skip1:

	mov edx, eax	; hash ^= hash >> 13
	shr edx, 13
	xor eax, edx

	mov edx, 5bd1e995h
	mul edx				; hash *= salt

	mov edx, eax	; hash ^= hash >> 15
	shr edx, 15
	xor eax, edx

	pop rbx

	pop rbp

	ret
