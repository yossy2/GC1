#pragma once
// ���C���[
// �������Ⴂ�ق�����
enum class LAYER
{
	BG,
	CHAR,
	UI,
	MAX
};

LAYER begin(LAYER);
LAYER end(LAYER);
LAYER operator++(LAYER& id);
LAYER operator*(LAYER id);