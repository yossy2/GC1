#pragma once
#include <Scene\BaseScene.h>

// �e�̔��˔���֐��I�u�W�F�N�g

struct FuncBullet
{
	bool operator()(ActQueT& que, std::vector<sharedObj>& objList);

private:
	static std::map<UNIT_ID, int> _MaxCount;
};
