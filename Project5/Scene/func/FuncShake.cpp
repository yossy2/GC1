#include "FuncShake.h"
#include "../GameScene.h"

bool FuncShake::operator()(ActQueT & que, void* scene)
{
	((GameScene*)scene)->_shakeCnt = 20;
	return false;
}
