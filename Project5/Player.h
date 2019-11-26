#pragma once
#include <memory>
#include <Scene\SceneMng.h>
#include "Obj.h"
#include <InputState.h>

/* ----------自機クラス--------- */

class Player :
	public Obj
{
public:
	Player();
	Player(Vector2Dbl pos,Vector2Int size);
	void Update(sharedObj plObj) override;
	~Player();
private:
	void Init();
	std::shared_ptr<InputState> _input;		// 入力管理クラス
};

