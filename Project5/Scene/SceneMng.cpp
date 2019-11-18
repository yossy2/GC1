#include <algorithm>
#include <DxLib.h>
#include <_DebugDispOut.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include <common\ImageMng.h>

/* ----------シーン管理クラス---------- */

SceneMng *SceneMng::sInstance = nullptr;

SceneMng::SceneMng():ScreenSize{800,600},
					 ScreenCenter{ ScreenSize / 2 },
					 GameScreenSize{500,390},
			         GameScreenOffSet{(ScreenSize - GameScreenSize) / 2}
{
	_frameCnt = 0;
}

SceneMng::~SceneMng()
{

}

void SceneMng::Run(void)
{
	SysInit();
	_activeScene = std::make_unique<TitleScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_drawList.clear();

		lpSceneMng.AddDrawQue({IMAGE_ID("frame")[0],
							   static_cast<double>(ScreenSize.x / 2),
							   static_cast<double>(ScreenSize.y / 2),
							   0.0,
							   0,
							   LAYER::UI
		});
		_dbgStartDraw();

		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		_frameCnt++;
		Draw();
	}
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();

	/*for (auto layer : LAYER())
	{
		SetDrawScreen(_screenID[layer]);
		ClsDrawScreen();
	}*/

	std::sort(_drawList.begin(), _drawList.end(), 
		[](DrawQueT queA, DrawQueT queB){
		return std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queA), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queA)) <
			   std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queB), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queB));
		});

	// 描画スクリーンを初期化
	// レイヤー数があまり多くないのでここで毎フレームやる
	for (auto layer : LAYER())
	{
		SetDrawScreen(_screenID[layer]);
		ClsDrawScreen();
	}

	// たまっているQueをそれぞれのレイヤーに描画する
	for (auto que : _drawList)
	{
		double x, y, rad;
		int id;
		LAYER layer;

		std::tie(id,x,y,rad,std::ignore,layer) = que;

		if (_screenID[layer] != GetDrawScreen())
		{
			SetDrawScreen(_screenID[layer]);
		}

		DrawRotaGraph(
			static_cast<int>(x),
			static_cast<int>(y),
			1.0,
			rad,
			id,
			true
			);
	}

	// それぞれのレイヤーを描画する
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	for (auto layer:LAYER())
	{
		DrawRotaGraph(ScreenCenter.x,
					  ScreenCenter.y,
					  1.0,
					  0.0,
					  _screenID[layer],
					  true
					 );
	}

	ScreenFlip();
}

bool SceneMng::AddDrawQue(DrawQueT dQue)
{
	if (std::get<static_cast<int>(DRAW_QUE::IMAGE)>(dQue) <= 0)
	{
		// 画像IDが不正なので追加しない
		return false;
	}
	// Queを追加
	_drawList.emplace_back(dQue);
	return true;
}

int SceneMng::frameCnt(void)
{
	return _frameCnt;
}

bool SceneMng::SysInit(void)
{
	// ---------- ｼｽﾃﾑ処理
	SetWindowText("Yoshihara");

	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return false;
	}

	// 描画スクリーン作成
	for (auto layer : LAYER())
	{
		_screenID.try_emplace(layer, MakeScreen(ScreenSize.x, ScreenSize.y, true));
	}
	SetDrawScreen(DX_SCREEN_BACK);
	_dbgSetup(200);

	lpImageMng.GetID("frame", "image/frame.png");

	return false;
}


