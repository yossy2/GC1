#include <DxLib.h>
#include <Scene\SceneMng.h>
#include "Obj.h"
#include <_DebugConOut.h>
#include <_DebugDispOut.h>

Obj::Obj()
{
	_alive = true;
	_dead = false;
	_animFrame = 0;
	_animCount = 0;
	_speed = 0.0;
	_rad = 0.0;
	_zOrder = 0;
}

void Obj::Draw(void)
{
	// ---------- �͈̓`�F�b�N
	if (_animMap.find(_state) == _animMap.end())
	{
		AST();
		return;
	}
	
	if (_animFrame >= _animMap[_state].size() || _animFrame < 0)
	{
		AST();
		return;
	}

	// �A�j���[�V�����̃R�}���̍X�V�m�F
	if (_animCount >= _animMap[_state][_animFrame].second)
	{
		// �����V���b�g�̊m�F
		if (_animMap[_state][_animFrame].first != -1)
		{
			_animFrame++;
		}
	}

	// �A�j���[�V�������[�v�̊m�F
	if (_animFrame < _animMap[_state].size())
	{
		_animCount++;
	}
	else
	{
		_animFrame = 0;
		_animCount = 0;
	}

	// ���݂̃R�}�̉摜��`��L���[�ɒǉ�
	lpSceneMng.AddDrawQue({ _animMap[_state][_animFrame].first,
							_pos.x + static_cast<double>(lpSceneMng.GameScreenOffSet.x),
						    _pos.y + static_cast<double>(lpSceneMng.GameScreenOffSet.y),
							_rad ,
							_zOrder,
							LAYER::CHAR});
}

void Obj::Draw(int id)
{
	_dbgDrawBox(_pos.x - 15,_pos.y - 15, _pos.x + 15, _pos.y + 15,0xff0000,true);
	lpSceneMng.AddDrawQue({ id,
							_pos.x + static_cast<double>(SceneMng::GetInstance().GameScreenOffSet.x),
					        _pos.y + static_cast<double>(SceneMng::GetInstance().GameScreenOffSet.y),
					        _rad,
							_zOrder,
							LAYER::CHAR
	});
}


Obj::~Obj()
{
}

// STATE��get,set
// get
STATE Obj::state(void) const
{
	return _state;
}

// set
bool Obj::state(const STATE key)
{
	if (_animMap.find(key) == _animMap.end())
	{
		// �o�^�����A�j���[�V�����̃L�[�̒��ɑ��݂��Ȃ�
		AST();
		return false;
	}

	// ���݂Ɠ����A�j���[�V�����̎��̓R�}���ƃJ�E���g���N���A���Ȃ�
	if (_state != key)
	{
		_animFrame = 0;
		_animCount = 0;
	}

	_state = key;
	return true;
}

bool Obj::setAlive(bool alive)
{
	_alive = alive;
	if (!_alive)
	{
		state(STATE::DEATH);
	}
	return true;
}

// _dead�̓����o���炵���ύX���Ȃ����炢��Ȃ�����
// _alive��set���g���Ă邩�炻��ɍ��킹�Ĉꉞ
bool Obj::setDead(bool dead)
{
	if (isAlive())
	{
		// ���󔚔����I�������������g��Ȃ��̂Ő������Ă���Ƃ��͒l��ݒ肵�Ȃ�
		return false;
	}

	_dead = dead;

	return true;
}

bool Obj::isAnimEnd(void)
{
	// ---------- �͈̓`�F�b�N
	if (_animMap.find(_state) == _animMap.end())
	{
		AST();
		return true;
	}

	if (_animFrame >= _animMap[_state].size() || _animFrame < 0)
	{
		AST();
		return true;
	}

	if (_animMap[_state][_animFrame].first == -1)
	{
		return true;
	}

	return false;
}

// �A�j���[�V�����̃f�[�^��o�^
bool Obj::SetAnim(const STATE key, AnimVector& data)
{
	return _animMap.try_emplace(key, std::move(data)).second;

	/*if (_animMap.find(key) == _animMap.end())
	{
		_animMap[key] = std::move(data);
		return true;
	}
	
	return false;*/
}

// ������Ԋm�F,���S��Ԑݒ�
bool Obj::DestroyProc(void)
{
	if (isAlive())
	{
		return false;
	}

	if (isAnimEnd())
	{
		setDead(true);
	}

	return true;
}
