#include "DxLib.h"
#include "main.h"
#include "Game.h"
#include "Select.h"
#include "Mouse.h"

int selScreen[PZL_SEL_MAX];		// �ڸėp�̉摜�̕`���
int selPzScreen;				// ��L����ɂ܂Ƃ߂��`���
SEL_MOVE selMoveMode;			// �摜����������

int selPanelMove;				// �摜�ړ��p����

// �I��ݏ�����
bool SelectInit(void)
{
	bool rtnFlag = true;

	divID = 0;

	buttonPos.x = SCREEN_SIZE_X - (SCREEN_SIZE_X - PUZZLE_SIZE_X) / 2;
	buttonPos.y = SCREEN_SIZE_Y - BUTTON_SIZE_Y / 2 - 10;			// 10�͗]��
	buttonFlag = 0;

	for (int i = 0; i < PZL_SEL_MAX; i++)
	{
		selScreen[i] = MakeScreen(PUZZLE_SIZE_X, PUZZLE_SIZE_Y, true);
		pzlImageID[i] = 0;
	}

	selPzScreen = MakeScreen(PUZZLE_SIZE_X, PUZZLE_SIZE_Y, true);
	selPanelMove = 0;
	selMoveMode = SEL_MOVE_NON;

	sceneFunc = SelectScene;

	return rtnFlag;
}

// �I��ݏ���
bool SelectScene(void)
{
	SelectDraw(); 

	if (selPanelMove != 0)
	{
		if (selMoveMode == SEL_MOVE_RIGHT)
		{
			selPanelMove += 5;
			if (selPanelMove >= PUZZLE_SIZE_X)
			{
				selPanelMove = 0;
				selMoveMode = SEL_MOVE_NON;
			}
		}
		else if (selMoveMode == SEL_MOVE_LEFT)
		{
			selPanelMove -= 5;
			if (selPanelMove <= 0)
			{
				selPanelMove = 0;
				selMoveMode = SEL_MOVE_NON;
			}
		}
		else
		{
			AST();
			selPanelMove = 0;
		}
		return false;
	}

	// �ްъJ�n����
	if (OnClickCircle())
	{
		GameInit();

		// �摜�J��
		DeleteGraph(selScreen[0]);
		DeleteGraph(selScreen[1]);
		DeleteGraph(selPzScreen);
		return true;
	}	

	if (buttonFlag)
	{
		return false;
	}

	// ���د�
	if ((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT))
	{
		if (divID < (sizeof(divTable) / sizeof(divTable[0]) - 1))
		{
			divID++;
		}
	}

	// �E�د�
	if ((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_RIGHT))
	{
		if (divID > 0)
		{
			divID--;
		}
	}

	// �摜�I��
	// �E�ɗ����
	if (mouseF.wheel < 0)
	{
		selPanelMove = -PUZZLE_SIZE_X;
		pzlImageID[PZL_SEL_OLD] = pzlImageID[PZL_SEL_NOW];
		pzlImageID[PZL_SEL_NOW]++;
		if (pzlImageID[PZL_SEL_NOW] >= PZL_IMAGE_MAX)
		{
			pzlImageID[PZL_SEL_NOW] = 0;
		}
		selMoveMode = SEL_MOVE_RIGHT;
	}

	// ���ɗ����
	if (mouseF.wheel > 0)
	{
		selPanelMove = PUZZLE_SIZE_X;
		pzlImageID[PZL_SEL_OLD] = pzlImageID[PZL_SEL_NOW];
		pzlImageID[PZL_SEL_NOW]--;
		if (pzlImageID[PZL_SEL_NOW] < 0)
		{
			pzlImageID[PZL_SEL_NOW] = PZL_IMAGE_MAX - 1;
		}
		selMoveMode = SEL_MOVE_LEFT;
	}

	return false;
}

// �I��ݕ`��
void SelectDraw(void)
{
	// �I�𒆂̉摜�ƕ��������Z�b�g�ɂ����摜�����
	SetDrawScreen(selScreen[PZL_SEL_NOW]);
	ClsDrawScreen();
	DrawGraph(0, 0, puzzleImg[pzlImageID[PZL_SEL_NOW]], false);
	DrawDivLine();

	if (selPanelMove != 0)
	{
		// �I��؂�ւ���
		// �ЂƂO�̑I���摜�ƕ��������Z�b�g�ɂ����摜�����
		SetDrawScreen(selScreen[PZL_SEL_OLD]);
		ClsDrawScreen();
		DrawGraph(0, 0, puzzleImg[pzlImageID[PZL_SEL_OLD]], false);
		DrawDivLine();
	}

	// ��ō�����摜������
	SetDrawScreen(selPzScreen);
	ClsDrawScreen();

	if (selMoveMode == SEL_MOVE_RIGHT)
	{
		DrawGraph(selPanelMove, 0, selScreen[PZL_SEL_NOW], false);
		DrawGraph(selPanelMove + PUZZLE_SIZE_X, 0, selScreen[PZL_SEL_OLD], false);
	}
	else if (selMoveMode == SEL_MOVE_LEFT)
	{
		DrawGraph(selPanelMove, 0, selScreen[PZL_SEL_NOW], false);
		DrawGraph(selPanelMove - PUZZLE_SIZE_X, 0, selScreen[PZL_SEL_OLD], false);
	}
	else
	{
		DrawGraph(selPanelMove, 0, selScreen[PZL_SEL_NOW], false);
	}

	// �ޯ��ޯ̧�ɕ`��
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(0, 0, selPzScreen, false);
	DrawButton();
	DrawFormatString(320, 0, 0xffffff, "div : %d", divTable[divID]);
	ScreenFlip();
}

// ���݂̕`��
void DrawButton()
{
	DrawRectRotaGraph(buttonPos.x, buttonPos.y, buttonFlag * BUTTON_SIZE_X, 0, BUTTON_SIZE_X, BUTTON_SIZE_Y, 1.0, 0.0, buttonImg, true,false);
}

// ϳ������݂̏�ɂ��邩����
bool OnClickCircle()
{
	// ���݂̒��S����ϳ��̋����̓��
	int length = (mouseF.pos.x - buttonPos.x) * (mouseF.pos.x - buttonPos.x)
		+ (mouseF.pos.y - buttonPos.y) * (mouseF.pos.y - buttonPos.y);

	if (buttonFlag && !(mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT))
	{
		buttonFlag = BUTTON_OFF;
		if (length <= ((BUTTON_SIZE_X - BUTTON_EDGE_WIDTH) / 2) * ((BUTTON_SIZE_X - BUTTON_EDGE_WIDTH) / 2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	if ((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT))
	{
		if (length <= ((BUTTON_SIZE_X - BUTTON_EDGE_WIDTH) / 2) * ((BUTTON_SIZE_X - BUTTON_EDGE_WIDTH) / 2))
		{
			buttonFlag = BUTTON_ON;
		}
	}
	
	return false;
}