#include <DxLib.h>
#include <_DebugConOut.h>
#include "InputState.h"
#include "TitleScene.h"
#include "GameScene.h"

TitleScene::TitleScene()
{
	TRACE("TitleScene\n");
}


TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
{
 	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}
