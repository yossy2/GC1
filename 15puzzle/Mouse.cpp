#define MOUSE_DEF
#include "DxLib.h"
#include "main.h"
#include "Mouse.h"

// ϳ����̍X�V
void UpdateMouse(void)
{
	// �د����
	mouseF.trg[TRG_OLD] = mouseF.trg[TRG_NOW];
	mouseF.trg[TRG_NOW] = GetMouseInput();

	// ϳ����ق̍��W�擾
	GetMousePoint(&mouseF.pos.x, &mouseF.pos.y);

	// ��]�ʎ擾
	mouseF.wheel = GetMouseWheelRotVol();
}