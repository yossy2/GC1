#pragma once
#include <vector>
#include <Vector2.h>
#include <Obj.h>

/* ----------敵の動き制御クラス--------- */

#define PIT_IN_CNT_MAX 30		// PitIn移動の時の移動時間
#define SIGMOID_CNT_MAX 60		// シグモイド移動の時の移動時間
#define SPIRAL_CNT_MAX 60		// SPIRAL移動の時の移動時間
#define SPIRAL_RADIUS 70		// SPIRAL移動の初期半径
#define SPREAD_CNT_MAX 120		// 拡散するときの周期

// 敵の移動タイプ
enum class MOVE_TYPE
{
	WAIT,
	SIGMOID,
	SPIRAL,
	PIT_IN,
	LR,
	SPREAD,
	ATTACK,
};

// 左右移動の方向
enum class MOVE_LR
{
	MOVE_LEFT,
	MOVE_RIGHT,
};

using MoveState = std::vector<std::pair<MOVE_TYPE,Vector2Dbl>>;

class EnemyMove
{
public:
	EnemyMove(Vector2Dbl& pos,Vector2Int& size,Vector2Int& num,double& rad,bool& exFlag);
	~EnemyMove();

	void Update(sharedObj plObj);
	bool SetMoveState(MoveState& state, bool newFlag);

	MOVE_TYPE GetMoveType(void);
private:
	void SetMovePrg(void);							 

	// 移動タイプ関数
	void (EnemyMove::*_enemyMoveType)(void);		// 移動タイプを切り替えるメンバ関数ポインタ
	void MoveSigmoid(void);							// シグモイド曲線状に移動(波のような動き)
	void MoveSpiral(void);							// 螺旋状に移動
	void PitIn(void);								// 指定位置に等速直線移動で向かう
	void Wait(void);								// 待機(移動なし)
	void MoveLR(void);								// 左右移動
	void Spread(void);								// 拡散運動
	void MoveAttack(void);							// 突撃

	// 移動制御で使う変数
	// 共通
	int _moveCnt;					// 移動時間カウント
	Vector2Dbl _movePerFrame;		// 1フレームの移動量

	// シグモイド
	Vector2Dbl _length;				// 移動開始位置から目標位置への直線距離

	// SPIRAL
	double _anglePerFrame;			// 1フレームの移動角度
	double _moveRadius;				// 円移動の中心からの距離、だんだん短くなる

	// Spread
	static int _firstSprCnt;		// 敵が一斉に拡散運動し始めた時の最初のカウント

	Vector2Dbl _startPos;			// 各移動関数のスタート地点
	Vector2Dbl _endPos;				// 各移動関数のエンド地点

	Vector2Dbl& _pos;				// 対応している敵オブジェクトの座標
	Vector2Dbl  _posOld;			// 移動前の座標

	Vector2Int& _size;
	Vector2Int& _num;

	double& _rad;					// 敵オブジェクトの向き
	bool& _exFlag;					// 拡散中に突撃するか

	MoveState _aimState;			// 移動タイプとその目標(座標、フレーム、回転角など)のリスト(vector)
	int _aimCnt;					// _aimStateの何番目か

	Vector2Dbl plPos;				// Updateで渡されたPlayerの座標
};

