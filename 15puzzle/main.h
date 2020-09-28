#pragma once

// ���ޯ��ү���ޗp��`
#ifdef _DEBUG
#define AST(){\
	CHAR ast_mes[256];\
	wsprintf(ast_mes,"%s %d�s��\0",__FILE__,__LINE__);\
	MessageBox(0,ast_mes,"���ĕ\��", MB_OK);\
	}
#else
#define AST()
#endif

// enum��`
enum TRG
{
	TRG_NOW,		// ����
	TRG_OLD,		// 1�ڰёO
	TRG_MAX
};

// define��`
#define SCREEN_SIZE_X 600		// �ްѽ�ذݻ���X
#define SCREEN_SIZE_Y 320		// �ްѽ�ذݻ���Y

#define PUZZLE_SIZE_X 320		// �߽�ى摜����X
#define PUZZLE_SIZE_Y 320		// �߽�ى摜����Y

#define PZL_IMAGE_MAX 3			// �I���ł���摜�̐�

#define BUTTON_SIZE_X 126		// ���݉摜����X
#define BUTTON_SIZE_Y 126		// ���݉摜����Y
#define BUTTON_EDGE_WIDTH 25	// ���݂̂ӂ��̕�
#define BUTTON_ON 1				// ���݉������
#define BUTTON_OFF 0			// ���ݒʏ���

#define SCR_MAX 5

// �\����
// �񎟌����W
struct Pos
{
	int x;
	int y;
};

// ���ڲ�pؽ�
struct ReplayData
{
	Pos pos;			// ϳ����W
	ReplayData* next;	// �����ް��ւ��߲��
};

// extern�錾
#ifdef MAIN_DEF
#define MAIN_EX
#else
#define MAIN_EX extern
#endif

MAIN_EX bool (*sceneFunc)(void);	// ��ݏ����̊֐��߲��

MAIN_EX Pos buttonPos;			// ���݂̒��S���W
MAIN_EX int buttonFlag;			// ���ݏ�ԊǗ��p

MAIN_EX int divID;				// divTable�ɱ�������ID
extern int divTable[5];			// ���������i�[����ð���
MAIN_EX unsigned int clickCnt;	// �萔�ۑ�
MAIN_EX unsigned int minClickCount[SCR_MAX];

// �摜�i�[�p�ϐ�
MAIN_EX int puzzleImg[PZL_IMAGE_MAX];			// �߽�ى摜
MAIN_EX int buttonImg;			// ���݉摜�i�[�p

