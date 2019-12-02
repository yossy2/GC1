#include <DxLib.h>
#include <_DebugConOut.h>
#include "ImageMng.h"

std::unique_ptr<ImageMng,ImageMng::ImageMngDeleter> ImageMng::sInstance(new ImageMng());

// ゲーム中に呼び出す
const VecInt & ImageMng::GetID(const std::string & key)
{
	return GetID(key,key);
}

// Initで画像の読み込みの際に使う(LoadGraph)
const VecInt & ImageMng::GetID(const std::string & key, const std::string & fileName)
{
	if (_imgMap.find(key) == _imgMap.end())
	{
		_imgMap[key].resize(1);
		_imgMap[key][0] = LoadGraph(fileName.c_str(),true);
	}

	return _imgMap[key];
}

// Initで画像の読み込みの際に使う(LoadDivGraph)
const VecInt & ImageMng::GetID(const std::string & key, const std::string & fileName, const Vector2Int & divSize, const Vector2Int & divCnt)
{
	if (_imgMap.find(key) == _imgMap.end())
	{
		_imgMap[key].resize(divCnt.x * divCnt.y);

		if (LoadDivGraph(fileName.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y,
			divSize.x, divSize.y, &_imgMap[key][0], true) == -1)
		{
			AST();
		}
	}	

	return _imgMap[key];
}

ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
