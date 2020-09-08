; nasmfunc

[BITS 32]


	GLOBAL 	io_hlt

; 関数の中身
; void io_hlt();
io_hlt:
	HLT
	RET

; void write_mem8(int addr, int data);
write_mem8:
	MOV 	ECX, [ESP + 4] 		; ESP + (sizeof int) * 1
	MOV 	ECX, [ESP + 8] 		; ESP + (sizeof int) * 2
	MOV 	[ECX], AL
	RET