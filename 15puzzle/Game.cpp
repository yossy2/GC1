#define GAME_DEF
#include <time.h>
#include <math.h>
#include "DxLib.h"
#include "main.h"
#include "Mouse.h"
#include "Game.h"
#include "Clear.h"


int **pzData;		// �����摜ID�i�[�p
Pos chipSize;		// ���������ۂ����߻���
int blankID;		// ��������ID
int blankAnimCount;	// �������ق̱�Ұ��ݶ���
Pos mouseChip;		// ϳ�������Ă����وʒu
int moveSpeedTable[] = { 80,40,32,20,16 };

// �ړ����ِ���p
Pos movePanelPos;	// �ړ����ق̍��W
Pos moveEndPos;		// �ړ��I���n�_�̍��W

// ���Ӱ�ޗp
int autoID;			// ���Ӱ�ނŎ��ɑ����������ق�ID
bool autoFlag;		// ���Ӱ�ނ��ǂ���
bool blankMove;		// �������ق̓��ʂȈړ�Ӱ�ށA�������������ق̈ړ���ɔ������ق���������
DIR blankMoveDir;	// �������ق𓮂����Ă�����������
DIR avoDir;			// �����������
int blankMoveCnt;	// blankMove���s�����񐔁A�v4��ňړ���ɍs����
Pos blank;			// blankID�̏ꏊ���L��

bool xLineLast;		// ����̍Ō�̓�����낦��Ӱ��
bool yLineLast;
int lineNumY;		// ������������̔ԍ�
int lineNumX;		// ���������c��̔ԍ�

bool jamaFlag;		// ��̉E�[�����ق��ז��Ȏ��ɂ���Ӱ��
bool cycleFlag;		// ��]Ӱ��

bool autoLastLine;	// �Ō�̓������낦�邼

// �ްѼ�ݏ�����
bool GameInit(void)
{
	bool rtnFlag = true;

	if ((divID < 0) || (divID >= (sizeof(divTable) / sizeof(divTable[0]))))
	{
		AST();
		divID = 0;
	}
	shuffleSeed = (unsigned int)time(NULL);
	srand(shuffleSeed);
	// ��؂��c���Ă�����
	FreeMemData();

	// ���I����؂��m��
	pzData = (int **)malloc(sizeof(int*) * divTable[divID]);
	if (pzData == NULL)
	{
		AST();
		return false;
	}

	*pzData = (int *)malloc(sizeof(int) * divTable[divID] * divTable[divID]);
	if (*pzData == NULL)
	{
		AST();
		return false;
	}

	for (int i = 1; i < divTable[divID]; i++)
	{
		pzData[i] = pzData[0] + i * divTable[divID];
	}

	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			pzData[y][x] = x + y * divTable[divID];
			//pzData[y][x] = y + x * divTable[divID];
		}
	}

	chipSize.x = PUZZLE_SIZE_X / divTable[divID];
	chipSize.y = PUZZLE_SIZE_Y / divTable[divID];

	//blankID = rand() % (divTable[divID] * divTable[divID]);
	blankID = divTable[divID] * divTable[divID] - 1;
	blankAnimCount = 0;
	moveID = NON_ID;
	moveSpeed = moveSpeedTable[divID];

	autoID = 0;
	autoFlag = false;
	xLineLast = false;
	lineNumY = 0;
	lineNumX = 0;
	blankMove = false;
	blankMoveCnt = 0;
	jamaFlag = false;
	autoLastLine = false;
	cycleFlag = false;

	replayFlag = false;

	PanelShuffle();

	clickCnt = 0;

	sceneFunc = GameScene;

	return rtnFlag;
}

// �ްѼ�ݏ���
bool GameScene(void)
{
	mouseChip.x = (mouseF.pos.x / chipSize.x);
	mouseChip.y = (mouseF.pos.y / chipSize.y);

	PanelCtl();
	if (autoFlag && moveID == NON_ID)
	{
		if (blankMove)
		{
			BlankMove();
		}
		else if (xLineLast)
		{
			XLineLastMove();
		}
		else if (yLineLast)
		{
			YLineLastMove();
		}
		else if (jamaFlag)
		{
			JamaMove();
		}
		else if (autoLastLine)
		{
			LastLineMove();
		}
		else if (cycleFlag)
		{
			CycleMove();
		}
		else
		{
			AutoClear();
		}
	}	

	GameDraw();

	blankAnimCount++;
	
	if (moveID == NON_ID)
	{
		if (isClear())
		{
			autoFlag = false;
			xLineLast = false;
			blankMove = false;
			jamaFlag = false;
			autoLastLine = false;
			cycleFlag = false;
			ClearInit();

			return true;
		}
	}

	if (CheckHitKey(KEY_INPUT_SPACE) && !autoFlag)
	{
		autoFlag = true;
	}
	else if(CheckHitKey(KEY_INPUT_X) && autoFlag)
	{
		autoFlag = false;
	}
	else
	{
		// �������Ȃ�
	}
	return false;
}

