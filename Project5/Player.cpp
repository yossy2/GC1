#include <DxLib.h>
#include "Player.h"
#include <_DebugConOut.h>
#include <KeyState.h>

Player::Player()
{
	_unitID = UNIT_ID::PLAYER;

	Init();
}

Player::Player(Vector2Dbl pos, Vector2Int size)
{
	_unitID = UNIT_ID::PLAYER;
	_pos = pos;
	_size = size;
	Init();

}

void Player::Update(sharedObj plObj)
{
	// 死亡処理中は何もしない
	if (DestroyProc())
	{
		return;
	}

	// キー情報の取得
	(*_input).Update();

	// 移動処理
	auto playerMove = [](std::weak_ptr<InputState> keyData,const INPUT_ID id, double& pNum, const double speed)
	{
		if (!keyData.expired())
		{
			if ((*keyData.lock()).state(id).first)
			{
				pNum += speed;
			}
		}
	};

	playerMove(_input, INPUT_ID::LEFT,  _pos.x, -_speed);
	playerMove(_input, INPUT_ID::RIGHT, _pos.x,  _speed);
	playerMove(_input, INPUT_ID::UP,    _pos.y, -_speed);
	playerMove(_input, INPUT_ID::DOWN,  _pos.y,  _speed);

	if ((*_input).state(INPUT_ID::BTN_1).first && !(*_input).state(INPUT_ID::BTN_1).second)
	{
		lpSceneMng.AddActQue({ ACT_QUE::SHOT,*this });
	}

}


Player::~Player()
{
}

void Player::Init()
{
	// ----------アニメーション登録開始
	// 通常
	AnimVector data;
	data.emplace_back(IMAGE_ID("char")[0], 30);
	data.emplace_back(IMAGE_ID("char")[1], 60);
	SetAnim(STATE::NORMAL, data);

	// EX
	data.emplace_back(IMAGE_ID("char")[2], 0);
	SetAnim(STATE::EX,data);

	// 爆発
	int size = static_cast<int>(lpImageMng.GetID("plBlast").size());
	for (int i = 0; i < size; i++)
	{
		data.emplace_back(IMAGE_ID("plBlast")[i], i * 5 + 20);
	}
	data.emplace_back(-1, size * 5 + 20);
	SetAnim(STATE::DEATH, data);

	// -----------アニメーション登録終了

	_speed = 4.0;

	// 入力状態を管理するunique_ptr作成
	_input = std::make_shared<KeyState>();

	// 初期アニメーション
	state(STATE::NORMAL);
}
