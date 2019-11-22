#include "FuncBullet.h"
#include <Bullet.h>

bool FuncBullet::operator()(ActQueT que, std::vector<sharedObj> objList)
{
	UNIT_ID id;
	if (que.second.unitID() == UNIT_ID::PLAYER)
	{
		id = UNIT_ID::PLAYER_BULLET;
	}
	else
	{
		id = UNIT_ID::ENEMY_BULLET;
	}
	objList.emplace_back(new Bullet(que.second.pos(), id));
	return true;
}
