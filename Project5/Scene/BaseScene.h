#pragma once
#include <memory>
#include <Obj.h>

/* ----------�V�[�����N���X---------- */

class BaseScene;

// �A�N�V����Que�����o
enum class ACT_QUE
{
	NON,
	SHOT,				// �e���ˎ�
	HIT_CHECK,			// �e�̓����蔻��A
	SHAKE,				// ��ʗh�炵
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

