#include <time.h>
#include <algorithm>
#include <DxLib.h>
#include <_DebugConOut.h>
#include <ImageMng.h>
#include "SceneMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <Player.h>
#include <Enemy.h>
#include <EnemyMove.h>

GameScene::GameScene()
{
	srand(static_cast<unsigned>(time(NULL)));
	// 画像読み込み
	lpImageMng.GetID("char", "image/char.png", { 30,32 }, { 10,10 });
	lpImageMng.GetID("shot", "image/shot.png", { 8,3 }, { 1,2 });
	lpImageMng.GetID("enBlast", "image/en_blast.png", { 64,64 }, { 5,1 });
	lpImageMng.GetID("plBlast", "image/pl_blast.png", { 64,64 }, { 4,1 });
	auto gameScreenSize = lpSceneMng.GameScreenSize;

	_objList.emplace_back(new Player({ static_cast<double>(lpSceneMng.ScreenCenter.x),
									   static_cast<double>(gameScreenSize.y - 32)},
									   {0,0})
	);


	// 敵オブジェクト作成
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			// 移動タイプ設定
			MoveState tmpMoveState;
			// WAITはendPosのxに待機フレーム数を設定
			tmpMoveState.emplace_back(MOVE_TYPE::WAIT, Vector2Dbl{ static_cast<double>(x + y * 10) * 30.0,0.0 });

			// 出てくる順番によって左右、上下位置を変更
			tmpMoveState.emplace_back(MOVE_TYPE::SIGMOID,
									  Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) + ((1 + x + y * 10) % 2) * (gameScreenSize.x * 3 / 7)) ,
												  static_cast<double>((gameScreenSize.y * 5 / 6) - (((x + y * 10) % 6) / 4) * SPIRAL_RADIUS * 2)
									  });

			// SPIRALはendPosに回転の中心座標を設定
			tmpMoveState.emplace_back(MOVE_TYPE::SPIRAL,
				Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) + ((1 + x + y * 10) % 2) * (gameScreenSize.x * 3 / 7)) ,
				static_cast<double>((gameScreenSize.y * 5 / 6) - SPIRAL_RADIUS)
				});

			tmpMoveState.emplace_back(MOVE_TYPE::PIT_IN, 
									  Vector2Dbl{ (30.0 + 10.0) * static_cast<double>(x) + 15.0,
									              (32.0 + 3.0) * static_cast<double>(y) + 16.0 + 40.0
									  });

			tmpMoveState.emplace_back(MOVE_TYPE::LR, 
									  Vector2Dbl{ (30.0 + 10.0) * static_cast<double>(x) + 15.0,
												  (32.0 + 3.0) * static_cast<double>(y) + 16.0 + 40.0 
									  });

			// 830:画面の幅+敵オブジェクトの幅
			// 268:{(画面下端 - プレイヤーの高さ - 敵の高さの半分) - (画面上端 + 敵の高さの半分)} / 2
			// 敵のサイズとかマジックナンバー使ってるのでとりあえず画面サイズも
			EnemyState eState = {
				ENEMY_TYPE::A,
				{ -15.0 + (static_cast<double>((x + y * 10) % 2) * (gameScreenSize.x + 30)),
				   16.0 + (static_cast<double>((((x + y * 10) % 6) / 2) * (gameScreenSize.y - 32 * 2) / 2))},
				{ 30, 32 } ,
				tmpMoveState
			};

			_objList.emplace_back(new Enemy(eState));
		}
	}

	TRACE("GameScene\n");
}

GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{
	for (auto data : _objList)
	{
		(*data).Update();
	}

	for (auto data : _objList)
	{
		(*data).Draw();
	}

	// 死んでいる（爆発が終了している）オブジェクトの削除
	_objList.erase(
		std::remove_if(
			_objList.begin(),
			_objList.end(),
			[](sharedObj& obj) {return (*obj).isDead(); }
		),
		_objList.end()
	);

	return std::move(own);
}
