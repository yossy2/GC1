#pragma once
#include "BaseScene.h"

/* ----------�Q�[���V�[���N���X--------- */

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own) override;
private:
	void RunActQue(std::vector<ActQueT> actList) override;
	std::vector<sharedObj> _objList;
};

