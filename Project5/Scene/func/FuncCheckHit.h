#pragma once
#include <Scene\BaseScene.h>

struct FuncCheckHit
{
	bool operator()(ActQueT& que, std::vector<sharedObj>& objList);
};