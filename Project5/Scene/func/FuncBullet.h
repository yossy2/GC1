#pragma once
#include <Scene\BaseScene.h>

// �e�̔��˔���֐��I�u�W�F�N�g

struct FuncBullet
{
	bool operator()(ActQueT& que, void* scene);

private:
	static std::map<UNIT_ID, int> _MaxCount;
};