// �ްѼ�ݕ`��
void GameDraw(void)
{
	ClsDrawScreen();

	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] != blankID && pzData[y][x] != moveID)
			{
				if (pzData[y][x] != y * divTable[divID] + x)
				{
					SetDrawBright(200, 200, 200);
				}
				DrawRectGraph(x * chipSize.x, y * chipSize.y,
					pzData[y][x] % divTable[divID] * chipSize.x, pzData[y][x] / divTable[divID] * chipSize.y,
					chipSize.x, chipSize.y, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
				SetDrawBright(255, 255, 255);
			}
			else if (moveID == -1 && pzData[y][x] == blankID)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 - (int)(cos((double)blankAnimCount / 20) * 100));

				DrawBox(x * chipSize.x, y * chipSize.y,
					(x + 1) * chipSize.x, (y + 1) * chipSize.y,
					0x0000ff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{

			}
		}
	}

	if (mouseChip.x >= 0 && mouseChip.x < divTable[divID] &&
		mouseChip.y >= 0 && mouseChip.y < divTable[divID] &&
		pzData[mouseChip.y][mouseChip.x] != blankID && !replayFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(mouseChip.x * chipSize.x, mouseChip.y * chipSize.y,
			(mouseChip.x + 1) * chipSize.x, (mouseChip.y+ 1) * chipSize.y,
			0xaaaa00, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawDivLine();

	// �ړ����ٕ`��
	if (moveID != NON_ID)
	{
		SetDrawBright(200, 200, 200);
		DrawRectGraph(movePanelPos.x, movePanelPos.y,
			moveID % divTable[divID] * chipSize.x, moveID / divTable[divID] * chipSize.y,
			chipSize.x, chipSize.y, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
		SetDrawBright(255, 255, 255);
		DrawBox(movePanelPos.x, movePanelPos.y,
			movePanelPos.x + chipSize.x + 1, movePanelPos.y + chipSize.y + 1, 0xffff00, false);

	}

	// ���{�\��
	DrawRotaGraph(SCREEN_SIZE_X - (SCREEN_SIZE_X - PUZZLE_SIZE_X) / 2,
		SCREEN_SIZE_Y / 2, 0.5, 0, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
	DrawFormatString(PUZZLE_SIZE_X, 0, 0xffffff, "blankID:%d", blankID);

	// �萔�\��
	for (int i = 0; i < SCR_MAX; i++)
	{
		DrawFormatString(PUZZLE_SIZE_X, 32 + 16 * i, 0xffffff, "%d : %d",i + 1, minClickCount[i]);
	}
	DrawFormatString(PUZZLE_SIZE_X, 16, 0xffffff, "Cnt : %d", clickCnt);

	ScreenFlip();
}

// �������`��
void DrawDivLine(void)
{
	for (int i = 0; i < divTable[divID] + 1; i++)
	{
		int lineX = PUZZLE_SIZE_X / divTable[divID] * i;
		if (i == divTable[divID])
		{
			lineX--;
		}
		DrawLine(lineX, 0, lineX, PUZZLE_SIZE_Y, 0xffff00);
	}

	for (int i = 0; i < divTable[divID] + 1; i++)
	{
		int lineY = PUZZLE_SIZE_Y / divTable[divID] * i;
		if (i == divTable[divID])
		{
			lineY--;
		}
		DrawLine(0, lineY, PUZZLE_SIZE_X, lineY, 0xffff00);
	}
}

// ���ٓ���ւ�����
void PanelCtl(void)
{
	if (MovePanel())
	{
		// ���د����ꂽ�Ƃ������ڲӰ�ނ̂Ƃ�
		if (((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT) || replayFlag) && !autoFlag)
		{
			if (!replayFlag)
			{
				SetMovePanel(mouseChip, DIR_LEFT);
			}
			else
			{
				SetMovePanel(ptrReplay->pos, DIR_LEFT);
			}		
		}
	}
}

// ���ق̓�������������������Ă��邩���ׂ�
void SetMovePanel(Pos panel, DIR moveDir)
{
	if (moveDir >= DIR_MAX)
	{
		return;
	}

	if (panel.x < 0 || panel.x >= divTable[divID] || panel.y < 0 || panel.y >= divTable[divID])
	{
		// ���ق̏ザ��Ȃ������画�蒆�~
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	Pos nextPanel = panel;
	
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPanel.x--;
		break;

	case DIR_RIGHT:
		nextPanel.x++;
		break;

	case DIR_UP:
		nextPanel.y--;
		break;

	case DIR_DOWN:
		nextPanel.y++;
		break;

	default:
		AST();
		break;
	}

	if (nextPanel.x < 0 || nextPanel.x >= divTable[divID] || nextPanel.y < 0 || nextPanel.y >= divTable[divID])
	{
		// �����悪�͈͊O�������画�蒆�~
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	if (pzData[nextPanel.y][nextPanel.x] != blankID)
	{
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	moveID = pzData[panel.y][panel.x];
	movePanelPos.x = panel.x * chipSize.x;
	movePanelPos.y = panel.y * chipSize.y;
	moveEndPos.x = nextPanel.x * chipSize.x;
	moveEndPos.y = nextPanel.y * chipSize.y;

	if (!replayFlag)
	{
		clickCnt++;
		if (ptrReplay == NULL)
		{
			ptrReplay = (ReplayData*)malloc(sizeof(ReplayData));
			ReplayList = ptrReplay;
		}
		else
		{
			ptrReplay->next = (ReplayData*)malloc(sizeof(ReplayData));
			ptrReplay = ptrReplay->next;
		}
		ptrReplay->next = NULL;
		ptrReplay->pos.x = panel.x;
		ptrReplay->pos.y = panel.y;
	}
	else
	{
		if (ptrReplay->next != NULL)
		{
			ptrReplay = ptrReplay->next;
		}
	}

	SwapNum(&pzData[nextPanel.y][nextPanel.x], &pzData[panel.y][panel.x]);
	blank.x = panel.x;
	blank.y = panel.y;
}

// ���ٓ���ւ�����
void SwapNum(int *panelID, int *nextPanelID)
{
	int tmpID = *panelID;
	*panelID = *nextPanelID;
	*nextPanelID = tmpID;
}

// ���وړ�����
bool MovePanel(void)
{
	if (moveID == NON_ID)
	{
		return true;
	}

	if (moveEndPos.x - movePanelPos.x != 0)
	{
		if (moveEndPos.x - movePanelPos.x > 0)
		{
			movePanelPos.x += moveSpeed;
		}
		else
		{
			movePanelPos.x -= moveSpeed;
		}
	}
	else if (moveEndPos.y - movePanelPos.y != 0)
	{
		if (moveEndPos.y - movePanelPos.y > 0)
		{
			movePanelPos.y += moveSpeed;
		}
		else
		{
			movePanelPos.y -= moveSpeed;
		}
	}
	else
	{
		// �ړ��I��
		moveID = NON_ID;
		blankAnimCount = 0;

		return true;
	}

	return false;
}

// ���ټ���ُ���
void PanelShuffle(void)
{
	Pos blankPanel;					// �������ق̍��W
	bool getBlankFlag = false;		// �������ق��擾������
	DIR swapDir;					// �����������
	bool getDirFlag = false;		// �����ł����������������

	// �������ق̍��W���擾
	for (int y = 0; y < divTable[divID]; y++)
	{
		if (getBlankFlag)
		{
			break;
		}

		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPanel = { x,y };
				getBlankFlag = true;
				break;
			}
		}
	}

	if (!getBlankFlag)
	{
		AST();
		return;
	}

	Pos nextPanel = blankPanel;
	int shuffleCnt = 10000;

	for (int i = 0; i < shuffleCnt; i++)
	{
		// ������������
		while (!getDirFlag)
		{
			swapDir = (DIR)(rand() % (int)DIR_MAX);

			switch (swapDir)
			{
			case DIR_LEFT:
				if ((nextPanel.x - 1) >= 0)
				{
					nextPanel.x--;
					getDirFlag = true;
				}
				break;

			case DIR_RIGHT:
				if ((nextPanel.x + 1) < divTable[divID])
				{
					nextPanel.x++;
					getDirFlag = true;
				}
				break;

			case DIR_UP:
				if ((nextPanel.y - 1) >= 0)
				{
					nextPanel.y--;
					getDirFlag = true;
				}
				break;

			case DIR_DOWN:
				if ((nextPanel.y + 1) < divTable[divID])
				{
					nextPanel.y++;
					getDirFlag = true;
				}
				break;

			default:
				AST();
				break;
			}
		}

		SwapNum(&pzData[nextPanel.y][nextPanel.x], &pzData[blankPanel.y][blankPanel.x]);

		blankPanel = nextPanel;
		getDirFlag = false;
	}

	if (isClear())
	{
		// ���łɸر���Ă���ċA
		PanelShuffle();
	}
}

// ��؊J��
void FreeMemData()
{
	// ����
	if (pzData != NULL)
	{
		free(pzData[0]);
	}
	free(pzData);

	pzData = NULL;

	// ؽ�
	ptrReplay = ReplayList;
	if (ptrReplay != NULL)
	{
		ReplayData* nextData;		// ������鎟���߲�����L��
		do
		{
			nextData = ptrReplay->next;
			free(ptrReplay);
			ptrReplay = nextData;
		} while (nextData != NULL);
	}

	ReplayList = NULL;
	ptrReplay = NULL;
}

// �ر����
bool isClear(void)
{
	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] != (y * divTable[divID] + x))
			{
				return false;
			}
		}
	}

	return true;
}

