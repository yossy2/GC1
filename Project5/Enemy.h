#pragma once
#include <tuple>
#include <Scene\SceneMng.h>
#include <Obj.h>
#include <EnemyMove.h>

/* ----------�G�N���X--------- */

#define ENEMY_CNT_X 10
#define ENYMY_CNT_Y 5

// �G�̎��
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

// �G�̃X�e�[�^�X�Atuple�̗v�f
enum class ENEMY_STATE
{
	TYPE,
	VECTOR,
	SIZE,
	NUM,
	AIM,
	MAX
};

// �G�̏��������Ɏg���f�[�^���i�[����Tuple
using EnemyState = std::tuple<ENEMY_TYPE, Vector2Dbl, Vector2Int,Vector2Int,MoveState&>;

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
	bool alive(bool alive);

	EnemyMove _moveCtl{_pos,_size,_num,_rad,_exFlag};		// �̂̈ړ����
	ENEMY_TYPE _type;					// �G�̎�ށi�摜�j
	Vector2Int _num;					// �G�G���A�̂ǂ��ɔz�u����邩
};

