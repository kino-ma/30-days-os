; haribote-os

BOTPAK 		EQU 	0x00280000	; bootpackのロード先
DSKCAC 		EQU 	0x00100000	; ディスクキャッシュの場所
DSKCAC0 	EQU 	0x00008000 	; 同上(リアルモード)


; BOOT_INFO関係
CYLS 	EQU 	0x0ff0
LEDS 	EQU 	0x0ff1
VMODE 	EQU 	0x0ff2
SCRNX 	EQU 	0x0ff4
SCRNY 	EQU 	0x0ff6
VRAM 	EQU 	0x0ff8

	ORG 	0xc200

	MOV 	AL, 0x13		; VGA グラフィックス
	MOV 	AH, 0x00
	INT 	0x10

	MOV 	BYTE [VMODE], 8	; 画面モードを記録する
	MOV 	WORD [SCRNX], 320
	MOV 	WORD [SCRNY], 200
	MOV 	DWORD [VRAM], 0x000a0000

; キーボードのLED状態を取得する
	MOV 	AH, 0x02
	INT 	0x16
	MOV 	[LEDS], AL

; PICが一切の割り込みを受け付けないようにする
	MOV 	AL, 0xff
	OUt 	0x21, AL
	NOP
	OUT 	0xa1, AL

	CLI 				; CPUレベルでも割り込み禁止

; CPUから1MB以上のメモリにアクセスできるように、A20GATEを設定
	CALL 	waitkbdout
	MOV 	AL, 0xd1
	OUT 	0x64, AL
	CALL 	waitkbdout
	MOV 	AL, 0xdf
	OUT 	0x60, AL
	CALL 	waitkbdout

; プロテクトモード以降
	LGDT 	[GDTR0]
	MOV 	EAX, CR0
	AND 	EAX, 0x7fffffff	; bits[31]を0にする（ページング禁止のため）
	OR 		EAX, 0x00000001 ; bit0を1にする（プロテクトモード移行のため）
	MOV 	CR0, EAX
	JMP 	pipelineflush

pipelineflush:
	MOV 	AX, 1 * 8 		; 読み書き可能セグメント32bit
	MOV 	DS, AX
	MOV 	ES, AX
	MOV 	FS, AX
	MOV 	GS, AX
	MOV 	SS, AX

; bootpackの転送
	MOV 	ESI, bootpack 	; 転送元
	MOV 	EDI, BOTPAK 	; 転送先
	MOV 	ECX, 512 * 1024 / 4
	CALL 	memcpy

; ディスクデータも本来の位置に転送
;  まずブートセクタ
	MOV 	ESI, 0x7c00
	MOV 	EDI, DSKCAC
	MOV 	ECX, 512 / 4
	CALL	memcpy

; 残り全部
	MOV 	ESI, DSKCAC0 + 512	; 転送元
	MOV 	EDI, DSKCAC  + 512	; 転送元
	MOV 	ECX, 0
	MOV 	CL, BYTE [CYLS]
	IMUL 	ECX, 512 * 18 * 2 / 4	; IPLの分だけ差し引く
	SUB 	ECX, 512 / 4
	CALL 	memcpy

; bootkackの起動
	MOV 	EBX, BOTPAK
	MOV 	ECX, [EBX + 16]
	ADD 	ECX, 3				; ECX += 3
	SHR 	ECX, 2				; ECX /= 4
	JZ 		skip
	MOV 	ESI, [EBX + 20]
	ADD 	ESI, EBX
	MOV 	EDX, [EBX + 12]
	CALL 	memcpy

skip:
	MOV 	ESP, [EBX + 12] 	; スタック初期値
	JMP 	DWORD 2 * 8:0x0000001b

waitkbdout:
	IN		AL, 0x64
	AND 	AL, 0x02
	JNZ 	waitkbdout
	RET

memcpy:
	MOV 	EAX, [ESI]
	ADD 	ESI, 4
	MOV 	[EDI], EAX
	ADD 	EDI, 4
	SUB 	ECX, 1
	JNZ 	memcpy		; 引き算した結果が0でなければ再びmemcpy
	RET

	ALIGNB 	16, DB 0

GDT0:
	TIMES 	8 	DB 0
	DW 		0xffff, 0x0000, 0x9200, 0x00cf	; 読み書き可能セグメント32bit
	DW 		0xffff, 0x0000, 0x9a28, 0x0047	; 読み書き可能セグメント32bit

	DW 		0

GDTR0:
	DW		8 * 3 - 1
	DD 		GDT0
	ALIGNB	16, DB 0

bootpack: