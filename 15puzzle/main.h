#pragma once

// ﾃﾞﾊﾞｯｸﾞﾒｯｾｰｼﾞ用定義
#ifdef _DEBUG
#define AST(){\
	CHAR ast_mes[256];\
	wsprintf(ast_mes,"%s %d行目\0",__FILE__,__LINE__);\
	MessageBox(0,ast_mes,"ｱｻｰﾄ表示", MB_OK);\
	}
#else
#define AST()
#endif

// enum定義
enum TRG
{
	TRG_NOW,		// 現在
	TRG_OLD,		// 1ﾌﾚｰﾑ前
	TRG_MAX
};

// define定義
#define SCREEN_SIZE_X 600		// ｹﾞｰﾑｽｸﾘｰﾝｻｲｽﾞX
#define SCREEN_SIZE_Y 320		// ｹﾞｰﾑｽｸﾘｰﾝｻｲｽﾞY

#define PUZZLE_SIZE_X 320		// ﾊﾟｽﾞﾙ画像ｻｲｽﾞX
#define PUZZLE_SIZE_Y 320		// ﾊﾟｽﾞﾙ画像ｻｲｽﾞY

#define PZL_IMAGE_MAX 3			// 選択できる画像の数

#define BUTTON_SIZE_X 126		// ﾎﾞﾀﾝ画像ｻｲｽﾞX
#define BUTTON_SIZE_Y 126		// ﾎﾞﾀﾝ画像ｻｲｽﾞY
#define BUTTON_EDGE_WIDTH 25	// ﾎﾞﾀﾝのふちの幅
#define BUTTON_ON 1				// ﾎﾞﾀﾝ押下状態
#define BUTTON_OFF 0			// ﾎﾞﾀﾝ通常状態

#define SCR_MAX 5

// 構造体
// 二次元座標
struct Pos
{
	int x;
	int y;
};

// ﾘﾌﾟﾚｲ用ﾘｽﾄ
struct ReplayData
{
	Pos pos;			// ﾏｳｽ座標
	ReplayData* next;	// 次のﾃﾞｰﾀへのﾎﾟｲﾝﾀ
};

// extern宣言
#ifdef MAIN_DEF
#define MAIN_EX
#else
#define MAIN_EX extern
#endif

MAIN_EX bool (*sceneFunc)(void);	// ｼｰﾝ処理の関数ﾎﾟｲﾝﾀ

MAIN_EX Pos buttonPos;			// ﾎﾞﾀﾝの中心座標
MAIN_EX int buttonFlag;			// ﾎﾞﾀﾝ状態管理用

MAIN_EX int divID;				// divTableにｱｸｾｽするID
extern int divTable[5];			// 分割数を格納したﾃｰﾌﾞﾙ
MAIN_EX unsigned int clickCnt;	// 手数保存
MAIN_EX unsigned int minClickCount[SCR_MAX];

// 画像格納用変数
MAIN_EX int puzzleImg[PZL_IMAGE_MAX];			// ﾊﾟｽﾞﾙ画像
MAIN_EX int buttonImg;			// ﾎﾞﾀﾝ画像格納用

