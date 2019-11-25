#include <time.h>
#include <algorithm>
#include <DxLib.h>
#include <_DebugConOut.h>
#include <_DebugDispOut.h>
#include <ImageMng.h>
#include "SceneMng.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <Player.h>
#include <Enemy.h>
#include <EnemyMove.h>
#include <Bullet.h>
#include "func\FuncBullet.h"
#include "func\FuncCheckHit.h"

GameScene::GameScene()
{
	InitFunc();
	// 画像読み込み
	lpImageMng.GetID("char", "image/char.png", { 30,32 }, { 10,10 });
	lpImageMng.GetID("shot", "image/shot.png", { 8,3 }, { 1,2 });
	lpImageMng.GetID("enBlast", "image/en_blast.png", { 64,64 }, { 5,1 });
	lpImageMng.GetID("plBlast", "image/pl_blast.png", { 64,64 }, { 4,1 });
	auto gameScreenSize = lpSceneMng.GameScreenSize;

	_objList.emplace_back(new Player({ static_cast<double>(lpSceneMng.GameScreenSize.x / 2),
									   static_cast<double>(gameScreenSize.y - 32)},
									   {30,32})
	);

	// 敵オブジェクト作成
	Vector2Dbl cSize = {30.0,32.0};		// キャラクターのサイズ

	for (int y = 0; y < ENYMY_CNT_Y; y++)
	{
		for (int x = 0; x < ENYMY_CNT_X; x++)
		{
			// 移動タイプ設定
			MoveState tmpMoveState;
			// WAITはendPosのxに待機フレーム数を設定
			tmpMoveState.emplace_back(MOVE_TYPE::WAIT, Vector2Dbl{ static_cast<double>(x + y * 10) * cSize.x,0.0 });

			// 出てくる順番によって左右、上下位置を変更
			tmpMoveState.emplace_back(MOVE_TYPE::SIGMOID,
									  Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) 
																   + ((1 + x + y * ENYMY_CNT_X) % 2) 
																   * (gameScreenSize.x * 3 / 7)) ,
												  static_cast<double>((gameScreenSize.y * 5 / 6)
																   - (((x + y * ENYMY_CNT_X) % 6) / 4) * SPIRAL_RADIUS * 2)
									  });

			// SPIRALはendPosに回転の中心座標を設定
			tmpMoveState.emplace_back(MOVE_TYPE::SPIRAL,
									  Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) 
																   + ((1 + x + y * ENYMY_CNT_X) % 2) 
																   * (gameScreenSize.x * 3 / 7)) ,
												  static_cast<double>((gameScreenSize.y * 5 / 6) - SPIRAL_RADIUS)
												});

			tmpMoveState.emplace_back(MOVE_TYPE::PIT_IN, 
									  Vector2Dbl{ (cSize.x + 10.0) * static_cast<double>(x) + 15.0,
									              (cSize.y + 3.0) * static_cast<double>(y) + (cSize.y / 2.0) + 40.0
									  });

			// LRはendPosのxに終了までのフレーム数を入れる
			tmpMoveState.emplace_back(MOVE_TYPE::LR, 
									  Vector2Dbl{ static_cast<double>((5 * 10 - (x + y * 10) - 1) * cSize.x),
												  0.0 
									  });


			tmpMoveState.emplace_back(MOVE_TYPE::SPREAD,
									  Vector2Dbl{ static_cast<double>(((lpSceneMng.GameScreenSize.x 
																    - static_cast<int>(cSize.x)) * x / 9)
																    + cSize.x / 2.0),
																	((cSize.y + 3.0)
																		* static_cast<double>(y * lpSceneMng.GameScreenSize.x 
																			/ ((cSize.x + 10.0)* static_cast<double>(ENYMY_CNT_X))))
																	+ 16.0 + 40.0
									  });

			// 830:画面の幅+敵オブジェクトの幅
			// 268:{(画面下端 - プレイヤーの高さ - 敵の高さの半分) - (画面上端 + 敵の高さの半分)} / 2
			// 敵のサイズとかマジックナンバー使ってるのでとりあえず画面サイズも
			EnemyState eState = {
				ENEMY_TYPE::A,
				{ -15.0 + (static_cast<double>(((x + y * 10) % 2) * (gameScreenSize.x + static_cast<int>(cSize.x)))),
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
	_dbgDrawCircle(((lpSceneMng.frameCnt() % 160) - ((lpSceneMng.frameCnt() % 160) / 80) * 2 * (lpSceneMng.frameCnt() % 80)), 15, 15, 0xff0000, true);

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

void GameScene::InitFunc(void)
{
	_funcQue = { { ACT_QUE::SHOT,FuncBullet() },{ ACT_QUE::HIT_CHECK,FuncCheckHit() } };
}

void GameScene::RunActQue(std::vector<ActQueT> actList)
{
	for (auto que : actList)
	{
		try
		{
			_funcQue.at(que.first)(que,_objList);
		}
		catch (...)
		{
			AST();
		}
	}
}
