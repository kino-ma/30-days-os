; nasmfunc

[BITS 32]


	GLOBAL 	io_hlt,	io_cli,	io_stihlt
	GLOBAL	io_in8, 	io_in16,	io_in32
	GLOBAL	io_out8,	io_out16,	io_out32
	GLOBAL	io_load_eflags, io_store_eflags
	GLOBAL	load_gdtr, load_idtr

[SECTION .text]

; 関数の中身
; void io_hlt();
io_hlt:
	HLT
	RET

; void io_cli(void);
io_cli:
	CLI
	RET

; void io_sti(void);
io_sti:
	STI
	RET

; void io_stihlt(void);
io_stihlt:
	STI
	HLT
	RET

; int io_in8(int port);
io_in8:
	MOV		EDX, [ESP + 4]	; port
	MOV 	EAX, 0
	IN		AL, DX
	RET

; int io_in16(int port);
io_in16:
	MOV		EDX, [ESP + 4]	; port
	MOV 	EAX, 0
	IN		AX, DX
	RET

; int io_in32(int port);
io_in32:
	MOV		EDX, [ESP + 4]	; port
	IN		EAX, DX
	RET

; int io_in8(int port, int data);
io_out8:
	MOV		EDX, [ESP + 4]	; port
	MOV 	AL, [ESP + 8]	; data
	OUT		DX, AL
	RET

; int io_out16(int port, int data);
io_out16:
	MOV		EDX, [ESP + 4]	; port
	MOV 	EAX, [ESP + 8]	; data
	OUT		DX, AX
	RET

; int io_out32(int port, int data);
io_out32:
	MOV		EDX, [ESP + 4]	; port
	MOV 	EAX, [ESP + 8]	; data
	OUT		DX, EAX
	RET

; int io_load_eflags(void);
io_load_eflags:
	PUSHFD					; push EFLAGS
	POP		EAX
	RET

; void io_store_eflags(int eflags);
io_store_eflags:
	MOV		EAX, [ESP + 4]
	PUSH	EAX
	POPFD					; pop EFLAS
	RET

; void load_gdtr(int limit, int addr);
load_gdtr:
	MOV     AX, [ESP + 4]	; limit
	MOV 	[ESP + 6], AX	; addr
	LGDT 	[ESP + 6]
	RET

; void load_idtr(int limit, int addr);
load_idtr:
	MOV     AX, [ESP + 4]	; limit
	MOV 	[ESP + 6], AX	; addr
	LIDT 	[ESP + 6]
	RET