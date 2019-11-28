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

	// �ړ����ݒ�
	_moveCtl.SetMoveState(std::get<static_cast<int>(ENEMY_STATE::AIM)>(state),true);
	init();
}

void Enemy::Update(sharedObj plObj)
{
	// ���S�������͉������Ȃ�
	if (DestroyProc())
	{
		return;
	}
	_moveCtl.Update(plObj);
}

void Enemy::exFlag(bool isEX)
{
	// �ړ��^�C�v���g�U�̎��ɂ���flag�𗧂��グ��
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
	// -----------�A�j���[�V�����o�^
	// �ҋ@���
	AnimVector data;
	data.emplace_back(IMAGE_ID("char")[10 + (static_cast<int>(_type)) * 2], 30);
	data.emplace_back(IMAGE_ID("char")[11 + (static_cast<int>(_type)) * 2], 60);
	SetAnim(STATE::NORMAL, data);

	// ����
	int size = static_cast<int>(lpImageMng.GetID("enBlast").size());
	for (int i = 0; i < size; i++)
	{
		data.emplace_back(IMAGE_ID("enBlast")[i], 10 + i * 5);
	}
	data.emplace_back(-1, size * 5);
	SetAnim(STATE::DEATH, data);

	// -----------�A�j���[�V�����o�^�I��

	_speed = 4.0;

	// �����A�j���[�V����
	state(STATE::NORMAL);
}
