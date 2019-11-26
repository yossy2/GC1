#pragma once
#include <Scene\SceneMng.h>
#include "Obj.h"

class Bullet :
	public Obj
{
public:
	Bullet();
	Bullet(Vector2Dbl pos,UNIT_ID id);
	~Bullet();
	void Update(sharedObj plObj) override;

private:
	void Init(void);
};

