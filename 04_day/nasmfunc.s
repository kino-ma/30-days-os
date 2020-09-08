; nasmfunc

[BITS 32]


	GLOBAL 	io_hlt

[SECTION .text]

; 関数の中身
; void io_hlt();
io_hlt:
	HLT
	RET