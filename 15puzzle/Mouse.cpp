#define MOUSE_DEF
#include "DxLib.h"
#include "main.h"
#include "Mouse.h"

// ﾏｳｽ情報の更新
void UpdateMouse(void)
{
	// ｸﾘｯｸ情報
	mouseF.trg[TRG_OLD] = mouseF.trg[TRG_NOW];
	mouseF.trg[TRG_NOW] = GetMouseInput();

	// ﾏｳｽｶｰｿﾙの座標取得
	GetMousePoint(&mouseF.pos.x, &mouseF.pos.y);

	// 回転量取得
	mouseF.wheel = GetMouseWheelRotVol();
}