// ================================�������牺�������N���A�֌W�A�߂����ቘ��==========================================

// �����N���A
void AutoClear(void)
{
	if (autoID >= divTable[divID] * (divTable[divID] - 2) && !autoLastLine)
	{
		autoLastLine = true;
		LastLineMove();
		return;
	}

	lineNumY = autoID / divTable[divID];

	Pos autoPos = { divTable[divID],divTable[divID] };		// autoID�̏ꏊ,�����l�͕������ɐݒ�

	// autoID��T��,
	for (int y = (autoID / divTable[divID]); y < divTable[divID]; y++)
	{
		if (autoPos.x != divTable[divID])
		{
			// ���W���ς���Ă���T���I��
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == autoID)
			{
				autoPos = { x,y };
				break;
			}
		}
	}

	if (autoPos.x == divTable[divID])
	{
		AST();
	}

	// �ړI�n�ݒ�
	Pos desPos = { autoID % divTable[divID] ,autoID / divTable[divID] };
	if (autoID % divTable[divID] == divTable[divID] - 2)
	{
		desPos.x += 1;
		if (pzData[desPos.y][desPos.x] == autoID + 1 && autoPos.x == desPos.x && (autoPos.y == desPos.y + 1))
		{
			// �ړI�n�ɉE�[�����ق��������܂܂������疳�����[�v�ɂȂ�
			jamaFlag = true;
			JamaMove();
			return;
		}
	}
	else if (autoID % divTable[divID] == divTable[divID] - 1)
	{
		desPos.y += 1;
	}


	// autoID�����ق𓮂������������߂�
	DIR moveDir;

	if (desPos.x - autoPos.x < 0)
	{
		moveDir = DIR_LEFT;
	}
	else if (desPos.x - autoPos.x > 0)
	{
		moveDir = DIR_RIGHT;
	}
	else if (desPos.y - autoPos.y < 0)
	{
		moveDir = DIR_UP;
	}
	else if (desPos.y - autoPos.y > 0)
	{
		moveDir = DIR_DOWN;
	}
	else
	{
		// ������v���Ă�����
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			// ��̍Ōザ��Ȃ������玟�����ق�
			autoID++;
		}
		else
		{
			// ��̍Ōゾ�����炻�̗�����ꂢ�ɑ�����
			xLineLast = true;
			autoID--;
		}
		return;
	}

	// ���������������ɓ������邩����(�ړ��悪blankID��)

	Pos nextPos;		// �����������ꏊ
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPos = { autoPos.x - 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_LEFT);
			return;
		}
		break;

	case DIR_UP:
		nextPos = { autoPos.x, autoPos.y - 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_UP);
			return;
		}
		break;

	case DIR_RIGHT:
		nextPos = { autoPos.x + 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_RIGHT);
			return;
		}
		break;

	case DIR_DOWN:
		nextPos = { autoPos.x, autoPos.y + 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_DOWN);
			return;
		}
		break;

	default:
		AST();
		break;
	}

	// ����������������blankID���Ȃ������ꍇ
	// blankID��T��
	Pos blankPos = { divTable[divID],divTable[divID] };
	DIR moveDir2;

	for (int y = (autoID / divTable[divID]); y < divTable[divID]; y++)
	{
		if (blankPos.x != divTable[divID])
		{
			// ���W���ς���Ă���T���I��
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPos = { x,y };
				break;
			}
		}
	}

	blank = blankPos;

	// blankID�����ق𓮂����������������߂ē�����

	/*
	 * �@7/22(��)����
	 * �Z���ɂ�邱��
	 *  �E�������ق𓮂����Ƃ���autoID�����ق𓮂����Ȃ��悤�ɂ���
	 *�@�@���ړ��悪autoID����������ʂȈړ�Ӱ�ނɈڍs
	 *�@�E��L���ł�����ŏ��̈�������Ɠ�����ý�
	 *�@�E��̍Ō�̓�����낦��̂ɓ��ʂȈړ��@�����
	 *�@�@���Ō�̓�͖ړI�n�����点�΂����̂ł́H
	 */

	if (nextPos.x - blankPos.x < 0)
	{
		// blankID�����ق̍��ɂ������ق���������
		blankPos.x -= 1;
		blankMoveDir = DIR_LEFT;
		moveDir2 = DIR_RIGHT;
	}
	else if (nextPos.x - blankPos.x > 0)
	{
		blankPos.x += 1;
		blankMoveDir = DIR_RIGHT;
		moveDir2 = DIR_LEFT;
	}
	else if (nextPos.y - blankPos.y < 0)
	{
		blankPos.y -= 1;
		blankMoveDir = DIR_UP;
		moveDir2 = DIR_DOWN;
	}
	else if (nextPos.y - blankPos.y > 0)
	{
		blankPos.y += 1;
		blankMoveDir = DIR_DOWN;
		moveDir2 = DIR_UP;
	}
	else
	{
		// ������v���Ă����ԁA���Ȃ��͂�
		AST();
		return;
	}

	// �͈�����
	if (blankPos.x >= divTable[divID] || blankPos.y >= divTable[divID])
	{
		AST();
		return;
	}

	// �ړ��悪autoID,�������͂��łɂ��낦�����ق��������AautoID��������blankMove
	if (pzData[blankPos.y][blankPos.x] > autoID)
	{
		SetMovePanel(blankPos, moveDir2);
		if (pzData[desPos.y][desPos.x] == autoID)
		{
			//�ړI�n�ɓ������Ă���autoID��i�߂�
			if (autoID % divTable[divID] != divTable[divID] - 1)
			{
				// ��̍Ōザ��Ȃ������玟�����ق�
				if (!autoLastLine)
				{
					autoID++;
				}
				else if (lineNumY == divTable[divID] - 2)
				{
					autoID += divTable[divID];
				}
				else
				{
					// �c�̓�����ړ�������
					yLineLast = true;
				}
			}
			else
			{
				// ��̍Ōゾ�����炻�̗�����ꂢ�ɑ�����
				xLineLast = true;
				lineNumY = autoID / divTable[divID];
				autoID--;
			}
		}
	}
	else if (pzData[blankPos.y][blankPos.x] == autoID && moveDir == DIR_UP)
	{
		if (pzData[blank.y - 1][blank.x] > autoID)
		{
			blankPos = blank;
			blankPos.y -= 1;
			SetMovePanel(blankPos, DIR_DOWN);
		}
		else
		{
			blankMove = true;
			blankMoveCnt = 0;
		}
	}
	else if (pzData[blankPos.y][blankPos.x] != autoID && moveDir2 == DIR_RIGHT)
	{
		blankPos = blank;
		blankPos.y += 1;
		SetMovePanel(blankPos, DIR_UP);
	}
	else
	{
		blankMove = true;
		blankMoveCnt = 0;
	}
}

