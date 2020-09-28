#pragma once

#define NON_ID (-1)			// ID�����݂��Ȃ����

enum DIR
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_MAX,
};

enum PZL_IMAGE
{
	PZL_SEL_NOW,
	PZL_SEL_OLD,
	PZL_SEL_MAX,
};

// extern�錾
#ifdef GAME_DEF
#define GAME_EX
#else
#define GAME_EX extern
#endif

GAME_EX ReplayData* ReplayList;						// ���ڲ�ް�ؽĂ̐擪�߲��
GAME_EX ReplayData* ptrReplay;						// ���݂����ڲ�ް�ؽĂ̱����ʒu
GAME_EX int moveID;									// �ړ���������ID:-1�̂Ƃ�����\ 			
GAME_EX unsigned int shuffleSeed;					// ������ڲ�̗����̼��ޒl
GAME_EX bool replayFlag;							// ���ڲӰ�ނ��ǂ���
GAME_EX int pzlImageID[PZL_SEL_MAX];
GAME_EX int moveSpeed;		// ���ق̈ړ���߰��
GAME_EX int moveSpeedTable[];		// �������ɉ������ړ����x

// �֐��������ߐ錾
bool GameInit(void);		// �ްѼ�ݏ�����
bool GameScene(void);		// �ްѼ�ݏ���
void GameDraw(void);		// �ްѼ�ݕ`��
void DrawDivLine(void);		// �������̕`��

void PanelCtl(void);			// ���وړ�����
void SetMovePanel(Pos panel, DIR moveDir);			// ���ق̓�������������������Ă��邩���ׂ�
void SwapNum(int *panelID,int *nextPanelID);		// ���ٓ���ւ�����
bool MovePanel(void);								// ���وړ�����
void PanelShuffle(void);							// ���ټ���ُ���

void FreeMemData();									// ��؊J��

bool isClear(void);									// �ر����

void AutoClear(void);		// �����N���A
void BlankMove(void);		// �������ق�autoID�����ق𓮂������������ɂ����Ă���
void XLineLastMove(void);	// ����̍Ō�𑵂�����
void JamaMove(void);		// ��̉E�[�̂���ז�
void YLineLastMove(void);	// �c��̍Ō�𑵂�����
void LastLineMove(void);	// �Ō�̗�𑵂�����
void CycleMove(void);		// ��]Ӱ��

