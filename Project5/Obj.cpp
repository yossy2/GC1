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
	// ---------- 範囲チェック
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

	// アニメーションのコマ数の更新確認
	if (_animCount >= _animMap[_state][_animFrame].second)
	{
		// ワンショットの確認
		if (_animMap[_state][_animFrame].first != -1)
		{
			_animFrame++;
		}
	}

	// アニメーションループの確認
	if (_animFrame < _animMap[_state].size())
	{
		_animCount++;
	}
	else
	{
		_animFrame = 0;
		_animCount = 0;
	}

	// 現在のコマの画像を描画キューに追加
	lpSceneMng.AddDrawQue({ _animMap[_state][_animFrame].first,
							_pos.x + static_cast<double>(lpSceneMng.GameScreenOffSet.x),
						    _pos.y + static_cast<double>(lpSceneMng.GameScreenOffSet.y),
							_rad ,
							_zOrder,
							LAYER::CHAR,
						    DX_BLENDMODE_NOBLEND,
							255
	});
}

void Obj::Draw(int id)
{
	lpSceneMng.AddDrawQue({ id,
							_pos.x + static_cast<double>(SceneMng::GetInstance().GameScreenOffSet.x),
					        _pos.y + static_cast<double>(SceneMng::GetInstance().GameScreenOffSet.y),
					        _rad,
							_zOrder,
							LAYER::CHAR,
							DX_BLENDMODE_NOBLEND,
							255
	});
}


Obj::~Obj()
{
}

// STATEのget,set
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
		// 登録したアニメーションのキーの中に存在しない
		AST();
		return false;
	}

	// 現在と同じアニメーションの時はコマ数とカウントをクリアしない
	if (_state != key)
	{
		_animFrame = 0;
		_animCount = 0;
	}

	_state = key;
	return true;
}

bool Obj::alive(bool alive)
{
	_alive = alive;
	if (!_alive)
	{
		_zOrder = 10;
		state(STATE::DEATH);
		return false;
	}
	return true;
}

// _deadはメンバからしか変更しないからいらないかも
// _aliveはsetを使ってるからそれに合わせて一応
bool Obj::dead(bool dead)
{
	if (alive())
	{
		// 現状爆発が終了した時しか使わないので生存しているときは値を設定しない
		return false;
	}

	_dead = dead;

	return true;
}

const Vector2Dbl& Obj::pos(void)
{
	return _pos;
}

const Vector2Int & Obj::size(void)
{
	return _size;
}

void Obj::exFlag(bool isEX)
{
	_exFlag = isEX;

	return;
}

const UNIT_ID & Obj::unitID(void)
{
	return _unitID;
}

bool Obj::isAnimEnd(void)
{
	// ---------- 範囲チェック
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

// アニメーションのデータを登録
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

// 生存状態確認,死亡状態設定
bool Obj::DestroyProc(void)
{
	if (alive())
	{
		return false;
	}

	if (isAnimEnd())
	{
		dead(true);
	}

	return true;
}
