#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::Bullet(Vector2Dbl pos,UNIT_ID id)
{
	_pos = pos;
	_unitID = id;
	// 今はプレイヤーだけなので上向き
	_rad = -3.1415926 / 2.0;
	_size = { 8,3 };
	_zOrder = 10;
	Init();
}


Bullet::~Bullet()
{
}

void Bullet::Update(sharedObj plObj)
{
	// 死亡処理中は何もしない
	if (DestroyProc())
	{
		return;
	}
	
	// 画面外判定
	if (_pos.y < -_size.x || _pos.y > lpSceneMng.GameScreenSize.y + _size.x)
	{
		setAlive(false);
	}
	_pos += _speed * Vector2Dbl{ cos(_rad), sin(_rad) };

	// 当たり判定
	lpSceneMng.AddActQue({ ACT_QUE::HIT_CHECK,*this });

}

void Bullet::Init(void)
{
	// ----------アニメーション登録開始
	// 通常
	AnimVector data;
	data.emplace_back(IMAGE_ID("shot")[0], 1);
	SetAnim(STATE::NORMAL, data);

	// 死亡
	data.emplace_back(-1, 0);
	SetAnim(STATE::DEATH, data);

	// -----------アニメーション登録終了

	_speed = 4.0;


	// 初期アニメーション
	state(STATE::NORMAL);
}
