#include <DxLib.h>
#include "KeyState.h"
#include <_DebugConOut.h>


KeyState::KeyState()
{
	_keyConDef.reserve(static_cast<size_t>(end(INPUT_ID())));
	_keyConDef.emplace_back(KEY_INPUT_LEFT);	// LEFT
	_keyConDef.emplace_back(KEY_INPUT_RIGHT);	// RIGHT
	_keyConDef.emplace_back(KEY_INPUT_UP);		// UP
	_keyConDef.emplace_back(KEY_INPUT_DOWN);	// DOWN
	_keyConDef.emplace_back(KEY_INPUT_Z);		// BTN1
	_keyConDef.emplace_back(KEY_INPUT_X);		// BTN2
	_keyConDef.emplace_back(KEY_INPUT_A);		// BTN3
	_keyConDef.emplace_back(KEY_INPUT_S);		// BTN4

	// �L�[�R���t�B�O�ǂݍ���
	FILE *fp;
	fopen_s(&fp,"data/key.dat","rb");

	if (fp != nullptr)
	{
		_keyCon.resize(static_cast<int>(end(INPUT_ID())));
		fread(_keyCon.data(), sizeof (_keyCon[0]), _keyCon.size(), fp);
		fclose(fp);
	}
	else
	{
		_keyCon = _keyConDef;
	}

	_chgModeKeyOld = 1;

	// �֐��|�C���^�ݒ�
	KeyUpdateMode = &KeyState::RefKeyData;
}


KeyState::~KeyState()
{
}

void KeyState::Update(void)
{
	// old�̐ݒ�
	SetOld();
	_chgModeKeyOld = _buf[KEY_INPUT_F1];
	GetHitKeyStateAll(_buf);
	(this->*KeyUpdateMode)();
}

void KeyState::RefKeyData(void)
{
	for (auto id : INPUT_ID())
	{
		state(id, _buf[_keyCon[static_cast<int>(id)]]);
	}
	
	// �R���t�B�O���[�h�ؑ�
	if (_buf[KEY_INPUT_F1] && !_chgModeKeyOld)
	{
		for (auto id : INPUT_ID())
		{
			state(id, 0);
		}
		KeyUpdateMode = &KeyState::SetKeyConfig;
		_chgConfID = begin(INPUT_ID());
		
		TRACE("�L�[�R���t�B�O�J�n\n");
		TRACE("changeID : %d\n", static_cast<int>(_chgConfID));
	}
}

void KeyState::SetKeyConfig(void)
{
	// �L�[�X�V���[�h�ؑ�
	if (_buf[KEY_INPUT_F1] && !_chgModeKeyOld)
	{
		KeyUpdateMode = &KeyState::RefKeyData;
		TRACE("�L�[�R���t�B�O�I��\n");
		return;
	}
	
	// ����key�����ł�_keyCon�ɐݒ肳��Ă�����false
	auto checkKey = [&](int key) {
		for (INPUT_ID id = begin(INPUT_ID()); id < _chgConfID; ++id)
		{
			if (_keyCon[static_cast<int>(id)] == key)
			{
				return false;
			}
		}
		return true;
	};

	// ������Ă�L�[�̌���
	for (int keyNum = 0; keyNum < sizeof(_buf); keyNum++)
	{
		if (_buf[keyNum])
		{
			// F1�L�[�͎g�����Ⴞ��
			if (keyNum == KEY_INPUT_F1)
			{
				continue;
			}

			// �����g���Ă���L�[�͎g�����Ⴞ��
			if (!checkKey(keyNum))
			{
				continue;
			}

			// �g���ĂȂ��L�[��������ݒ�OK
			TRACE("%3d -> %3d\n", _keyCon[static_cast<int>(_chgConfID)], keyNum);
			_keyCon[static_cast<int>(_chgConfID)] = keyNum;
			++_chgConfID;
			TRACE("changeID : %d\n", static_cast<int>(_chgConfID));

			// ���ׂẴL�[��ݒ肵�I����Ă�����R���t�B�O���[�h�I��
			if (_chgConfID >= end(_chgConfID))
			{
				// �L�[�R���t�B�O�����o��(�Z�[�u)
				FILE* fp;
				fopen_s(&fp, "data/key.dat", "wb");

				if (fp != nullptr)
				{
					fwrite(_keyCon.data(), sizeof (_keyCon[0]), _keyCon.size(), fp);
					fclose(fp);
				}
				else
				{
					AST();
				}
				TRACE("�L�[�R���t�B�O���Z�[�u���܂���\n");

				TRACE("�L�[�R���t�B�O�I��\n");
				KeyUpdateMode = &KeyState::RefKeyData;
			}
			return;
		}
	}
}