// �������ق�autoID�����ق𓮂������������ɂ����Ă���
void BlankMove(void)
{
 	Pos movePos = blank;
	DIR moveDir;
	if (blankMoveDir == DIR_LEFT || blankMoveDir == DIR_RIGHT)
	{
		switch (blankMoveCnt)
		{
		case 0:
			if (blank.y != divTable[divID] - 1)
			{
				movePos.y += 1;
				avoDir = DIR_DOWN;
				moveDir = DIR_UP;
			}
			else
			{
				movePos.y -= 1;
				avoDir = DIR_UP;
				moveDir = DIR_DOWN;
			}
			break;

		case 1:
		case 2:
			if (blankMoveDir == DIR_LEFT)
			{
				movePos.x -= 1;
				moveDir = DIR_RIGHT;
			}
			else
			{
				movePos.x += 1;
				moveDir = DIR_LEFT;
			}
			break;

		case 3:
			if (avoDir == DIR_DOWN)
			{
				movePos.y -= 1;
				moveDir = DIR_DOWN;
			}
			else
			{
				movePos.y += 1;
				moveDir = DIR_UP;
			}
			break;

		default:
			break;
		}
	}
	else
	{
		switch (blankMoveCnt)
		{
		case 0:
			if (blank.x != divTable[divID] - 1)
			{
				movePos.x += 1;
				avoDir = DIR_RIGHT;
				moveDir = DIR_LEFT;
			}
			else
			{
				movePos.x -= 1;
				avoDir = DIR_LEFT;
				moveDir = DIR_RIGHT;
			}
			break;

		case 1:
		case 2:
			if (blankMoveDir == DIR_UP)
			{
				movePos.y -= 1;
				moveDir = DIR_DOWN;
			}
			else
			{
				movePos.y += 1;
				moveDir = DIR_UP;
			}
			break;

		case 3:
			if (avoDir == DIR_RIGHT)
			{
				movePos.x -= 1;
				moveDir = DIR_RIGHT;
			}
			else
			{
				movePos.x += 1;
				moveDir = DIR_LEFT;
			}
			break;

		default:
			break;
		}
	}

	if (pzData[movePos.y][movePos.x] > autoID || autoLastLine)
	{
		SetMovePanel(movePos, moveDir);
		blank = movePos;
		blankMoveCnt++;
		if (blankMoveCnt >= 4)
		{
			blankMove = false;
		}
	}
	else
	{
		AST();
		return;
	}
}

