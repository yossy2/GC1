#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Vector2.h>
#include <ImageMng.h>

/* ----------オブジェクト基底クラス--------- */

// オブジェクトの状態
enum class STATE
{
	NORMAL,		// 通常
	EX,			// 特殊
	DEATH,		// 爆発
	MAX,
};

class Obj;

using AnimVector = std::vector<std::pair<int, unsigned int>>;
using sharedObj = std::shared_ptr<Obj>;

class Obj
{
public:
	Obj();
	virtual void Update(void) = 0;
	void Draw(void);		// アニメーションの描画			
	void Draw(int id);		// 好きな画像を指定して描画
	virtual ~Obj();

	// 状態get,set
	STATE state(void)const;							// アニメーション状態設定
	bool state(const STATE key);					// アニメーション状態は？
	
	// 生死状態get,set
	bool isAlive(void) { return _alive; }			// 生存してる？
	bool setAlive(bool alive);						// alive::オブジェクトの生存状態
	bool isDead(void) { return _dead; }				// 爆発終わってる？
	bool setDead(bool dead);						// dead::オブジェクトが死亡状態時、爆発が終了しているかの状態

	// 座標get,set
	const Vector2Dbl& pos(void);

	bool isAnimEnd(void);							// アニメーションが終了しているか
	bool SetAnim(const STATE key,AnimVector& data);	// アニメーションのデータを登録

private:
	std::map<STATE, AnimVector> _animMap;			// アニメーション格納マップ
	STATE _state;									// 状態
	unsigned int _animFrame;						// 現在のコマ数
	unsigned int _animCount;						// アニメーションが始まってからのフレーム数のカウント

protected:
	bool DestroyProc(void);			// 生存状態確認,死亡状態設定
	bool _alive;					// 生存してるか(かつ爆発していない)
	bool _dead;						// 死亡してるか(かつ爆発が終了した)
	Vector2Dbl _pos;				// 座標
	Vector2Int _size;				// サイズ
	int _zOrder;					// 描画優先度
	double _rad;					// 回転角
	double _speed;					// 速さ
};

