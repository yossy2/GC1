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

GameScene::GameScene()
{
	srand(static_cast<unsigned>(time(NULL)));
	// �摜�ǂݍ���
	lpImageMng.GetID("char", "image/char.png", { 30,32 }, { 10,10 });
	lpImageMng.GetID("shot", "image/shot.png", { 8,3 }, { 1,2 });
	lpImageMng.GetID("enBlast", "image/en_blast.png", { 64,64 }, { 5,1 });
	lpImageMng.GetID("plBlast", "image/pl_blast.png", { 64,64 }, { 4,1 });
	auto gameScreenSize = lpSceneMng.GameScreenSize;

	_objList.emplace_back(new Player({ static_cast<double>(lpSceneMng.ScreenCenter.x),
									   static_cast<double>(gameScreenSize.y - 32)},
									   {0,0})
	);


	// �G�I�u�W�F�N�g�쐬
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			// �ړ��^�C�v�ݒ�
			MoveState tmpMoveState;
			// WAIT��endPos��x�ɑҋ@�t���[������ݒ�
			tmpMoveState.emplace_back(MOVE_TYPE::WAIT, Vector2Dbl{ static_cast<double>(x + y * 10) * 30.0,0.0 });

			// �o�Ă��鏇�Ԃɂ���č��E�A�㉺�ʒu��ύX
			tmpMoveState.emplace_back(MOVE_TYPE::SIGMOID,
									  Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) + ((1 + x + y * 10) % 2) * (gameScreenSize.x * 3 / 7)) ,
												  static_cast<double>((gameScreenSize.y * 5 / 6) - (((x + y * 10) % 6) / 4) * SPIRAL_RADIUS * 2)
									  });

			// SPIRAL��endPos�ɉ�]�̒��S���W��ݒ�
			tmpMoveState.emplace_back(MOVE_TYPE::SPIRAL,
				Vector2Dbl{ static_cast<double>((gameScreenSize.x * 2 / 7) + ((1 + x + y * 10) % 2) * (gameScreenSize.x * 3 / 7)) ,
				static_cast<double>((gameScreenSize.y * 5 / 6) - SPIRAL_RADIUS)
				});

			tmpMoveState.emplace_back(MOVE_TYPE::PIT_IN, 
									  Vector2Dbl{ (30.0 + 10.0) * static_cast<double>(x) + 15.0,
									              (32.0 + 3.0) * static_cast<double>(y) + 16.0 + 40.0
									  });

			// LR��endPos��x�ɏI���܂ł̃t���[����������
			tmpMoveState.emplace_back(MOVE_TYPE::LR, 
									  Vector2Dbl{ static_cast<double>((5 * 10 - (x + y * 10) - 1) * 30.0),
												  0.0 
									  });

			tmpMoveState.emplace_back(MOVE_TYPE::SPREAD,
				Vector2Dbl{ static_cast<double>((lpSceneMng.GameScreenSize.x / 10) * x + 15.0),
							(42.0) * static_cast<double>(y) + 16.0
				});

			// 830:��ʂ̕�+�G�I�u�W�F�N�g�̕�
			// 268:{(��ʉ��[ - �v���C���[�̍��� - �G�̍����̔���) - (��ʏ�[ + �G�̍����̔���)} / 2
			// �G�̃T�C�Y�Ƃ��}�W�b�N�i���o�[�g���Ă�̂łƂ肠������ʃT�C�Y��
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
	_dbgDrawCircle(((lpSceneMng.frameCnt() % 160) - ((lpSceneMng.frameCnt() % 160) / 80) * 2 * (lpSceneMng.frameCnt() % 80)), 15, 15, 0xff0000, true);

	// ����ł���i�������I�����Ă���j�I�u�W�F�N�g�̍폜
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
