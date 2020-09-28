#pragma once

// 画像の動く方向
enum SEL_MOVE
{
	SEL_MOVE_NON,			// 移動していない
	SEL_MOVE_RIGHT,			// 右流れ
	SEL_MOVE_LEFT,			// 左流れ
};

// 関数ﾌﾟﾛﾄﾀｲﾌﾟ宣言
bool SelectInit(void);		// 選択ｼｰﾝ初期化
bool SelectScene(void);		// 選択ｼｰﾝ処理
void SelectDraw(void);		// 選択ｼｰﾝ描画

void DrawButton();			// ﾎﾞﾀﾝの描画
bool OnClickCircle();		// ﾏｳｽがﾎﾞﾀﾝの上にあるかﾁｪｯｸ
