#include "DxLib.h"
#include "main.h"
#include "TitleScene.h"
#include "GameScene.h"

int bgImg;			// �w�i�摜ID
int logoImg;		// ����ۺމ摜
int startImg;		// ���ټ�݂̑���w�������摜

// ���ټ�ݏ�����
bool TitleInit(void)
{
	bool rtnFlg = true;

	// �摜�ǂݍ���
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

// ���ټ�ݏ���
void TitleScene(void)
{
	DrawTitle();

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		GameInit();
	}
}

// ���ټ�ݕ`��
void DrawTitle(void)
{
	ClsDrawScreen();
	// �w�i
	DrawGraph(0, 0,bgImg, false);

	// ۺ�
	DrawGraph((SCREEN_SIZE_X - TITLE_LOGO_SIZE_X) / 2, (SCREEN_SIZE_Y - TITLE_LOGO_SIZE_Y) / 2, logoImg,false);
	ScreenFlip();
}