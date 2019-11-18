#pragma once
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

class InputState
{
public:
	InputState();
	virtual ~InputState();

	const KeyMap& state(void)const;					// �L�[�}�b�v���Ǝ擾
	const KeyPair & state(INPUT_ID id)const;		// �w�肵���L�[�̉�����Ԃ����擾
	bool state(INPUT_ID id, int inputData);			// �L�[��Ԑݒ�
	void SetOld(void);								// �L�[��old��ԍX�V
	virtual void Update(void) = 0;
private:
	KeyMap _state;									// ���͉\�L�[�̉������,first:new,second:old
};

