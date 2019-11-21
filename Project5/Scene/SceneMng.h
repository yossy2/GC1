#pragma once
#include <memory>
#include <map>
#include <vector>
#include <tuple>
#include <common/Vector2.h>
#include "BaseScene.h"
#include "LAYER.h"

#define lpSceneMng SceneMng::GetInstance()

// 描画Queメンバ
enum class DRAW_QUE
{
	IMAGE,		// 画像ID
	X,			// 描画座標X
	Y,			// 描画座標Y
	RAD,		// 回転角
	ZORDER,		// 描画優先度(奥行)
	LAYER		// 描画レイヤー
};

using DrawQueT = std::tuple<int, double, double, double, int, LAYER>;

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *sInstance;
	}

	static void Create(void)
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}

	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}

	void Run(void);

	bool AddDrawQue(DrawQueT dQue);			// 描画queリクエスト
	bool AddActQue(ActQueT aQue);			// アクションqueリクエスト
	int frameCnt(void);						// get frameCnt関数

	const Vector2Int ScreenSize;			// 画面サイズ
	const Vector2Int ScreenCenter;			// 画面中央
	const Vector2Int GameScreenSize;		// ゲーム画面サイズ
	const Vector2Int GameScreenOffSet;		// ゲーム画面の左上座標

private:
	static SceneMng* sInstance;
	unique_Base _activeScene;

	void Draw(void);
	std::map<LAYER, int> _screenID;			// 描画スクリーンID格納場所
	std::vector<DrawQueT> _drawList;		// 描画que格納場所
	std::vector<ActQueT> _actList;			// アクションQue格納場所

	int _frameCnt;							// フレームが変更されてからのカウント

	SceneMng();
	~SceneMng();
	bool SysInit(void);
};