// ����̍Ō�����낦����
void XLineLastMove(void)
{
	if (autoID % divTable[divID] == divTable[divID] - 1)
	{
		SetMovePanel({ divTable[divID] - 1 , lineNumY + 1 }, DIR_UP);
		xLineLast = false;
		autoID++;
		return;
	}
	else if (autoID % divTable[divID] == divTable[divID] - 2 && pzData[lineNumY][divTable[divID] - 2] == blankID)
	{
		SetMovePanel({ divTable[divID] - 1 , lineNumY}, DIR_LEFT);
		autoID++;
		return;
	}
	else
	{
		if (pzData[lineNumY + 1][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 , lineNumY }, DIR_DOWN);
			return;
		}
	}

	// blankID�����낦������̍Ō�̍����ɂ����Ă�������
	if (pzData[lineNumY + 2][divTable[divID] - 2] == blankID)
	{
		SetMovePanel({ divTable[divID] - 2 , lineNumY  + 1}, DIR_DOWN);
	}
	else
	{
		SetMovePanel({ divTable[divID] - 2 , lineNumY + 2 }, DIR_RIGHT);
	}
}

// ��̉E�[�̂���ז�
void JamaMove(void)
{
	if (!autoLastLine)
	{
		if (pzData[lineNumY + 1][divTable[divID] - 2] == blankID)
		{
			if (pzData[lineNumY + 1][divTable[divID] - 1] == autoID)
			{
				SetMovePanel({ divTable[divID] - 2 ,lineNumY + 2 }, DIR_UP);
			}
			else if (pzData[lineNumY + 1][divTable[divID] - 1] == autoID + 1)
			{
				SetMovePanel({ divTable[divID] - 1 ,lineNumY + 1 }, DIR_LEFT);
				jamaFlag = false;
			}
			else
			{
				AST();
			}
		}
		else if (pzData[lineNumY + 2][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY + 2 }, DIR_LEFT);
		}
		else if (pzData[lineNumY + 2][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY + 1 }, DIR_DOWN);
		}
		else if (pzData[lineNumY + 1][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY }, DIR_DOWN);
		}
		else if (pzData[lineNumY][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 ,lineNumY }, DIR_RIGHT);
		}
		else if (pzData[lineNumY][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 ,lineNumY + 1 }, DIR_UP);
		}
		else
		{
			AST();
			lineNumY = autoID / divTable[divID];
			jamaFlag = false;
		}
	}
	else
	{
		if (pzData[divTable[divID] - 2][lineNumX + 1] == blankID)
		{
			if (pzData[divTable[divID] - 1][lineNumX + 1] == (autoID + divTable[divID]))
			{
				SetMovePanel({ lineNumX + 1,divTable[divID] - 1 }, DIR_LEFT);
				jamaFlag = false;
			}
			else
			{
				SetMovePanel({ lineNumX + 2,divTable[divID] - 2 }, DIR_LEFT);
			}
		}
		else if (pzData[divTable[divID] - 2][lineNumX + 2] == blankID)
		{
			SetMovePanel({ lineNumX + 2,divTable[divID] - 1}, DIR_UP);
		}
		else if (pzData[divTable[divID] - 1][lineNumX + 2] == blankID)
		{
			SetMovePanel({ lineNumX + 1,divTable[divID] - 1 }, DIR_RIGHT);
		}
		else if (pzData[divTable[divID] - 1][lineNumX + 1] == blankID)
		{
			SetMovePanel({ lineNumX ,divTable[divID] - 1 }, DIR_RIGHT);
		}
		else if (pzData[divTable[divID] - 1][lineNumX] == blankID)
		{
			SetMovePanel({ lineNumX ,divTable[divID] - 2 }, DIR_DOWN);
		}
		else if (pzData[divTable[divID] - 2][lineNumX] == blankID)
		{
			SetMovePanel({ lineNumX  + 1,divTable[divID] - 2 }, DIR_LEFT);
		}
		else
		{
			AST();
			jamaFlag = false;
		}
	}
}

