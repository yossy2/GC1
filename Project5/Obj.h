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

class Obj;

using AnimVector = std::vector<std::pair<int, unsigned int>>;
using sharedObj = std::shared_ptr<Obj>;

class Obj
{
public:
	Obj();
	virtual void Update(void) = 0;
	void Draw(void);		// �A�j���[�V�����̕`��			
	void Draw(int id);		// �D���ȉ摜���w�肵�ĕ`��
	virtual ~Obj();

	// ���get,set
	STATE state(void)const;							// �A�j���[�V������Ԑݒ�
	bool state(const STATE key);					// �A�j���[�V������Ԃ́H
	
	// �������get,set
	bool isAlive(void) { return _alive; }			// �������Ă�H
	bool setAlive(bool alive);						// alive::�I�u�W�F�N�g�̐������
	bool isDead(void) { return _dead; }				// �����I����Ă�H
	bool setDead(bool dead);						// dead::�I�u�W�F�N�g�����S��Ԏ��A�������I�����Ă��邩�̏��

	// ���Wget,set
	const Vector2Dbl& pos(void);

	bool isAnimEnd(void);							// �A�j���[�V�������I�����Ă��邩
	bool SetAnim(const STATE key,AnimVector& data);	// �A�j���[�V�����̃f�[�^��o�^

private:
	std::map<STATE, AnimVector> _animMap;			// �A�j���[�V�����i�[�}�b�v
	STATE _state;									// ���
	unsigned int _animFrame;						// ���݂̃R�}��
	unsigned int _animCount;						// �A�j���[�V�������n�܂��Ă���̃t���[�����̃J�E���g

protected:
	bool DestroyProc(void);			// ������Ԋm�F,���S��Ԑݒ�
	bool _alive;					// �������Ă邩(���������Ă��Ȃ�)
	bool _dead;						// ���S���Ă邩(���������I������)
	Vector2Dbl _pos;				// ���W
	Vector2Int _size;				// �T�C�Y
	int _zOrder;					// �`��D��x
	double _rad;					// ��]�p
	double _speed;					// ����
};

