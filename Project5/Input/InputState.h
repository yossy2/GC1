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

	const KeyMap& state(void)const;					// キーマップごと取得
	const KeyPair & state(INPUT_ID id)const;		// 指定したキーの押下状態だけ取得
	bool state(INPUT_ID id, int inputData);			// キー状態設定
	void SetOld(void);								// キーのold状態更新
	virtual void Update(void) = 0;
private:
	KeyMap _state;									// 入力可能キーの押下状態,first:new,second:old
};

