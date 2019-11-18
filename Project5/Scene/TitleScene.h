#pragma once
#include "BaseScene.h"

/* ----------タイトルシーンクラス--------- */

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own)override;
};

