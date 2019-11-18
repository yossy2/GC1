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
	void RefKeyData(void);						// キー状態更新モード
	void SetKeyConfig(void);					// コンフィグ変更モード
	void (KeyState::*KeyUpdateMode)(void);		// Update関数で行う処理が入ったメンバ関数ポインタ
	char _buf[256];								// すべてのキー情報
	int _chgModeKeyOld;							// KeyStateで行う処理を変更するキーのOld押下状態
	INPUT_ID _chgConfID;						// コンフィグモードで現在設定中のキー
	std::vector<int> _keyCon;					// キーコンフィグ
	std::vector<int> _keyConDef;				// デフォルトのキーコンフィグ
};

