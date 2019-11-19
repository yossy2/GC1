#pragma once
#include <vector>
#include <Vector2.h>

/* ----------�G�̓�������N���X--------- */

#define PIT_IN_CNT_MAX 60		// PitIn�ړ��̎��̈ړ�����
#define SIGMOID_CNT_MAX 120		// �V�O���C�h�ړ��̎��̈ړ�����
#define SPIRAL_CNT_MAX 120		// SPIRAL�ړ��̎��̈ړ�����
#define SPIRAL_RADIUS 70		// SPIRAL�ړ��̏������a
#define SPREAD_CNT_MAX 120		// �g�U����Ƃ��̎���

// �G�̈ړ��^�C�v
enum class MOVE_TYPE
{
	WAIT,
	SIGMOID,
	SPIRAL,
	PIT_IN,
	LR,
	SPREAD,
};

// ���E�ړ��̕���
enum class MOVE_LR
{
	MOVE_LEFT,
	MOVE_RIGHT,
};

using MoveState = std::vector<std::pair<MOVE_TYPE,Vector2Dbl>>;

class EnemyMove
{
public:
	EnemyMove(Vector2Dbl& pos,double& rad);
	~EnemyMove();

	void Update(void);
	bool SetMoveState(MoveState& state, bool newFlag);	// 
private:
	void SetMovePrg(void);							// 

	// �ړ��^�C�v�֐�
	void (EnemyMove::*_enemyMoveType)(void);		// �ړ��^�C�v��؂�ւ��郁���o�֐��|�C���^
	void MoveSigmoid(void);							// �V�O���C�h�Ȑ���Ɉړ�(�g�̂悤�ȓ���)
	void MoveSpiral(void);							// ������Ɉړ�
	void PitIn(void);								// �w��ʒu�ɓ��������ړ��Ō�����
	void Wait(void);								// �ҋ@(�ړ��Ȃ�)
	void MoveLR(void);								// ���E�ړ�
	void Spread(void);								// �g�U�^��

	// �ړ�����Ŏg���ϐ�
	// ����
	int _moveCnt;					// �ړ����ԃJ�E���g
	Vector2Dbl _movePerFrame;		// 1�t���[���̈ړ���

	// �V�O���C�h
	Vector2Dbl _length;				// �ړ��J�n�ʒu����ڕW�ʒu�ւ̒�������

	// SPIRAL
	double _anglePerFrame;			// 1�t���[���̈ړ��p�x
	double _moveRadius;				// �~�ړ��̒��S����̋����A���񂾂�Z���Ȃ�

	// LR
	static MOVE_LR _moveLR;			// �G�̉�S�̂̈ړ�����

	Vector2Dbl _startPos;			// �e�ړ��֐��̃X�^�[�g�n�_
	Vector2Dbl _endPos;				// �e�ړ��֐��̃G���h�n�_

	Vector2Dbl& _pos;				// �Ή����Ă���G�I�u�W�F�N�g�̍��W
	Vector2Dbl  _posOld;			// �ړ��O�̍��W

	double& _rad;					// �G�I�u�W�F�N�g�̌���

	MoveState _aimState;			// �ړ��^�C�v�Ƃ��̖ڕW(���W�A�t���[���A��]�p�Ȃ�)�̃��X�g(vector)
	int _aimCnt;					// _aimState�̉��Ԗڂ�
};

