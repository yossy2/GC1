#include <DxLib.h>
#include <_DebugConOut.h>
#include <Scene/SceneMng.h>
#include <ImageMng.h>
#include "InputState.h"
#include "TitleScene.h"
#include "GameScene.h"

TitleScene::TitleScene()
{
	lpImageMng.GetID("title", "image/title.png");
	TRACE("TitleScene\n");
}


TitleScene::~TitleScene()
{
}

unique_Base TitleScene::Update(unique_Base own)
{
	lpSceneMng.AddDrawQue({ IMAGE_ID("title")[0],
		static_cast<double>(lpSceneMng.ScreenCenter.x),
		static_cast<double>(lpSceneMng.ScreenCenter.y),
		0.0,
		10,
		LAYER::UI,
		DX_BLENDMODE_NOBLEND,
		255
		});

 	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}
