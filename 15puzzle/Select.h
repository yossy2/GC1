#pragma once

// �摜�̓�������
enum SEL_MOVE
{
	SEL_MOVE_NON,			// �ړ����Ă��Ȃ�
	SEL_MOVE_RIGHT,			// �E����
	SEL_MOVE_LEFT,			// ������
};

// �֐��������ߐ錾
bool SelectInit(void);		// �I��ݏ�����
bool SelectScene(void);		// �I��ݏ���
void SelectDraw(void);		// �I��ݕ`��

void DrawButton();			// ���݂̕`��
bool OnClickCircle();		// ϳ������݂̏�ɂ��邩����
