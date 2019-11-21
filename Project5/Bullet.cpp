#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::Bullet(Vector2Dbl pos)
{
	_pos = pos;
	// ���̓v���C���[�����Ȃ̂ŏ����
	_rad = -3.1415926 / 2.0;
	_size = { 8,3 };
	_zOrder = 10;
	Init();
}


Bullet::~Bullet()
{
}

void Bullet::Update(void)
{
	// ���S�������͉������Ȃ�
	if (DestroyProc())
	{
		return;
	}
	
	// ��ʊO����
	if (_pos.y < -_size.x || _pos.y > lpSceneMng.GameScreenSize.y + _size.x)
	{
		setAlive(false);
	}
	_pos += _speed * Vector2Dbl{ cos(_rad), sin(_rad) };

}

void Bullet::Init(void)
{
	// ----------�A�j���[�V�����o�^�J�n
	// �ʏ�
	AnimVector data;
	data.emplace_back(IMAGE_ID("shot")[0], 1);
	SetAnim(STATE::NORMAL, data);

	// ���S
	data.emplace_back(-1, 0);
	SetAnim(STATE::DEATH, data);

	// -----------�A�j���[�V�����o�^�I��

	_speed = 4.0;


	// �����A�j���[�V����
	state(STATE::NORMAL);
}