; nasmfunc

[BITS 32]


	GLOBAL 	io_hlt

; 関数の中身
io_hlt:
	HLT
	RET