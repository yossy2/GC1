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

// オブジェクトの種別
enum class UNIT_ID
{
	NON,
	PLAYER,
	ENEMY,
	PLAYER_BULLET,
	ENEMY_BULLET,
};

class Obj;

using AnimVector = std::vector<std::pair<int, unsigned int>>;
using sharedObj = std::shared_ptr<Obj>;

class Obj
{
public:
	Obj();
	virtual void Update(sharedObj plObj) = 0;
	virtual void Draw(void);		// アニメーションの描画			
	void Draw(int id);		// 好きな画像を指定して描画
	virtual ~Obj();

	// get,set関数 : 変数名の_を外したのが関数名
	STATE state(void)const;							
	bool state(const STATE key);					
	
	bool alive(void) { return _alive; }
	virtual bool alive(bool alive);			// 死んだときの処理はオブジェクトごとに変えたいので仮想関数
	bool dead(void) { return _dead; }
	bool dead(bool dead);

	const Vector2Dbl& pos(void);
	const Vector2Int& size(void);
	
	virtual void exFlag(bool isEX);

	const UNIT_ID& unitID(void);

	bool isAnimEnd(void);							// アニメーションが終了しているか
	bool SetAnim(const STATE key,AnimVector& data);	// アニメーションのデータを登録

protected:
	std::map<STATE, AnimVector> _animMap;			// アニメーション格納マップ
	STATE _state;									// 状態
	unsigned int _animFrame;						// 現在のコマ数
	unsigned int _animCount;						// アニメーションが始まってからのフレーム数のカウント

	bool DestroyProc(void);			// 生存状態確認,死亡状態設定
	bool _alive;					// 生存してるか(かつ爆発していない)
	bool _dead;						// 死亡してるか(かつ爆発が終了した)
	bool _exFlag;					// 特殊行動フラグ(Enemy:突撃開始,Player:キャプチャされる,など)
	UNIT_ID _unitID;				// 種別
	Vector2Dbl _pos;				// 座標
	Vector2Int _size;				// サイズ
	int _zOrder;					// 描画優先度
	double _rad;					// 回転角
	double _speed;					// 速さ
};

