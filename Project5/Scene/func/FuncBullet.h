#pragma once
#include <Scene\BaseScene.h>

// 弾の発射判定関数オブジェクト

struct FuncBullet
{
	bool operator()(ActQueT& que, std::vector<sharedObj>& objList);

private:
	static std::map<UNIT_ID, int> _MaxCount;
};

