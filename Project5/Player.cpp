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
	// ���S�������͉������Ȃ�
	if (DestroyProc())
	{
		return;
	}

	// �L�[���̎擾
	(*_input).Update();

	// �ړ�����
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
	// ----------�A�j���[�V�����o�^�J�n
	// �ʏ�
	AnimVector data;
	data.emplace_back(IMAGE_ID("char")[0], 30);
	data.emplace_back(IMAGE_ID("char")[1], 60);
	SetAnim(STATE::NORMAL, data);

	// EX
	data.emplace_back(IMAGE_ID("char")[2], 0);
	SetAnim(STATE::EX,data);

	// ����
	int size = static_cast<int>(lpImageMng.GetID("plBlast").size());
	for (int i = 0; i < size; i++)
	{
		data.emplace_back(IMAGE_ID("plBlast")[i], i * 5 + 20);
	}
	data.emplace_back(-1, size * 5 + 20);
	SetAnim(STATE::DEATH, data);

	// -----------�A�j���[�V�����o�^�I��

	_speed = 4.0;

	// ���͏�Ԃ��Ǘ�����unique_ptr�쐬
	_input = std::make_shared<KeyState>();

	// �����A�j���[�V����
	state(STATE::NORMAL);
}
