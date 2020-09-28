#define MOUSE_DEF
#include "DxLib.h"
#include "main.h"
#include "Mouse.h"

// Ï³½î•ñ‚ÌXV
void UpdateMouse(void)
{
	// ¸Ø¯¸î•ñ
	mouseF.trg[TRG_OLD] = mouseF.trg[TRG_NOW];
	mouseF.trg[TRG_NOW] = GetMouseInput();

	// Ï³½¶°¿Ù‚ÌÀ•Wæ“¾
	GetMousePoint(&mouseF.pos.x, &mouseF.pos.y);

	// ‰ñ“]—Êæ“¾
	mouseF.wheel = GetMouseWheelRotVol();
}