#include <cmath>
#include "EnemyMove.h"
#include <_DebugConOut.h>
#include <_DebugDispOut.h>
#include <Scene\SceneMng.h>

EnemyMove::EnemyMove(Vector2Dbl& pos,double& rad,bool& exFlag) :_pos(pos),_rad(rad),_exFlag(exFlag)
{
	_enemyMoveType = nullptr;
	_aimCnt = -1;		// �ŏ���(SetMovePrg��)�C���N�������g�����Ƃ��ɂO�ɂ������̂ŏ����l��-1
}

EnemyMove::~EnemyMove()
{
}

void EnemyMove::Update(sharedObj plObj)
{
	plPos = (*plObj).pos();
	if (_enemyMoveType != nullptr)
	{
		(this->*_enemyMoveType)();
	}
}

// Enemy����̏������ƂɌ̂̈ړ���񃊃X�g��ݒ�
bool EnemyMove::SetMoveState(MoveState & state, bool newFlag)
{
	if (newFlag)
	{
		_aimState.clear();
	}

	// Enemy��MoveState���쐬�A�Q�Ƃœn���Amove�ŏ��L�����ړ�����Ɩ��ʂȃR�s�[���������Ȃ�
	_aimState = std::move(state);

	if (newFlag)
	{
		SetMovePrg();
	}

	return false;
}

MOVE_TYPE EnemyMove::GetMoveType(void)
{
	return _aimState[_aimCnt].first;
}

