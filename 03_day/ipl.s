; hello-os

	ORG		0x7c00

	CYLS 	EQU 10

; 以下は標準的なFAT12フォーマットフロッピーディスクのための記述

	JMP 	entry
	DB 		0x90
	DB		"HELLOIPL"
	DW		512
	DB		1
	DW		1
	DB		2
	DW		224
	DW		2880
	DB		0xf0
	DW		9
	DW		18
	DW		2
	DD		0
	DD		2880
	DB		0,0,0x29
	DD		0xffffffff
	DB		"HELLO-OS   "
	DB		"FAT12   "
	TIMES	18 DB 0

; プログラム本体
entry:
	MOV 	AX, 0
	MOV 	SS, AX
	MOV 	SP, 0x7c00
	MOV 	DS, AX

; ディスクを読む
	MOV		AX, 0x0820
	MOV 	ES, AX
	MOV 	CH, 0
	MOV 	DH, 0
	MOV 	CL, 2

	MOV 	AH, 0x02	; シリンダ0
	MOV 	DH, 0		; ヘッド0
	MOV 	CL, 2		; セクタ2

readloop:
	MOV 	SI, 0		; 失敗回数

retry:
	MOV 	AH, 0x02	; 0x02: ディスク読み込み
	MOV		AL, 1		; 1セクタ
	MOV 	BX, 0
	MOV 	DL, 0x00
	INT 	0x13		; ディスクBIOSコール
	JNC		next		; 失敗がなければ

	ADD 	SI, 1
	CMP 	SI, 5
	JAE 	error
	MOV 	AH, 0x00
	MOV 	DL, 0x00
	INT 	0x13
	JMP 	retry

next:
	MOV 	AX, ES		; アドレスを0x200進める
	ADD 	AX, 0x0020
	MOV 	ES, AX
	ADD 	CL, 1
	CMP 	CL, 18
	JBE 	readloop

	MOV 	CL, 1
	ADD 	DH, 1
	CMP 	DH, 2
	JB 		readloop 	; if DH < 2

	MOV 	DL, 0
	ADD 	CH, 1
	CMP 	CH, CYLS
	JB 		readloop

fin:
	HLT					; 何かあるまでCPUを停止させる
	JMP 	fin

error:
	MOV 	SI, msg

msg:
	DB		0x0a, 0x0d, 0x0a, 0x0d
	DB		"load error"
	DB		0x0a, 0x0d
	DB		0

	TIMES	0x1fe-($-$$) DB 0

	DB		0x55, 0xaa