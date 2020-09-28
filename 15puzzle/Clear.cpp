#include "DxLib.h"
#include "main.h"
#include "Game.h"
#include "Clear.h"
#include "Select.h"

// ∏ÿ±º∞›èâä˙âª
bool ClearInit(void)
{
	bool rtnFlag = true;

	// ç≈íZéËêîçXêV
	for (int i = SCR_MAX - 1; i >= 0; i--)
	{
		if (minClickCount[i] > clickCnt)
		{
			if (i != SCR_MAX - 1)
			{
				minClickCount[i + 1] = minClickCount[i];
			}
			minClickCount[i] = clickCnt;
		}
	}

	FILE *fp;
	if (fopen_s(&fp, "score.dat", "wb") == 0)
	{
		fwrite(minClickCount, sizeof(minClickCount[0]), SCR_MAX, fp);
	}
	fclose(fp);

	sceneFunc = ClearScene;;

	return rtnFlag;
}

// ∏ÿ±º∞›èàóù
bool ClearScene(void)
{
	if (replayFlag)
	{
		PanelCtl();
		if (moveID == NON_ID)
		{
			if (isClear())
			{
				replayFlag = false;
			}
		}
	}
	else
	{
		if (OnClickCircle())
		{
			SelectInit();
			return true;
		}


		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			srand(shuffleSeed);
			rand();					// Ç±ÇÍÇÕblankIDÇÃï™
			PanelShuffle();
			ptrReplay = ReplayList;
			moveSpeed = moveSpeedTable[divID] * 2;
			replayFlag = true;
		}
	}
	
	ClearDraw();
	return false;
}

// ∏ÿ±º∞›ï`âÊ
void ClearDraw(void)
{
	if (!replayFlag)
	{
		ClsDrawScreen();
		DrawGraph(0, 0, puzzleImg[pzlImageID[PZL_SEL_NOW]], false);
		DrawButton();
		ScreenFlip();
	}
	else
	{
		GameDraw();
	}
}