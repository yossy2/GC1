#pragma once
#include <tuple>
#include <Scene\SceneMng.h>
#include <Obj.h>
#include <EnemyMove.h>

/* ----------敵クラス--------- */

#define ENYMY_CNT_X 10
#define ENYMY_CNT_Y 5

// 敵の種別
enum class ENEMY_TYPE
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	MAX
};

// 敵のステータス、tupleの要素
enum class ENEMY_STATE
{
	TYPE,
	VECTOR,
	SIZE,
	AIM,
	MAX
};

// 敵の初期化時に使うデータを格納したTuple
using EnemyState = std::tuple<ENEMY_TYPE, Vector2Dbl, Vector2Int,MoveState&>;

class Enemy :
	public Obj
{
public:
	Enemy();
	Enemy(EnemyState& state);
	void Update(sharedObj plObj) override;

	void exFlag(bool isEX);

	~Enemy();
private:
	void init();
	EnemyMove _moveCtl{_pos,_rad,_exFlag};		// 個体の移動情報
	ENEMY_TYPE _type;					// 敵の種類（画像）
};

