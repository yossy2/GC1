#pragma once
#include <memory>
#include <Obj.h>

/* ----------シーン基底クラス---------- */

class BaseScene;

// アクションQueメンバ
enum class ACT_QUE
{
	NON,
	SHOT,				// 弾発射時
	HIT_CHECK,			// 弾の当たり判定、
	SHAKE,				// 画面揺らし
};

using unique_Base = std::unique_ptr<BaseScene>;
using ActQueT = std::pair<ACT_QUE, Obj&>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;
	virtual void RunActQue(std::vector<ActQueT> actList);
	Vector2Int _screenPos;
};

