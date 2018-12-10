global start, sse_support
extern kmain, init

FLAGS equ 1 << 0 | 1 << 1 | 1 << 2
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	dd 0, 0, 0, 0, 0
	dd 0, 1024, 768, 32

section .text
start:
	mov esp, stack

	; put magic number and multiboot header pointer on stack
	push eax
	push ebx

	; check if cpu supports sse2
	mov eax, 0x1
	cpuid
	test edx, 1<<25
	jz .no_sse

	; enable sse2
	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, 3 << 9
	mov cr4, eax

	mov byte [sse_support], 0xa0
	
.main:
	call init
	call kmain

	jmp .l

.no_sse:
	mov byte [sse_support], 0xf0
jmp .main

	; hang cpu if kmain returned
.l:	cli
	hlt
jmp .l

sse_support db 0

; reserve 16k for stack
section .bss
	resb 0x4000
stack:
