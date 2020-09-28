#pragma once

#define NON_ID (-1)			// IDが存在しない状態

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

// extern宣言
#ifdef GAME_DEF
#define GAME_EX
#else
#define GAME_EX extern
#endif

GAME_EX ReplayData* ReplayList;						// ﾘﾌﾟﾚｲﾃﾞｰﾀﾘｽﾄの先頭ﾎﾟｲﾝﾀ
GAME_EX ReplayData* ptrReplay;						// 現在のﾘﾌﾟﾚｲﾃﾞｰﾀﾘｽﾄのｱｸｾｽ位置
GAME_EX int moveID;									// 移動するﾊﾟﾈﾙID:-1のとき操作可能 			
GAME_EX unsigned int shuffleSeed;					// そのﾌﾟﾚｲの乱数のｼｰﾄﾞ値
GAME_EX bool replayFlag;							// ﾘﾌﾟﾚｲﾓｰﾄﾞかどうか
GAME_EX int pzlImageID[PZL_SEL_MAX];
GAME_EX int moveSpeed;		// ﾊﾟﾈﾙの移動ｽﾋﾟｰﾄﾞ
GAME_EX int moveSpeedTable[];		// 分割数に応じた移動速度

// 関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言
bool GameInit(void);		// ｹﾞｰﾑｼｰﾝ初期化
bool GameScene(void);		// ｹﾞｰﾑｼｰﾝ処理
void GameDraw(void);		// ｹﾞｰﾑｼｰﾝ描画
void DrawDivLine(void);		// 分割線の描画

void PanelCtl(void);			// ﾊﾟﾈﾙ移動制御
void SetMovePanel(Pos panel, DIR moveDir);			// ﾊﾟﾈﾙの動きたい方向が抜かれているか調べる
void SwapNum(int *panelID,int *nextPanelID);		// ﾊﾟﾈﾙ入れ替え処理
bool MovePanel(void);								// ﾊﾟﾈﾙ移動処理
void PanelShuffle(void);							// ﾊﾟﾈﾙｼｬｯﾌﾙ処理

void FreeMemData();									// ﾒﾓﾘ開放

bool isClear(void);									// ｸﾘｱﾁｪｯｸ

void AutoClear(void);		// 自動クリア
void BlankMove(void);		// 抜きﾊﾟﾈﾙをautoIDのﾊﾟﾈﾙを動かしたい方向にもっていく
void XLineLastMove(void);	// 横列の最後を揃えたい
void JamaMove(void);		// 列の右端のやつが邪魔
void YLineLastMove(void);	// 縦列の最後を揃えたい
void LastLineMove(void);	// 最後の列を揃えたい
void CycleMove(void);		// 回転ﾓｰﾄﾞ

