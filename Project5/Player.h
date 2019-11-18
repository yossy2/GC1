#pragma once
#include "Obj.h"
#include <memory>
#include <InputState.h>

/* ----------自機クラス--------- */

class Player :
	public Obj
{
public:
	Player();
	Player(Vector2Dbl pos,Vector2Int size);
	void Update(void) override;
	~Player();
private:
	void Init();
	std::shared_ptr<InputState> _input;		// 入力管理クラス
};

