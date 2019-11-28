#include <DxLib.h>
#include "Enemy.h"

Enemy::Enemy()
{
	_type = ENEMY_TYPE::A;
	_unitID = UNIT_ID::ENEMY;

	init();
}

Enemy::Enemy(EnemyState& state)
{
	_type = std::get<static_cast<int>(ENEMY_STATE::TYPE)>(state);
	_pos = std::get<static_cast<int>(ENEMY_STATE::VECTOR)>(state);
	_size = std::get<static_cast<int>(ENEMY_STATE::SIZE)>(state);
	_unitID = UNIT_ID::ENEMY;

	// 移動情報設定
	_moveCtl.SetMoveState(std::get<static_cast<int>(ENEMY_STATE::AIM)>(state),true);
	init();
}

void Enemy::Update(sharedObj plObj)
{
	// 死亡処理中は何もしない
	if (DestroyProc())
	{
		return;
	}
	_moveCtl.Update(plObj);
}

void Enemy::exFlag(bool isEX)
{
	// 移動タイプが拡散の時にだけflagを立ち上げる
	if (isEX)
	{
		if (_moveCtl.GetMoveType() != MOVE_TYPE::SPREAD)
		{
			isEX = false;
		}
	}

	_exFlag = isEX;
}

Enemy::~Enemy()
{
}

void Enemy::init()
{
	// -----------アニメーション登録
	// 待機状態
	AnimVector data;
	data.emplace_back(IMAGE_ID("char")[10 + (static_cast<int>(_type)) * 2], 30);
	data.emplace_back(IMAGE_ID("char")[11 + (static_cast<int>(_type)) * 2], 60);
	SetAnim(STATE::NORMAL, data);

	// 爆発
	int size = static_cast<int>(lpImageMng.GetID("enBlast").size());
	for (int i = 0; i < size; i++)
	{
		data.emplace_back(IMAGE_ID("enBlast")[i], 10 + i * 5);
	}
	data.emplace_back(-1, size * 5);
	SetAnim(STATE::DEATH, data);

	// -----------アニメーション登録終了

	_speed = 4.0;

	// 初期アニメーション
	state(STATE::NORMAL);
}
