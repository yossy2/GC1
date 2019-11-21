#pragma once
#include <Scene\SceneMng.h>
#include "Obj.h"

class Bullet :
	public Obj
{
public:
	Bullet();
	Bullet(Vector2Dbl pos);
	~Bullet();
	void Update(void) override;

private:
	void Init(void);
};

