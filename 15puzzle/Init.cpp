#include "DxLib.h"
#include "main.h"
#include "Init.h"
#include "Select.h"

// ¼½ÃÑŒn‰Šú‰»
bool SysInit(void)
{
	bool rtnFlag = true;

	// ---------- ¼½ÃÑˆ—
	SetWindowText("15PUZZLE_1916047");

	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		AST();
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// ‰æ‘œ‚Ì“Ç‚İ‚İ
	const char* fileNameList[PZL_IMAGE_MAX] = { "image/puzzle1.png" ,"image/puzzle2.png" ,"image/puzzle3.png" };

	for (int i = 0; i < PZL_IMAGE_MAX; i++)
	{
		puzzleImg[i] = LoadGraph(fileNameList[i]);
		if (puzzleImg[i] == -1)
		{
			AST();
			rtnFlag = false;
		}
	}

	buttonImg = LoadGraph("image/button2.png");
	if (buttonImg == -1)
	{
		AST();
		rtnFlag = false;
	}	

	FILE *fp = NULL;
	if (fopen_s(&fp, "score.dat", "rb") != 0)
	{
		for (int i = 0; i < SCR_MAX; i++)
		{
			minClickCount[i] = 9999;
		}
	}
	else
	{
		fread(minClickCount,sizeof(minClickCount[0]),SCR_MAX,fp );
	}
	fclose(fp);

	if (!SelectInit())
	{
		AST();
		return false;
	}

	return rtnFlag;
}