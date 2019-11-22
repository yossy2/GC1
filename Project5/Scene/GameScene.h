#pragma once
#include <functional>
#include "BaseScene.h"

/* ----------ゲームシーンクラス--------- */

using funcAct = std::function<bool(ActQueT&, std::vector<sharedObj>&)>;

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
	std::map<ACT_QUE, funcAct> funcQue;
};

