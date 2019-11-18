#include <_DebugConOut.h>
#include "InputState.h"


InputState::InputState()
{
	// 入力キーの設定
	KeyPair pair = { 0,1 };
	for (auto id : INPUT_ID())
	{
			_state.try_emplace(id, pair);
	}
}


InputState::~InputState()
{
}

// キーマップごと取得
const KeyMap & InputState::state(void) const
{
	return _state;
}

// 指定したキーの押下状態だけ取得
const KeyPair & InputState::state(INPUT_ID id) const
{
	static const KeyPair errorState = KeyPair(0, 0);		// エラー時に返す
	try 
	{
		return _state.at(id);
	}
	catch (...)
	{
		AST();
		return errorState;
	}
}

// キー状態設定
bool InputState::state(INPUT_ID id,int inputData)
{
	if (_state.find(id) != _state.end())
	{
		_state[id].first = inputData;
		return true;
	}
	
	return false;
}

// 入力キーのold状態更新
void InputState::SetOld(void)
{
	for (auto id : INPUT_ID())
	{
		try
		{
			// first  : now
			// second : old
			_state[id].second = _state[id].first;
		}
		catch(...)
		{
			AST();
			_state.emplace(id, KeyPair{ 0,1 });
		}
	}
}
