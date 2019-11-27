#include <DxLib.h>
#include <Scene\/SceneMng.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	lpSceneMng.Run();
	SceneMng::Destroy();
	return 0;
}