#pragma once
#include <memory>
#include <vector>
#include <map>
#include "Vector2.h"

/* ----------�摜�Ǘ��N���X---------- */

class ImageMng;

using VecInt = std::vector<int>;

#define lpImageMng ImageMng::GetInstance()
#define IMAGE_ID(KEY) (ImageMng::GetInstance().GetID(KEY))

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		return *sInstance;
	}

	// �Q�[�����ɌĂяo��
	const VecInt& GetID(const std::string& key);									

	// Init�ŉ摜�̓ǂݍ��݂̍ۂɎg��(LoadGraph)
	const VecInt& GetID(const std::string& key, 
						const std::string& fileName);

	// Init�ŉ摜�̓ǂݍ��݂̍ۂɎg��(LoadDivGraph)
	const VecInt& GetID(const std::string& key,
						const std::string& fileName,		
						const Vector2Int& divSize, 
						const Vector2Int& divCnt);				


private:
	struct ImageMngDeleter
	{
		void operator()(ImageMng* imageMng)const
		{
			delete imageMng;
		}
	};

	ImageMng();
	~ImageMng();
	// private�ň�����ݒ肵�Ă���̂ň�����private�ł�OK�炵��
	static std::unique_ptr<ImageMng,ImageMngDeleter> sInstance;	// �V���O���g��

	// �{����string��key�ɂ���Ɠ��삪�x���Ȃ�̂�enum�Ƃ��̂ق�������
	// ���ƂȂ̂�string���g�p���Ă���
	std::map<std::string, VecInt> _imgMap;		// �摜ID�i�[�ꏊ
};

