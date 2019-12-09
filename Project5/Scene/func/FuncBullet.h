#pragma once
#include <Scene\BaseScene.h>

// 弾の発射判定関数オブジェクト

struct FuncBullet
{
	bool operator()(ActQueT& que, void* scene);

private:
	static std::map<UNIT_ID, int> _MaxCount;
};

