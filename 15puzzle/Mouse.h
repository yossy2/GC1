#pragma once

// enum��`
// ϳ�����p
struct Mouse
{
	Pos pos;						// ���ق̍��W
	unsigned int trg[TRG_MAX];		// �د��������
	int wheel;						// ϳ��̉�]��
};

#ifdef MOUSE_DEF
#define MOUSE_EX
#else
#define MOUSE_EX extern
#endif

MOUSE_EX Mouse mouseF;		// ϳ�����p

// �֐��������ߐ錾
void UpdateMouse(void);		// ϳ����̎擾
