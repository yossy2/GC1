#pragma once
#include <memory>

/* ----------�V�[�����N���X---------- */

class BaseScene;

using unique_Base = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;
};

