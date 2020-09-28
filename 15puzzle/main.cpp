#define MAIN_DEF

#include "DxLib.h"
#include "main.h"
#include "Init.h"
#include "Select.h"
#include "Game.h"
#include "Clear.h"
#include "Mouse.h"

int divTable[] = { 2,4,5,8,10 };		// ï™äÑêîÇäiî[ÇµÇΩ√∞ÃﬁŸ

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	if (!SysInit())
	{
		return -1;
	}

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		UpdateMouse();
		sceneFunc();
	}

	FreeMemData();
	DxLib_End();

	return 0;
}