#include "DxLib.h"
#include "main.h"
#include "Game.h"
#include "Select.h"
#include "Mouse.h"

int selScreen[PZL_SEL_MAX];		// ｾﾚｸﾄ用の画像の描画先
int selPzScreen;				// 上記を一つにまとめた描画先
SEL_MOVE selMoveMode;			// 画像が流れる方向

int selPanelMove;				// 画像移動用ｶｳﾝﾀ

// 選択ｼｰﾝ初期化
bool SelectInit(void)
{
	bool rtnFlag = true;

	divID = 0;

	buttonPos.x = SCREEN_SIZE_X - (SCREEN_SIZE_X - PUZZLE_SIZE_X) / 2;
	buttonPos.y = SCREEN_SIZE_Y - BUTTON_SIZE_Y / 2 - 10;			// 10は余白
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

// 選択ｼｰﾝ処理
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

	// ｹﾞｰﾑ開始ﾁｪｯｸ
	if (OnClickCircle())
	{
		GameInit();

		// 画像開放
		DeleteGraph(selScreen[0]);
		DeleteGraph(selScreen[1]);
		DeleteGraph(selPzScreen);
		return true;
	}	

	if (buttonFlag)
	{
		return false;
	}

	// 左ｸﾘｯｸ
	if ((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT))
	{
		if (divID < (sizeof(divTable) / sizeof(divTable[0]) - 1))
		{
			divID++;
		}
	}

	// 右ｸﾘｯｸ
	if ((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_RIGHT))
	{
		if (divID > 0)
		{
			divID--;
		}
	}

	// 画像選択
	// 右に流れる
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

	// 左に流れる
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

// 選択ｼｰﾝ描画
void SelectDraw(void)
{
	// 選択中の画像と分割線をセットにした画像を作る
	SetDrawScreen(selScreen[PZL_SEL_NOW]);
	ClsDrawScreen();
	DrawGraph(0, 0, puzzleImg[pzlImageID[PZL_SEL_NOW]], false);
	DrawDivLine();

	if (selPanelMove != 0)
	{
		// 選択切り替え中
		// ひとつ前の選択画像と分割線をセットにした画像を作る
		SetDrawScreen(selScreen[PZL_SEL_OLD]);
		ClsDrawScreen();
		DrawGraph(0, 0, puzzleImg[pzlImageID[PZL_SEL_OLD]], false);
		DrawDivLine();
	}

	// 上で作った画像を合体
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

	// ﾊﾞｯｸﾊﾞｯﾌｧに描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(0, 0, selPzScreen, false);
	DrawButton();
	DrawFormatString(320, 0, 0xffffff, "div : %d", divTable[divID]);
	ScreenFlip();
}

// ﾎﾞﾀﾝの描画
void DrawButton()
{
	DrawRectRotaGraph(buttonPos.x, buttonPos.y, buttonFlag * BUTTON_SIZE_X, 0, BUTTON_SIZE_X, BUTTON_SIZE_Y, 1.0, 0.0, buttonImg, true,false);
}

// ﾏｳｽがﾎﾞﾀﾝの上にあるかﾁｪｯｸ
bool OnClickCircle()
{
	// ﾎﾞﾀﾝの中心からﾏｳｽの距離の二乗
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