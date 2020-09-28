#pragma once

// enum定義
// ﾏｳｽ制御用
struct Mouse
{
	Pos pos;						// ｶｰｿﾙの座標
	unsigned int trg[TRG_MAX];		// ｸﾘｯｸ押下状態
	int wheel;						// ﾏｳｽの回転量
};

#ifdef MOUSE_DEF
#define MOUSE_EX
#else
#define MOUSE_EX extern
#endif

MOUSE_EX Mouse mouseF;		// ﾏｳｽ制御用

// 関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言
void UpdateMouse(void);		// ﾏｳｽ情報の取得
