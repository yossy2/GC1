#pragma once
#include "BaseScene.h"
#include <Obj.h>

/* ----------�Q�[���V�[���N���X--------- */

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own) override;
private:
	std::vector<sharedObj> _objList;
};

