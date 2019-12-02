#pragma once
#include <memory>
#include <vector>
#include <map>
#include "Vector2.h"

/* ----------画像管理クラス---------- */

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

	// ゲーム中に呼び出す
	const VecInt& GetID(const std::string& key);									

	// Initで画像の読み込みの際に使う(LoadGraph)
	const VecInt& GetID(const std::string& key, 
						const std::string& fileName);

	// Initで画像の読み込みの際に使う(LoadDivGraph)
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
	// privateで引数を設定しているので引数がprivateでもOKらしい
	static std::unique_ptr<ImageMng,ImageMngDeleter> sInstance;	// シングルトン

	// 本当はstringをkeyにすると動作が遅くなるのでenumとかのほうがいい
	// 授業なのでstringを使用している
	std::map<std::string, VecInt> _imgMap;		// 画像ID格納場所
};

