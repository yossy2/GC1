#pragma once
#include "BaseScene.h"

/* ----------�^�C�g���V�[���N���X--------- */

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own)override;
};

