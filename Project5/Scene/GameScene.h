#pragma once
#include <functional>
#include "BaseScene.h"
#include <Obj.h>
/* ----------ゲームシーンクラス--------- */

using funcAct = std::function<bool(ActQueT&, void*)>;

struct FuncCheckHit;
struct FuncBullet;
struct FuncShake;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	unique_Base Update(unique_Base own) override;
private:
	friend FuncCheckHit;
	friend FuncBullet;
	friend FuncShake;
	void InitFunc(void);
	void RunActQue(std::vector<ActQueT> actList) override;
	std::vector<sharedObj> _objList;
	std::map<ACT_QUE, funcAct> _funcQue;
	int _shakeCnt;		// 画面揺らしのカウント
};

