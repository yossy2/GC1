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

	// キーコンフィグ読み込み
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

	// 関数ポインタ設定
	KeyUpdateMode = &KeyState::RefKeyData;
}


KeyState::~KeyState()
{
}

void KeyState::Update(void)
{
	// oldの設定
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
	
	// コンフィグモード切替
	if (_buf[KEY_INPUT_F1] && !_chgModeKeyOld)
	{
		for (auto id : INPUT_ID())
		{
			state(id, 0);
		}
		KeyUpdateMode = &KeyState::SetKeyConfig;
		_chgConfID = begin(INPUT_ID());
		
		TRACE("キーコンフィグ開始\n");
		TRACE("changeID : %d\n", static_cast<int>(_chgConfID));
	}
}

void KeyState::SetKeyConfig(void)
{
	// キー更新モード切替
	if (_buf[KEY_INPUT_F1] && !_chgModeKeyOld)
	{
		KeyUpdateMode = &KeyState::RefKeyData;
		TRACE("キーコンフィグ終了\n");
		return;
	}
	
	// 引数keyがすでに_keyConに設定されていたらfalse
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

	// 押されてるキーの検索
	for (int keyNum = 0; keyNum < sizeof(_buf); keyNum++)
	{
		if (_buf[keyNum])
		{
			// F1キーは使っちゃだめ
			if (keyNum == KEY_INPUT_F1)
			{
				continue;
			}

			// もう使っているキーは使っちゃだめ
			if (!checkKey(keyNum))
			{
				continue;
			}

			// 使ってないキーだったら設定OK
			TRACE("%3d -> %3d\n", _keyCon[static_cast<int>(_chgConfID)], keyNum);
			_keyCon[static_cast<int>(_chgConfID)] = keyNum;
			++_chgConfID;
			TRACE("changeID : %d\n", static_cast<int>(_chgConfID));

			// すべてのキーを設定し終わっていたらコンフィグモード終了
			if (_chgConfID >= end(_chgConfID))
			{
				// キーコンフィグ書き出し(セーブ)
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
				TRACE("キーコンフィグをセーブしました\n");

				TRACE("キーコンフィグ終了\n");
				KeyUpdateMode = &KeyState::RefKeyData;
			}
			return;
		}
	}
}
