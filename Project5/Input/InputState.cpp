#include <_DebugConOut.h>
#include "InputState.h"


InputState::InputState()
{
	// ���̓L�[�̐ݒ�
	KeyPair pair = { 0,1 };
	for (auto id : INPUT_ID())
	{
			_state.try_emplace(id, pair);
	}
}


InputState::~InputState()
{
}

// �L�[�}�b�v���Ǝ擾
const KeyMap & InputState::state(void) const
{
	return _state;
}

// �w�肵���L�[�̉�����Ԃ����擾
const KeyPair & InputState::state(INPUT_ID id) const
{
	static const KeyPair errorState = KeyPair(0, 0);		// �G���[���ɕԂ�
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

// �L�[��Ԑݒ�
bool InputState::state(INPUT_ID id,int inputData)
{
	if (_state.find(id) != _state.end())
	{
		_state[id].first = inputData;
		return true;
	}
	
	return false;
}

// ���̓L�[��old��ԍX�V
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
