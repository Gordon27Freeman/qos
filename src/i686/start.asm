global start
extern kmain, init

FLAGS equ 1 << 0 | 1 << 1 | 1 << 2
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	dd 0, 0, 0, 0, 0
	dd 0, 800, 600, 32

section .text
start:
	mov esp, stack
	push eax
	push ebx

	mov eax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, eax
	mov eax, cr4
	or ax, 3 << 9
	mov cr4, eax

	call init
	call kmain

.l:	cli
	hlt
jmp .l

section .bss
	resb 0x10000
stack:
