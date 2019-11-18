#include "DxLib.h"
#include "main.h"
#include "TitleScene.h"
#include "GameScene.h"

int bgImg;			// ”wŒi‰æ‘œID
int logoImg;		// À²ÄÙÛºŞ‰æ‘œ
int startImg;		// À²ÄÙ¼°İ‚Ì‘€ìw¦•¶š‰æ‘œ

// À²ÄÙ¼°İ‰Šú‰»
bool TitleInit(void)
{
	bool rtnFlg = true;

	// ‰æ‘œ“Ç‚İ‚İ
	bgImg = LoadGraph("image/titleBG.png");
	if (bgImg == -1)
	{
		AST();
		rtnFlg = false;
	}

	logoImg = LoadGraph("image/titleLogo.png");
	if (bgImg == -1)
	{
		AST();
		rtnFlg = false;
	}

	startImg = LoadGraph("image/start.png");
	if (bgImg == -1)
	{
		AST();
		rtnFlg = false;
	}

	sceneFunc = TitleScene;

	return rtnFlg;
}

// À²ÄÙ¼°İˆ—
void TitleScene(void)
{
	DrawTitle();

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		GameInit();
	}
}

// À²ÄÙ¼°İ•`‰æ
void DrawTitle(void)
{
	ClsDrawScreen();
	// ”wŒi
	DrawGraph(0, 0,bgImg, false);

	// ÛºŞ
	DrawGraph((SCREEN_SIZE_X - TITLE_LOGO_SIZE_X) / 2, (SCREEN_SIZE_Y - TITLE_LOGO_SIZE_Y) / 2, logoImg,false);
	ScreenFlip();
}