// �c��̍Ō�𑵂�����
void YLineLastMove(void)
{
	if (pzData[divTable[divID] - 1][lineNumX + 2] == blankID)
	{
		SetMovePanel({ lineNumX + 2 , divTable[divID] - 2 }, DIR_DOWN);
	}
	else if (pzData[divTable[divID] - 2][lineNumX + 2] == blankID)
	{
		SetMovePanel({ lineNumX + 1 , divTable[divID] - 2 }, DIR_RIGHT);
	}
	else if (pzData[divTable[divID] - 2][lineNumX + 1] == blankID)
	{
		SetMovePanel({ lineNumX, divTable[divID] - 2 }, DIR_RIGHT);
	}
	else if (pzData[divTable[divID] - 2][lineNumX] == blankID)
	{
		SetMovePanel({ lineNumX, divTable[divID] - 1 }, DIR_UP);
	}
	else if (pzData[divTable[divID] - 1][lineNumX] == blankID)
	{
		// �����ł����
		SetMovePanel({ lineNumX + 1, divTable[divID] - 1}, DIR_LEFT);
		autoID = (divTable[divID] - 2) * divTable[divID] + (++lineNumX);
		if (lineNumX >= divTable[divID] - 2)
		{
			autoFlag = false;
			cycleFlag = true;
		}
		yLineLast = false;
	}
	else
	{
		AST();
	}
}

