#pragma once
#include <memory>
#include <map>
#include <vector>
#include <tuple>
#include <common/Vector2.h>
#include "BaseScene.h"
#include "LAYER.h"

#define lpSceneMng SceneMng::GetInstance()

// �`��Que�����o
enum class DRAW_QUE
{
	IMAGE,		// �摜ID
	X,			// �`����WX
	Y,			// �`����WY
	RAD,		// ��]�p
	ZORDER,		// �`��D��x(���s)
	LAYER		// �`�惌�C���[
};

using DrawQueT = std::tuple<int, double, double, double, int, LAYER>;

class SceneMng
{
public:
	static SceneMng& GetInstance(void)
	{
		Create();
		return *sInstance;
	}

	static void Create(void)
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}

	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}

	void Run(void);

	bool AddDrawQue(DrawQueT dQue);			// �`��que���N�G�X�g
	bool AddActQue(ActQueT aQue);			// �A�N�V����que���N�G�X�g
	int frameCnt(void);						// get frameCnt�֐�

	const Vector2Int ScreenSize;			// ��ʃT�C�Y
	const Vector2Int ScreenCenter;			// ��ʒ���
	const Vector2Int GameScreenSize;		// �Q�[����ʃT�C�Y
	const Vector2Int GameScreenOffSet;		// �Q�[����ʂ̍�����W

private:
	static SceneMng* sInstance;
	unique_Base _activeScene;

	void Draw(void);
	std::map<LAYER, int> _screenID;			// �`��X�N���[��ID�i�[�ꏊ
	std::vector<DrawQueT> _drawList;		// �`��que�i�[�ꏊ
	std::vector<ActQueT> _actList;			// �A�N�V����Que�i�[�ꏊ

	int _frameCnt;							// �t���[�����ύX����Ă���̃J�E���g

	SceneMng();
	~SceneMng();
	bool SysInit(void);
};

