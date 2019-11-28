#include <cmath>
#include "EnemyMove.h"
#include <_DebugConOut.h>
#include <_DebugDispOut.h>
#include <Scene\SceneMng.h>

EnemyMove::EnemyMove(Vector2Dbl& pos,double& rad,bool& exFlag) :_pos(pos),_rad(rad),_exFlag(exFlag)
{
	_enemyMoveType = nullptr;
	_aimCnt = -1;		// 最初に(SetMovePrgで)インクリメントしたときに０にしたいので初期値は-1
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

// Enemyからの情報をもとに個体の移動情報リストを設定
bool EnemyMove::SetMoveState(MoveState & state, bool newFlag)
{
	if (newFlag)
	{
		_aimState.clear();
	}

	// EnemyでMoveStateを作成、参照で渡し、moveで所有権を移動すると無駄なコピーが発生しない
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

// 移動タイプ変更
void EnemyMove::SetMovePrg(void)
{
	auto aimRev = [&]() {
		// 拡大移動を探して再セット
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

	// 開始、終了地点設定
	_startPos = _pos;
	_endPos = _aimState[_aimCnt].second;

	// 移動タイプ設定
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
		// 格納場所は左右に動くので偏差で狙う
		// 到達するフレームは,移動開始フレーム(ここが呼ばれるのが前の移動の終了時なので,[現在のフレーム数 + 1]) + 移動フレーム数
		_endPos.x += static_cast<double>(((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % 200)
				  - (((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % 200) * 2 / 200) 
			      * 2 * ((lpSceneMng.frameCnt() + 1 + PIT_IN_CNT_MAX) % (200 / 2)));
		// ここの200は, {500(GameScreenSize.x) - 400(敵が格納されるエリアの横幅40 * 10)} * 2(往復分)
		// moveLRにも使ってるのでそのうち直そう

		_enemyMoveType = &EnemyMove::PitIn;
		_length = { (_endPos.x - _startPos.x) ,(_endPos.y - _startPos.y) };

		// 画像の向きの関係上90度足しておく
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

// シグモイド曲線状に移動
void EnemyMove::MoveSigmoid(void)
{
	_moveCnt++;
	_posOld = _pos;
	Vector2Dbl move;		// _startPosからの移動量

	move.x = static_cast<double>(_moveCnt) * _movePerFrame.x;
	move.y = abs(_length.y) / 
		    (1.0 + exp(-10.0 * (static_cast<double>(_moveCnt * _movePerFrame.x) - abs(_length.x) / 2.0) / abs(_length.x)));

	if (_length.x > 0)
	{
		// 左側から出るとき
		_pos.x = _startPos.x + move.x;
	}
	else
	{
		// 右側から
		_pos.x = _startPos.x - move.x;
	}

	if (_length.y > 0)
	{
		// 下向きに移動
		_pos.y = _startPos.y + move.y;
	}
	else
	{
		// 上向きに移動
		_pos.y = _startPos.y - move.y;
	}

	_rad = std::atan2(_pos.y - _posOld.y, _pos.x - _posOld.x) + (3.1415926 / 2.0);

	if (_moveCnt >= SIGMOID_CNT_MAX)
	{
		_pos = _endPos;
		SetMovePrg();
	}
}

// 螺旋状に移動
void EnemyMove::MoveSpiral(void)
{
	// _endPosには円移動の中心座標を設定
	_moveCnt++;

	double moveAngle = static_cast<double>(_moveCnt) * _anglePerFrame; // 移動角度

	if (_endPos.y - _startPos.y < 0)
	{
		// 下から上に回転し始める
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
		// 上から下に回転し始める
		if (_startPos.x < static_cast<double>(lpSceneMng.GameScreenSize.x / 2))
		{
			// 右回り
			_rad = -moveAngle - 3.1415926 / 2.0;
			_pos.x = _endPos.x + _moveRadius * std::cos(-moveAngle - 3.1415926 / 2.0);
			_pos.y = _endPos.y + _moveRadius * std::sin(-moveAngle - 3.1415926 / 2.0);
		}
		else
		{
			// 左回り
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

// 移動なし
void EnemyMove::Wait(void)
{
	// Waitの場合,_endPosのxの方に目標のフレーム数が入っている
	// 最初にWaitを呼んでからのフレーム数が目標
	if (_moveCnt >= static_cast<int>(_endPos.x))
	{
		SetMovePrg();
		return;
	}

	_moveCnt++;
}

// 左右移動
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

// 収縮
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

		// 画像の向きの関係上90度足しておく
		_rad = std::atan2(_length.y, _length.x) + (3.1415926 / 2.0);
		_movePerFrame = _length / static_cast<double>(PIT_IN_CNT_MAX);
	}
}