// �Ō�̗�𑵂�����
void LastLineMove(void)
{
	Pos autoPos = { divTable[divID],divTable[divID] };		// autoID�̏ꏊ,�����l�͕������ɐݒ�

	// autoID��T��,
	for (int y = divTable[divID] - 2; y < divTable[divID]; y++)
	{
		if (autoPos.x != divTable[divID])
		{
			// ���W���ς���Ă���T���I��
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == autoID)
			{
				autoPos = { x,y };
				break;
			}
		}
	}

	if (autoPos.x == divTable[divID])
	{
		AST();
		autoFlag = false;
		autoLastLine = false;
		return;
	}

	lineNumY = autoID / divTable[divID];

	// �ړI�n�ݒ�
	Pos desPos = { autoID % divTable[divID] ,autoID / divTable[divID] };

	if (lineNumY == divTable[divID] - 2)
	{
		if (pzData[desPos.y][desPos.x] == autoID)
		{
			// �{���̖ړI�n�ɒu���Ă��������]
			cycleFlag = true;
			CycleMove();

			return;
		}
		desPos.y += 1;
		if ((pzData[desPos.y][desPos.x] == autoID + divTable[divID] &&
			autoPos.x == desPos.x + 1 && autoPos.y == desPos.y))
		{
			// �ړI�n�ɉE�[�����ق��������܂܂������疳�����[�v�ɂȂ�
			jamaFlag = true;
			JamaMove();
			return;
		}
	}
	else
	{
		desPos.x += 1;
	}

	// autoID�����ق𓮂������������߂�
	DIR moveDir;

	if (desPos.y - autoPos.y > 0)
	{
		moveDir = DIR_DOWN;
	}
	else if (desPos.x - autoPos.x < 0)
	{
		moveDir = DIR_LEFT;
	}
	else
	{
		// ������v���Ă�����
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			if (lineNumY == divTable[divID] - 2)
			{
				autoID += divTable[divID];
			}
			else
			{
				// �c�̓�����ړ�������
				yLineLast = true;
			}
		}
		return;
	}

	// ���������������ɓ������邩����(�ړ��悪blankID��)

	Pos nextPos;		// �����������ꏊ
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPos = { autoPos.x - 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_LEFT);
			return;
		}
		break;

	case DIR_UP:
		nextPos = { autoPos.x, autoPos.y - 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_UP);
			return;
		}
		break;

	case DIR_RIGHT:
		nextPos = { autoPos.x + 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_RIGHT);
			return;
		}
		break;

	case DIR_DOWN:
		nextPos = { autoPos.x, autoPos.y + 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_DOWN);
			return;
		}
		break;

	default:
		AST();
		break;
	}

	// ����������������blankID���Ȃ������ꍇ
	// blankID��T��
	Pos blankPos = { divTable[divID],divTable[divID] };
	DIR moveDir2;

	for (int y = divTable[divID] - 2; y < divTable[divID]; y++)
	{
		if (blankPos.x != divTable[divID])
		{
			// ���W���ς���Ă���T���I��
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPos = { x,y };
				break;
			}
		}
	}

	blank = blankPos;

	// blankID�����ق𓮂����������������߂ē�����
	// �܂�blank����̒i�ɂ������牺�ɉ�����
	if (blank.y == divTable[divID] - 2 && pzData[blank.y + 1][blank.x] != autoID)
	{
		SetMovePanel({ blank.x,blank.y + 1 }, DIR_UP);
		return;
	}
	else if (nextPos.x - blankPos.x < 0)
	{
		// blankID�����ق̍��ɂ������ق���������
		blankPos.x -= 1;
		blankMoveDir = DIR_LEFT;
		moveDir2 = DIR_RIGHT;
	}
	else if (nextPos.x - blankPos.x > 0)
	{
		blankPos.x += 1;
		blankMoveDir = DIR_RIGHT;
		moveDir2 = DIR_LEFT;
	}
	else
	{
		// ������v���Ă����ԁA���Ȃ��͂�
		AST();
		return;
	}

	// �͈�����
	if (blankPos.x >= divTable[divID] || blankPos.y >= divTable[divID])
	{
		AST();
		return;
	}

	// �ړ��悪autoID,�������͂��łɂ��낦�����ق��������AautoID��������blankMove
	if (pzData[blankPos.y][blankPos.x] != autoID)
	{
		SetMovePanel(blankPos, moveDir2);
		
	}
	else if (pzData[desPos.y][desPos.x] == autoID)
	{
		//�ړI�n�ɓ������Ă���autoID��i�߂�
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			if (lineNumY == divTable[divID] - 2)
			{
				autoID += divTable[divID];
			}
			else
			{
				// �c�̓�����ړ�������
				yLineLast = true;
			}
		}
	}
	else
	{
		blankMove = true;
		blankMoveCnt = 0;
	}
}

// ��]Ӱ��
void CycleMove(void)
{
	if (blank.x == lineNumX)
	{
		if (blank.y == divTable[divID] - 1)
		{
			SetMovePanel({ blank.x + 1,blank.y }, DIR_LEFT);

			// ����
			if (pzData[blank.y][blank.x - 1] == autoID)
			{
				cycleFlag = false;
				autoID += divTable[divID];
			}
		}
		else
		{
			SetMovePanel({ blank.x,blank.y + 1}, DIR_UP);
		}
	}
	else
	{
		if (blank.y == divTable[divID] - 1)
		{
			SetMovePanel({ blank.x,blank.y - 1}, DIR_DOWN);
		}
		else
		{
			SetMovePanel({ blank.x - 1,blank.y }, DIR_RIGHT);
		}
	}
}