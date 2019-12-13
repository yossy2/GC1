#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <Vector2.h>
#include <ImageMng.h>

/* ----------�I�u�W�F�N�g���N���X--------- */

// �I�u�W�F�N�g�̏��
enum class STATE
{
	NORMAL,		// �ʏ�
	EX,			// ����
	DEATH,		// ����
	MAX,
};

// �I�u�W�F�N�g�̎��
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
	virtual void Draw(void);		// �A�j���[�V�����̕`��			
	void Draw(int id);		// �D���ȉ摜���w�肵�ĕ`��
	virtual ~Obj();

	// get,set�֐� : �ϐ�����_���O�����̂��֐���
	STATE state(void)const;							
	bool state(const STATE key);					
	
	bool alive(void) { return _alive; }
	virtual bool alive(bool alive);			// ���񂾂Ƃ��̏����̓I�u�W�F�N�g���Ƃɕς������̂ŉ��z�֐�
	bool dead(void) { return _dead; }
	bool dead(bool dead);

	const Vector2Dbl& pos(void);
	const Vector2Int& size(void);
	
	virtual void exFlag(bool isEX);

	const UNIT_ID& unitID(void);

	bool isAnimEnd(void);							// �A�j���[�V�������I�����Ă��邩
	bool SetAnim(const STATE key,AnimVector& data);	// �A�j���[�V�����̃f�[�^��o�^

protected:
	std::map<STATE, AnimVector> _animMap;			// �A�j���[�V�����i�[�}�b�v
	STATE _state;									// ���
	unsigned int _animFrame;						// ���݂̃R�}��
	unsigned int _animCount;						// �A�j���[�V�������n�܂��Ă���̃t���[�����̃J�E���g

	bool DestroyProc(void);			// ������Ԋm�F,���S��Ԑݒ�
	bool _alive;					// �������Ă邩(���������Ă��Ȃ�)
	bool _dead;						// ���S���Ă邩(���������I������)
	bool _exFlag;					// ����s���t���O(Enemy:�ˌ��J�n,Player:�L���v�`�������,�Ȃ�)
	UNIT_ID _unitID;				// ���
	Vector2Dbl _pos;				// ���W
	Vector2Int _size;				// �T�C�Y
	int _zOrder;					// �`��D��x
	double _rad;					// ��]�p
	double _speed;					// ����
};

