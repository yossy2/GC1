#pragma once
#include <vector>
#include "InputState.h"

class KeyState :
	public InputState
{
public:
	KeyState();
	~KeyState();
	void Update(void) override;
private:
	void RefKeyData(void);						// �L�[��ԍX�V���[�h
	void SetKeyConfig(void);					// �R���t�B�O�ύX���[�h
	void (KeyState::*KeyUpdateMode)(void);		// Update�֐��ōs�������������������o�֐��|�C���^
	char _buf[256];								// ���ׂẴL�[���
	int _chgModeKeyOld;							// KeyState�ōs��������ύX����L�[��Old�������
	INPUT_ID _chgConfID;						// �R���t�B�O���[�h�Ō��ݐݒ蒆�̃L�[
	std::vector<int> _keyCon;					// �L�[�R���t�B�O
	std::vector<int> _keyConDef;				// �f�t�H���g�̃L�[�R���t�B�O
};