// �ړ��^�C�v�ύX
void EnemyMove::SetMovePrg(void)
{
	auto aimRev = [&]() {
		// �g��ړ���T���čăZ�b�g
		for (_aimCnt = 0; _aimCnt < _aimState.size(); _aimCnt++)
		{
			if (_aimState[_aimCnt].first == MOVE_TYPE::SPREAD)
			{
				return true;
			}
		}
		return false;
	};

	_aimCnt++;
	if (_aimCnt >= _aimState.size())
	{
		if (!aimRev())
		{
			return;
		}
	}

	if (_pos.y > lpSceneMng.GameScreenSize.y)
	{
		_pos.y = -100;
	}

	// �J�n�A�I���n�_�ݒ�
	_startPos = _pos;
	_endPos = _aimState[_aimCnt].second;

	// �ړ��^�C�v�ݒ�
	switch (_aimState[_aimCnt].first)
	{
	case MOVE_TYPE::WAIT:
		_enemyMoveType = &EnemyMove::Wait;
		break;

	case MOVE_TYPE::SIGMOID:
		_enemyMoveType = &EnemyMove::MoveSigmoid;
		_length = { (_endPos.x - _startPos.x) ,(_endPos.y - _startPos.y) };
		_movePerFrame.x = abs(_length.x) / static_cast<double>(SIGMOID_CNT_MAX);
		break;

	case MOVE_TYPE::SPIRAL:
		_enemyMoveType = &EnemyMove::MoveSpiral;
		_anglePerFrame = 2.5 * 3.1415926 / static_cast<double>(SPIRAL_CNT_MAX);
		_moveRadius = static_cast<double>(SPIRAL_RADIUS);
		break;

	case MOVE_TYPE::PIT_IN:
		// �i�[�ꏊ�͍��E�ɓ����̂ŕ΍��ő_��
		// ���B����t���[����,�ړ��J�n�t���[��(�������Ă΂��̂��O�̈ړ��̏I�����Ȃ̂�,[���݂̃t���[���� + 1]) + �ړ��t���[����
		_endPos.x += static_cast<double>(((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % 200)
				  - (((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % 200) * 2 / 200) 
			      * 2 * ((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % (200 / 2)));
		// ������200��, {500(GameScreenSize.x) - 400(�G���i�[�����G���A�̉���40 * 10)} * 2(������)
		// moveLR�ɂ��g���Ă�̂ł��̂���������

		_enemyMoveType = &EnemyMove::PitIn;
		_length = { (_endPos.x - _startPos.x) ,(_endPos.y - _startPos.y) };

		// �摜�̌����̊֌W��90�x�����Ă���
		_rad = std::atan2(_length.y,_length.x) + (3.1415926 / 2.0);
		_movePerFrame = _length / static_cast<double>(PIT_IN_CNT_MAX);
		break;

	case MOVE_TYPE::LR:
		_enemyMoveType = &EnemyMove::MoveLR;
		break;

	case MOVE_TYPE::SPREAD:
		_movePerFrame = (_endPos - _startPos) / static_cast<double>(SPREAD_CNT_MAX / 2);
		_enemyMoveType = &EnemyMove::Spread;
		break;

	case MOVE_TYPE::ATTACK:
		_exFlag = false;
		_enemyMoveType = &EnemyMove::MoveAttack;
		break;
	default:
		AST();
		_enemyMoveType = &EnemyMove::Wait;
		break;
	}

	_moveCnt = 0;

}

// �V�O���C�h�Ȑ���Ɉړ�
void EnemyMove::MoveSigmoid(void)
{
	_moveCnt++;
	_posOld = _pos;
	Vector2Dbl move;		// _startPos����̈ړ���

	move.x = static_cast<double>(_moveCnt) * _movePerFrame.x;
	move.y = abs(_length.y) / 
		    (1.0 + exp(-10.0 * (static_cast<double>(_moveCnt * _movePerFrame.x) - abs(_length.x) / 2.0) / abs(_length.x)));

	if (_length.x > 0)
	{
		// ��������o��Ƃ�
		_pos.x = _startPos.x + move.x;
	}
	else
	{
		// �E������
		_pos.x = _startPos.x - move.x;
	}

	if (_length.y > 0)
	{
		// �������Ɉړ�
		_pos.y = _startPos.y + move.y;
	}
	else
	{
		// ������Ɉړ�
		_pos.y = _startPos.y - move.y;
	}

	_rad = std::atan2(_pos.y - _posOld.y, _pos.x - _posOld.x) + (3.1415926 / 2.0);

	if (_moveCnt >= SIGMOID_CNT_MAX)
	{
		_pos = _endPos;
		SetMovePrg();
	}
}

// ������Ɉړ�
void EnemyMove::MoveSpiral(void)
{
	// _endPos�ɂ͉~�ړ��̒��S���W��ݒ�
	_moveCnt++;

	double moveAngle = static_cast<double>(_moveCnt) * _anglePerFrame; // �ړ��p�x

	if (_endPos.y - _startPos.y < 0)
	{
		// �������ɉ�]���n�߂�
		if (_startPos.x < static_cast<double>(lpSceneMng.GameScreenSize.x / 2))
		{
			_rad = -3.1415926 / 2.0 + moveAngle;
			_pos.x = _endPos.x + _moveRadius * std::cos(3.1415926 / 2.0 + moveAngle);
			_pos.y = _endPos.y + _moveRadius * std::sin(3.1415926 / 2.0 + moveAngle);
		}
		else
		{
			_rad = 3.1415926 / 2.0 - moveAngle;
			_pos.x = _endPos.x + _moveRadius * std::cos(3.1415926 / 2.0 - moveAngle);
			_pos.y = _endPos.y + _moveRadius * std::sin(3.1415926 / 2.0 - moveAngle);
		}
	}
	else
	{
		// �ォ�牺�ɉ�]���n�߂�
		if (_startPos.x < static_cast<double>(lpSceneMng.GameScreenSize.x / 2))
		{
			// �E���
			_rad = -moveAngle - 3.1415926 / 2.0;
			_pos.x = _endPos.x + _moveRadius * std::cos(-moveAngle - 3.1415926 / 2.0);
			_pos.y = _endPos.y + _moveRadius * std::sin(-moveAngle - 3.1415926 / 2.0);
		}
		else
		{
			// �����
			_rad = moveAngle + 3.1415926 / 2.0;
			_pos.x = _endPos.x + _moveRadius * std::cos(moveAngle - 3.1415926 / 2.0);
			_pos.y = _endPos.y + _moveRadius * std::sin(moveAngle - 3.1415926 / 2.0);
		}
	}

	_moveRadius -= (static_cast<double>(SPIRAL_RADIUS) / static_cast<double>(SPIRAL_CNT_MAX * 2));
	//_rad = _moveAngle;

	if (_moveCnt >= SPIRAL_CNT_MAX)
	{
		SetMovePrg();
	}

}

void EnemyMove::PitIn(void)
{
	_moveCnt++;

	_pos += _movePerFrame;

	if (_moveCnt >= PIT_IN_CNT_MAX)
	{
		_rad = 0.0;
		_pos = _endPos;
		SetMovePrg();
		return;
	}
}

// �ړ��Ȃ�
void EnemyMove::Wait(void)
{
	// Wait�̏ꍇ,_endPos��x�̕��ɖڕW�̃t���[�����������Ă���
	// �ŏ���Wait���Ă�ł���̃t���[�������ڕW
	if (_moveCnt >= static_cast<int>(_endPos.x))
	{
		SetMovePrg();
		return;
	}

	_moveCnt++;
}

// ���E�ړ�
void EnemyMove::MoveLR(void)
{
	_moveCnt++;

	_pos.x += static_cast<double>(1 - ((lpSceneMng.frameCnt() % 200) * 2 / 200) * 2);

	if (_moveCnt >= static_cast<int>(_endPos.x) && (lpSceneMng.frameCnt() % 100) == 50)
	{
		SetMovePrg();
		return;
	}
}

// ���k
void EnemyMove::Spread(void)
{
	_moveCnt++;

	_pos += _movePerFrame * static_cast<double>(1 - ((_moveCnt % SPREAD_CNT_MAX) * 2 / SPREAD_CNT_MAX) * 2);

	if (_exFlag)
	{
		SetMovePrg();
		return;
	}
}

void EnemyMove::MoveAttack(void)
{
	if (_rad < 3.1415926)
	{
		_rad += 3.1415926 / 18.0;
	}
	else
	{
		_enemyMoveType = &EnemyMove::PitIn;

		_endPos = plPos + Vector2Dbl(0.0, 100.0);
		_startPos = _pos;

		_length = { (_endPos.x - _startPos.x) ,(_endPos.y - _startPos.y) };

		// �摜�̌����̊֌W��90�x�����Ă���
		_rad = std::atan2(_length.y, _length.x) + (3.1415926 / 2.0);
		_movePerFrame = _length / static_cast<double>(PIT_IN_CNT_MAX);
	}
}
