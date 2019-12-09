#include <algorithm>
#include "FuncBullet.h"
#include <Bullet.h>
#include "../GameScene.h"

std::map<UNIT_ID, int> FuncBullet::_MaxCount = { {UNIT_ID::PLAYER_BULLET,2},{UNIT_ID::ENEMY_BULLET,2} };

bool FuncBullet::operator()(ActQueT& que, void* scene)
{
	UNIT_ID id = que.second.unitID() == UNIT_ID::PLAYER ?
				 UNIT_ID::PLAYER_BULLET :
				 UNIT_ID::ENEMY_BULLET;
	
	// íeèoåªè„å¿ämîF
	if (_MaxCount[id] <= std::count_if(((GameScene*)scene)->_objList.begin(), ((GameScene*)scene)->_objList.end(), [&](sharedObj obj) {return (*obj).unitID() == id; }))
	{
		return false;
	}

	((GameScene*)scene)->_objList.emplace_back(new Bullet(que.second.pos(), id));
	return true;
}
