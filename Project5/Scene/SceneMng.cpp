#include <cstdlib>
#include <time.h>
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
							   static_cast<double>(ScreenCenter.x),
							   static_cast<double>(ScreenCenter.y),
							   0.0,
							   0,
							   LAYER::UI,
							   DX_BLENDMODE_NOBLEND,
							   255
		});
		_dbgStartDraw();
		(*_activeScene).RunActQue(std::move(_actList));
		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		_frameCnt++;
		Draw();
	}
}

void SceneMng::Draw(void)
{
	_dbgAddDraw();

	std::sort(_drawList.begin(), _drawList.end(), 
		[](DrawQueT queA, DrawQueT queB){
		return std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queA), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queA)) <
			   std::tie(std::get<static_cast<int>(DRAW_QUE::LAYER)>(queB), std::get<static_cast<int>(DRAW_QUE::ZORDER)>(queB));
		});

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	// 描画レイヤーや描画モードの初期値を設定する
	LAYER drawLayer = begin(LAYER());
	int blendMode = DX_BLENDMODE_NOBLEND;
	int blendModeNum = 255;

	//Queの描画先を設定
	SetDrawScreen(_layerGID);
	ClsDrawScreen();
	SetDrawBlendMode(blendMode, blendModeNum);

	// たまっているQueをそれぞれのレイヤーに描画する
	for (auto que : _drawList)
	{
		double x, y, rad;
		int id;
		LAYER layer;

		int blendModeOld = blendMode;
		int blendModeNumOld = blendModeNum;

		std::tie(id,x,y,rad,std::ignore,layer,blendMode,blendModeNum) = que;

		// 取り出したQueの内容が前までとLayerもしくは描画形式が違う場合、
		// 一旦バックバッファに描き出す
		if ((layer != drawLayer) || (blendModeNumOld != blendMode))
		{
			// _layerGIDに書いた内容をバックバッファに描画する
			SetDrawScreen(DX_SCREEN_BACK);
			SetDrawBlendMode(blendModeOld, blendModeNumOld);
			auto layPos = ScreenCenter + (*_activeScene)._screenPos;
			DrawRotaGraph(layPos.x, layPos.y, 1.0, 0.0, _layerGID, true);

			// 次のQueのための初期化を行い、描画先を一時描画先に設定する
			SetDrawScreen(_layerGID);
			SetDrawBlendMode(blendMode, blendModeNum);
			ClsDrawScreen();
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

	// for分を抜ける最後の_layerGIDの内容をバックバッファに描き出す
	// 最後に描画がくるやつはUIかEXで画面を揺らさないので描画位置は画面中央でOK
	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawBlendMode(blendMode, blendModeNum);
	DrawRotaGraph(ScreenCenter.x, ScreenCenter.y, 1.0, 0.0, _layerGID, true);	

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

bool SceneMng::AddActQue(ActQueT aQue)
{
	_actList.emplace_back(aQue);
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
	_layerGID = MakeScreen(ScreenSize.x, ScreenSize.y, true);

	SetDrawScreen(DX_SCREEN_BACK);
	_dbgSetup(200);

	lpImageMng.GetID("black", "image/black.png");
	lpImageMng.GetID("white", "image/white.png");
	lpImageMng.GetID("frame", "image/frame.png");

	std::srand(static_cast<unsigned int>(time(NULL)));

	return true;
}


