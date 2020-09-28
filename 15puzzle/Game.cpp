#define GAME_DEF
#include <time.h>
#include <math.h>
#include "DxLib.h"
#include "main.h"
#include "Mouse.h"
#include "Game.h"
#include "Clear.h"


int **pzData;		// 分割画像ID格納用
Pos chipSize;		// 分割した際のﾁｯﾌﾟｻｲｽﾞ
int blankID;		// 抜きﾊﾟﾈﾙID
int blankAnimCount;	// 抜きﾊﾟﾈﾙのｱﾆﾒｰｼｮﾝｶｳﾝﾄ
Pos mouseChip;		// ﾏｳｽが乗ってるﾊﾟﾈﾙ位置
int moveSpeedTable[] = { 80,40,32,20,16 };

// 移動ﾊﾟﾈﾙ制御用
Pos movePanelPos;	// 移動ﾊﾟﾈﾙの座標
Pos moveEndPos;		// 移動終了地点の座標

// ｵｰﾄﾓｰﾄﾞ用
int autoID;			// ｵｰﾄﾓｰﾄﾞで次に揃えたいﾊﾟﾈﾙのID
bool autoFlag;		// ｵｰﾄﾓｰﾄﾞかどうか
bool blankMove;		// 抜きﾊﾟﾈﾙの特別な移動ﾓｰﾄﾞ、動かしたいﾊﾟﾈﾙの移動先に抜きﾊﾟﾈﾙをうごかす
DIR blankMoveDir;	// 抜きﾊﾟﾈﾙを動かしていきたい方向
DIR avoDir;			// 回避した方向
int blankMoveCnt;	// blankMoveを行った回数、計4回で移動先に行ける
Pos blank;			// blankIDの場所を記憶

bool xLineLast;		// 横列の最後の二つをそろえるﾓｰﾄﾞ
bool yLineLast;
int lineNumY;		// 揃えたい横列の番号
int lineNumX;		// 揃えたい縦列の番号

bool jamaFlag;		// 列の右端のﾊﾟﾈﾙが邪魔な時にするﾓｰﾄﾞ
bool cycleFlag;		// 回転ﾓｰﾄﾞ

bool autoLastLine;	// 最後の二列をそろえるぞ

// ｹﾞｰﾑｼｰﾝ初期化
bool GameInit(void)
{
	bool rtnFlag = true;

	if ((divID < 0) || (divID >= (sizeof(divTable) / sizeof(divTable[0]))))
	{
		AST();
		divID = 0;
	}
	shuffleSeed = (unsigned int)time(NULL);
	srand(shuffleSeed);
	// ﾒﾓﾘが残ってたら解放
	FreeMemData();

	// 動的にﾒﾓﾘを確保
	pzData = (int **)malloc(sizeof(int*) * divTable[divID]);
	if (pzData == NULL)
	{
		AST();
		return false;
	}

	*pzData = (int *)malloc(sizeof(int) * divTable[divID] * divTable[divID]);
	if (*pzData == NULL)
	{
		AST();
		return false;
	}

	for (int i = 1; i < divTable[divID]; i++)
	{
		pzData[i] = pzData[0] + i * divTable[divID];
	}

	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			pzData[y][x] = x + y * divTable[divID];
			//pzData[y][x] = y + x * divTable[divID];
		}
	}

	chipSize.x = PUZZLE_SIZE_X / divTable[divID];
	chipSize.y = PUZZLE_SIZE_Y / divTable[divID];

	//blankID = rand() % (divTable[divID] * divTable[divID]);
	blankID = divTable[divID] * divTable[divID] - 1;
	blankAnimCount = 0;
	moveID = NON_ID;
	moveSpeed = moveSpeedTable[divID];

	autoID = 0;
	autoFlag = false;
	xLineLast = false;
	lineNumY = 0;
	lineNumX = 0;
	blankMove = false;
	blankMoveCnt = 0;
	jamaFlag = false;
	autoLastLine = false;
	cycleFlag = false;

	replayFlag = false;

	PanelShuffle();

	clickCnt = 0;

	sceneFunc = GameScene;

	return rtnFlag;
}

// ｹﾞｰﾑｼｰﾝ処理
bool GameScene(void)
{
	mouseChip.x = (mouseF.pos.x / chipSize.x);
	mouseChip.y = (mouseF.pos.y / chipSize.y);

	PanelCtl();
	if (autoFlag && moveID == NON_ID)
	{
		if (blankMove)
		{
			BlankMove();
		}
		else if (xLineLast)
		{
			XLineLastMove();
		}
		else if (yLineLast)
		{
			YLineLastMove();
		}
		else if (jamaFlag)
		{
			JamaMove();
		}
		else if (autoLastLine)
		{
			LastLineMove();
		}
		else if (cycleFlag)
		{
			CycleMove();
		}
		else
		{
			AutoClear();
		}
	}	

	GameDraw();

	blankAnimCount++;
	
	if (moveID == NON_ID)
	{
		if (isClear())
		{
			autoFlag = false;
			xLineLast = false;
			blankMove = false;
			jamaFlag = false;
			autoLastLine = false;
			cycleFlag = false;
			ClearInit();

			return true;
		}
	}

	if (CheckHitKey(KEY_INPUT_SPACE) && !autoFlag)
	{
		autoFlag = true;
	}
	else if(CheckHitKey(KEY_INPUT_X) && autoFlag)
	{
		autoFlag = false;
	}
	else
	{
		// 何もしない
	}
	return false;
}

// ｹﾞｰﾑｼｰﾝ描画
void GameDraw(void)
{
	ClsDrawScreen();

	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] != blankID && pzData[y][x] != moveID)
			{
				if (pzData[y][x] != y * divTable[divID] + x)
				{
					SetDrawBright(200, 200, 200);
				}
				DrawRectGraph(x * chipSize.x, y * chipSize.y,
					pzData[y][x] % divTable[divID] * chipSize.x, pzData[y][x] / divTable[divID] * chipSize.y,
					chipSize.x, chipSize.y, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
				SetDrawBright(255, 255, 255);
			}
			else if (moveID == -1 && pzData[y][x] == blankID)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 - (int)(cos((double)blankAnimCount / 20) * 100));

				DrawBox(x * chipSize.x, y * chipSize.y,
					(x + 1) * chipSize.x, (y + 1) * chipSize.y,
					0x0000ff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			else
			{

			}
		}
	}

	if (mouseChip.x >= 0 && mouseChip.x < divTable[divID] &&
		mouseChip.y >= 0 && mouseChip.y < divTable[divID] &&
		pzData[mouseChip.y][mouseChip.x] != blankID && !replayFlag)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(mouseChip.x * chipSize.x, mouseChip.y * chipSize.y,
			(mouseChip.x + 1) * chipSize.x, (mouseChip.y+ 1) * chipSize.y,
			0xaaaa00, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawDivLine();

	// 移動ﾊﾟﾈﾙ描画
	if (moveID != NON_ID)
	{
		SetDrawBright(200, 200, 200);
		DrawRectGraph(movePanelPos.x, movePanelPos.y,
			moveID % divTable[divID] * chipSize.x, moveID / divTable[divID] * chipSize.y,
			chipSize.x, chipSize.y, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
		SetDrawBright(255, 255, 255);
		DrawBox(movePanelPos.x, movePanelPos.y,
			movePanelPos.x + chipSize.x + 1, movePanelPos.y + chipSize.y + 1, 0xffff00, false);

	}

	// 見本表示
	DrawRotaGraph(SCREEN_SIZE_X - (SCREEN_SIZE_X - PUZZLE_SIZE_X) / 2,
		SCREEN_SIZE_Y / 2, 0.5, 0, puzzleImg[pzlImageID[PZL_SEL_NOW]], true, false);
	DrawFormatString(PUZZLE_SIZE_X, 0, 0xffffff, "blankID:%d", blankID);

	// 手数表示
	for (int i = 0; i < SCR_MAX; i++)
	{
		DrawFormatString(PUZZLE_SIZE_X, 32 + 16 * i, 0xffffff, "%d : %d",i + 1, minClickCount[i]);
	}
	DrawFormatString(PUZZLE_SIZE_X, 16, 0xffffff, "Cnt : %d", clickCnt);

	ScreenFlip();
}

// 分割線描画
void DrawDivLine(void)
{
	for (int i = 0; i < divTable[divID] + 1; i++)
	{
		int lineX = PUZZLE_SIZE_X / divTable[divID] * i;
		if (i == divTable[divID])
		{
			lineX--;
		}
		DrawLine(lineX, 0, lineX, PUZZLE_SIZE_Y, 0xffff00);
	}

	for (int i = 0; i < divTable[divID] + 1; i++)
	{
		int lineY = PUZZLE_SIZE_Y / divTable[divID] * i;
		if (i == divTable[divID])
		{
			lineY--;
		}
		DrawLine(0, lineY, PUZZLE_SIZE_X, lineY, 0xffff00);
	}
}

// ﾊﾟﾈﾙ入れ替え制御
void PanelCtl(void)
{
	if (MovePanel())
	{
		// 左ｸﾘｯｸされたときかﾘﾌﾟﾚｲﾓｰﾄﾞのとき
		if (((mouseF.trg[TRG_NOW] ^ mouseF.trg[TRG_OLD]) & (mouseF.trg[TRG_NOW] & MOUSE_INPUT_LEFT) || replayFlag) && !autoFlag)
		{
			if (!replayFlag)
			{
				SetMovePanel(mouseChip, DIR_LEFT);
			}
			else
			{
				SetMovePanel(ptrReplay->pos, DIR_LEFT);
			}		
		}
	}
}

// ﾊﾟﾈﾙの動きたい方向が抜かれているか調べる
void SetMovePanel(Pos panel, DIR moveDir)
{
	if (moveDir >= DIR_MAX)
	{
		return;
	}

	if (panel.x < 0 || panel.x >= divTable[divID] || panel.y < 0 || panel.y >= divTable[divID])
	{
		// ﾊﾟﾈﾙの上じゃなかったら判定中止
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	Pos nextPanel = panel;
	
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPanel.x--;
		break;

	case DIR_RIGHT:
		nextPanel.x++;
		break;

	case DIR_UP:
		nextPanel.y--;
		break;

	case DIR_DOWN:
		nextPanel.y++;
		break;

	default:
		AST();
		break;
	}

	if (nextPanel.x < 0 || nextPanel.x >= divTable[divID] || nextPanel.y < 0 || nextPanel.y >= divTable[divID])
	{
		// 動く先が範囲外だったら判定中止
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	if (pzData[nextPanel.y][nextPanel.x] != blankID)
	{
		SetMovePanel(panel, (DIR)((int)moveDir + 1));
		return;
	}

	moveID = pzData[panel.y][panel.x];
	movePanelPos.x = panel.x * chipSize.x;
	movePanelPos.y = panel.y * chipSize.y;
	moveEndPos.x = nextPanel.x * chipSize.x;
	moveEndPos.y = nextPanel.y * chipSize.y;

	if (!replayFlag)
	{
		clickCnt++;
		if (ptrReplay == NULL)
		{
			ptrReplay = (ReplayData*)malloc(sizeof(ReplayData));
			ReplayList = ptrReplay;
		}
		else
		{
			ptrReplay->next = (ReplayData*)malloc(sizeof(ReplayData));
			ptrReplay = ptrReplay->next;
		}
		ptrReplay->next = NULL;
		ptrReplay->pos.x = panel.x;
		ptrReplay->pos.y = panel.y;
	}
	else
	{
		if (ptrReplay->next != NULL)
		{
			ptrReplay = ptrReplay->next;
		}
	}

	SwapNum(&pzData[nextPanel.y][nextPanel.x], &pzData[panel.y][panel.x]);
	blank.x = panel.x;
	blank.y = panel.y;
}

// ﾊﾟﾈﾙ入れ替え処理
void SwapNum(int *panelID, int *nextPanelID)
{
	int tmpID = *panelID;
	*panelID = *nextPanelID;
	*nextPanelID = tmpID;
}

// ﾊﾟﾈﾙ移動処理
bool MovePanel(void)
{
	if (moveID == NON_ID)
	{
		return true;
	}

	if (moveEndPos.x - movePanelPos.x != 0)
	{
		if (moveEndPos.x - movePanelPos.x > 0)
		{
			movePanelPos.x += moveSpeed;
		}
		else
		{
			movePanelPos.x -= moveSpeed;
		}
	}
	else if (moveEndPos.y - movePanelPos.y != 0)
	{
		if (moveEndPos.y - movePanelPos.y > 0)
		{
			movePanelPos.y += moveSpeed;
		}
		else
		{
			movePanelPos.y -= moveSpeed;
		}
	}
	else
	{
		// 移動終了
		moveID = NON_ID;
		blankAnimCount = 0;

		return true;
	}

	return false;
}

// ﾊﾟﾈﾙｼｬｯﾌﾙ処理
void PanelShuffle(void)
{
	Pos blankPanel;					// 抜きﾊﾟﾈﾙの座標
	bool getBlankFlag = false;		// 抜きﾊﾟﾈﾙを取得したか
	DIR swapDir;					// 交換する方向
	bool getDirFlag = false;		// 交換できる方向を見つけたか

	// 抜きﾊﾟﾈﾙの座標を取得
	for (int y = 0; y < divTable[divID]; y++)
	{
		if (getBlankFlag)
		{
			break;
		}

		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPanel = { x,y };
				getBlankFlag = true;
				break;
			}
		}
	}

	if (!getBlankFlag)
	{
		AST();
		return;
	}

	Pos nextPanel = blankPanel;
	int shuffleCnt = 10000;

	for (int i = 0; i < shuffleCnt; i++)
	{
		// 交換方向決定
		while (!getDirFlag)
		{
			swapDir = (DIR)(rand() % (int)DIR_MAX);

			switch (swapDir)
			{
			case DIR_LEFT:
				if ((nextPanel.x - 1) >= 0)
				{
					nextPanel.x--;
					getDirFlag = true;
				}
				break;

			case DIR_RIGHT:
				if ((nextPanel.x + 1) < divTable[divID])
				{
					nextPanel.x++;
					getDirFlag = true;
				}
				break;

			case DIR_UP:
				if ((nextPanel.y - 1) >= 0)
				{
					nextPanel.y--;
					getDirFlag = true;
				}
				break;

			case DIR_DOWN:
				if ((nextPanel.y + 1) < divTable[divID])
				{
					nextPanel.y++;
					getDirFlag = true;
				}
				break;

			default:
				AST();
				break;
			}
		}

		SwapNum(&pzData[nextPanel.y][nextPanel.x], &pzData[blankPanel.y][blankPanel.x]);

		blankPanel = nextPanel;
		getDirFlag = false;
	}

	if (isClear())
	{
		// すでにｸﾘｱしてたら再帰
		PanelShuffle();
	}
}

// ﾒﾓﾘ開放
void FreeMemData()
{
	// ﾊﾟﾈﾙ
	if (pzData != NULL)
	{
		free(pzData[0]);
	}
	free(pzData);

	pzData = NULL;

	// ﾘｽﾄ
	ptrReplay = ReplayList;
	if (ptrReplay != NULL)
	{
		ReplayData* nextData;		// 解放する次のﾎﾟｲﾝﾀを記憶
		do
		{
			nextData = ptrReplay->next;
			free(ptrReplay);
			ptrReplay = nextData;
		} while (nextData != NULL);
	}

	ReplayList = NULL;
	ptrReplay = NULL;
}

// ｸﾘｱﾁｪｯｸ
bool isClear(void)
{
	for (int y = 0; y < divTable[divID]; y++)
	{
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] != (y * divTable[divID] + x))
			{
				return false;
			}
		}
	}

	return true;
}

// ================================ここから下が自動クリア関係、めっちゃ汚い==========================================

// 自動クリア
void AutoClear(void)
{
	if (autoID >= divTable[divID] * (divTable[divID] - 2) && !autoLastLine)
	{
		autoLastLine = true;
		LastLineMove();
		return;
	}

	lineNumY = autoID / divTable[divID];

	Pos autoPos = { divTable[divID],divTable[divID] };		// autoIDの場所,初期値は分割数に設定

	// autoIDを探す,
	for (int y = (autoID / divTable[divID]); y < divTable[divID]; y++)
	{
		if (autoPos.x != divTable[divID])
		{
			// 座標が変わってたら探索終了
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == autoID)
			{
				autoPos = { x,y };
				break;
			}
		}
	}

	if (autoPos.x == divTable[divID])
	{
		AST();
	}

	// 目的地設定
	Pos desPos = { autoID % divTable[divID] ,autoID / divTable[divID] };
	if (autoID % divTable[divID] == divTable[divID] - 2)
	{
		desPos.x += 1;
		if (pzData[desPos.y][desPos.x] == autoID + 1 && autoPos.x == desPos.x && (autoPos.y == desPos.y + 1))
		{
			// 目的地に右端のﾊﾟﾈﾙがあったままだったら無限ループになる
			jamaFlag = true;
			JamaMove();
			return;
		}
	}
	else if (autoID % divTable[divID] == divTable[divID] - 1)
	{
		desPos.y += 1;
	}


	// autoIDのﾊﾟﾈﾙを動かす向きを決める
	DIR moveDir;

	if (desPos.x - autoPos.x < 0)
	{
		moveDir = DIR_LEFT;
	}
	else if (desPos.x - autoPos.x > 0)
	{
		moveDir = DIR_RIGHT;
	}
	else if (desPos.y - autoPos.y < 0)
	{
		moveDir = DIR_UP;
	}
	else if (desPos.y - autoPos.y > 0)
	{
		moveDir = DIR_DOWN;
	}
	else
	{
		// もう一致している状態
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			// 列の最後じゃなかったら次のﾊﾟﾈﾙへ
			autoID++;
		}
		else
		{
			// 列の最後だったらその列をきれいに揃える
			xLineLast = true;
			autoID--;
		}
		return;
	}

	// 動かしたい向きに動かせるかﾁｪｯｸ(移動先がblankIDか)

	Pos nextPos;		// 動かしたい場所
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPos = { autoPos.x - 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_LEFT);
			return;
		}
		break;

	case DIR_UP:
		nextPos = { autoPos.x, autoPos.y - 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_UP);
			return;
		}
		break;

	case DIR_RIGHT:
		nextPos = { autoPos.x + 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_RIGHT);
			return;
		}
		break;

	case DIR_DOWN:
		nextPos = { autoPos.x, autoPos.y + 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_DOWN);
			return;
		}
		break;

	default:
		AST();
		break;
	}

	// 動かしたい方向にblankIDがなかった場合
	// blankIDを探す
	Pos blankPos = { divTable[divID],divTable[divID] };
	DIR moveDir2;

	for (int y = (autoID / divTable[divID]); y < divTable[divID]; y++)
	{
		if (blankPos.x != divTable[divID])
		{
			// 座標が変わってたら探索終了
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPos = { x,y };
				break;
			}
		}
	}

	blank = blankPos;

	// blankIDのﾊﾟﾈﾙを動かしたい方向を決めて動かす

	/*
	 * 　7/22(月)メモ
	 * 〇次にやること
	 *  ・抜きﾊﾟﾈﾙを動かすときにautoIDのﾊﾟﾈﾙを動かさないようにする
	 *　　→移動先がautoIDだったら特別な移動ﾓｰﾄﾞに移行
	 *　・上記ができたら最初の一個がちゃんと動くかﾃｽﾄ
	 *　・列の最後の二つをそろえるのに特別な移動法を作る
	 *　　→最後の二つは目的地をずらせばいいのでは？
	 */

	if (nextPos.x - blankPos.x < 0)
	{
		// blankIDのﾊﾟﾈﾙの左にあるﾊﾟﾈﾙをうごかす
		blankPos.x -= 1;
		blankMoveDir = DIR_LEFT;
		moveDir2 = DIR_RIGHT;
	}
	else if (nextPos.x - blankPos.x > 0)
	{
		blankPos.x += 1;
		blankMoveDir = DIR_RIGHT;
		moveDir2 = DIR_LEFT;
	}
	else if (nextPos.y - blankPos.y < 0)
	{
		blankPos.y -= 1;
		blankMoveDir = DIR_UP;
		moveDir2 = DIR_DOWN;
	}
	else if (nextPos.y - blankPos.y > 0)
	{
		blankPos.y += 1;
		blankMoveDir = DIR_DOWN;
		moveDir2 = DIR_UP;
	}
	else
	{
		// もう一致している状態、こないはず
		AST();
		return;
	}

	// 範囲ﾁｪｯｸ
	if (blankPos.x >= divTable[divID] || blankPos.y >= divTable[divID])
	{
		AST();
		return;
	}

	// 移動先がautoID,もしくはすでにそろえたﾊﾟﾈﾙかをﾁｪｯｸ、autoIDだったらblankMove
	if (pzData[blankPos.y][blankPos.x] > autoID)
	{
		SetMovePanel(blankPos, moveDir2);
		if (pzData[desPos.y][desPos.x] == autoID)
		{
			//目的地に到着してたらautoIDを進める
			if (autoID % divTable[divID] != divTable[divID] - 1)
			{
				// 列の最後じゃなかったら次のﾊﾟﾈﾙへ
				if (!autoLastLine)
				{
					autoID++;
				}
				else if (lineNumY == divTable[divID] - 2)
				{
					autoID += divTable[divID];
				}
				else
				{
					// 縦の二個ずつを移動させる
					yLineLast = true;
				}
			}
			else
			{
				// 列の最後だったらその列をきれいに揃える
				xLineLast = true;
				lineNumY = autoID / divTable[divID];
				autoID--;
			}
		}
	}
	else if (pzData[blankPos.y][blankPos.x] == autoID && moveDir == DIR_UP)
	{
		if (pzData[blank.y - 1][blank.x] > autoID)
		{
			blankPos = blank;
			blankPos.y -= 1;
			SetMovePanel(blankPos, DIR_DOWN);
		}
		else
		{
			blankMove = true;
			blankMoveCnt = 0;
		}
	}
	else if (pzData[blankPos.y][blankPos.x] != autoID && moveDir2 == DIR_RIGHT)
	{
		blankPos = blank;
		blankPos.y += 1;
		SetMovePanel(blankPos, DIR_UP);
	}
	else
	{
		blankMove = true;
		blankMoveCnt = 0;
	}
}

// 抜きﾊﾟﾈﾙをautoIDのﾊﾟﾈﾙを動かしたい方向にもっていく
void BlankMove(void)
{
 	Pos movePos = blank;
	DIR moveDir;
	if (blankMoveDir == DIR_LEFT || blankMoveDir == DIR_RIGHT)
	{
		switch (blankMoveCnt)
		{
		case 0:
			if (blank.y != divTable[divID] - 1)
			{
				movePos.y += 1;
				avoDir = DIR_DOWN;
				moveDir = DIR_UP;
			}
			else
			{
				movePos.y -= 1;
				avoDir = DIR_UP;
				moveDir = DIR_DOWN;
			}
			break;

		case 1:
		case 2:
			if (blankMoveDir == DIR_LEFT)
			{
				movePos.x -= 1;
				moveDir = DIR_RIGHT;
			}
			else
			{
				movePos.x += 1;
				moveDir = DIR_LEFT;
			}
			break;

		case 3:
			if (avoDir == DIR_DOWN)
			{
				movePos.y -= 1;
				moveDir = DIR_DOWN;
			}
			else
			{
				movePos.y += 1;
				moveDir = DIR_UP;
			}
			break;

		default:
			break;
		}
	}
	else
	{
		switch (blankMoveCnt)
		{
		case 0:
			if (blank.x != divTable[divID] - 1)
			{
				movePos.x += 1;
				avoDir = DIR_RIGHT;
				moveDir = DIR_LEFT;
			}
			else
			{
				movePos.x -= 1;
				avoDir = DIR_LEFT;
				moveDir = DIR_RIGHT;
			}
			break;

		case 1:
		case 2:
			if (blankMoveDir == DIR_UP)
			{
				movePos.y -= 1;
				moveDir = DIR_DOWN;
			}
			else
			{
				movePos.y += 1;
				moveDir = DIR_UP;
			}
			break;

		case 3:
			if (avoDir == DIR_RIGHT)
			{
				movePos.x -= 1;
				moveDir = DIR_RIGHT;
			}
			else
			{
				movePos.x += 1;
				moveDir = DIR_LEFT;
			}
			break;

		default:
			break;
		}
	}

	if (pzData[movePos.y][movePos.x] > autoID || autoLastLine)
	{
		SetMovePanel(movePos, moveDir);
		blank = movePos;
		blankMoveCnt++;
		if (blankMoveCnt >= 4)
		{
			blankMove = false;
		}
	}
	else
	{
		AST();
		return;
	}
}

// 横列の最後をそろえたい
void XLineLastMove(void)
{
	if (autoID % divTable[divID] == divTable[divID] - 1)
	{
		SetMovePanel({ divTable[divID] - 1 , lineNumY + 1 }, DIR_UP);
		xLineLast = false;
		autoID++;
		return;
	}
	else if (autoID % divTable[divID] == divTable[divID] - 2 && pzData[lineNumY][divTable[divID] - 2] == blankID)
	{
		SetMovePanel({ divTable[divID] - 1 , lineNumY}, DIR_LEFT);
		autoID++;
		return;
	}
	else
	{
		if (pzData[lineNumY + 1][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 , lineNumY }, DIR_DOWN);
			return;
		}
	}

	// blankIDをそろえたい列の最後の左下にもっていきたい
	if (pzData[lineNumY + 2][divTable[divID] - 2] == blankID)
	{
		SetMovePanel({ divTable[divID] - 2 , lineNumY  + 1}, DIR_DOWN);
	}
	else
	{
		SetMovePanel({ divTable[divID] - 2 , lineNumY + 2 }, DIR_RIGHT);
	}
}

// 列の右端のやつが邪魔
void JamaMove(void)
{
	if (!autoLastLine)
	{
		if (pzData[lineNumY + 1][divTable[divID] - 2] == blankID)
		{
			if (pzData[lineNumY + 1][divTable[divID] - 1] == autoID)
			{
				SetMovePanel({ divTable[divID] - 2 ,lineNumY + 2 }, DIR_UP);
			}
			else if (pzData[lineNumY + 1][divTable[divID] - 1] == autoID + 1)
			{
				SetMovePanel({ divTable[divID] - 1 ,lineNumY + 1 }, DIR_LEFT);
				jamaFlag = false;
			}
			else
			{
				AST();
			}
		}
		else if (pzData[lineNumY + 2][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY + 2 }, DIR_LEFT);
		}
		else if (pzData[lineNumY + 2][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY + 1 }, DIR_DOWN);
		}
		else if (pzData[lineNumY + 1][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 1 ,lineNumY }, DIR_DOWN);
		}
		else if (pzData[lineNumY][divTable[divID] - 1] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 ,lineNumY }, DIR_RIGHT);
		}
		else if (pzData[lineNumY][divTable[divID] - 2] == blankID)
		{
			SetMovePanel({ divTable[divID] - 2 ,lineNumY + 1 }, DIR_UP);
		}
		else
		{
			AST();
			lineNumY = autoID / divTable[divID];
			jamaFlag = false;
		}
	}
	else
	{
		if (pzData[divTable[divID] - 2][lineNumX + 1] == blankID)
		{
			if (pzData[divTable[divID] - 1][lineNumX + 1] == (autoID + divTable[divID]))
			{
				SetMovePanel({ lineNumX + 1,divTable[divID] - 1 }, DIR_LEFT);
				jamaFlag = false;
			}
			else
			{
				SetMovePanel({ lineNumX + 2,divTable[divID] - 2 }, DIR_LEFT);
			}
		}
		else if (pzData[divTable[divID] - 2][lineNumX + 2] == blankID)
		{
			SetMovePanel({ lineNumX + 2,divTable[divID] - 1}, DIR_UP);
		}
		else if (pzData[divTable[divID] - 1][lineNumX + 2] == blankID)
		{
			SetMovePanel({ lineNumX + 1,divTable[divID] - 1 }, DIR_RIGHT);
		}
		else if (pzData[divTable[divID] - 1][lineNumX + 1] == blankID)
		{
			SetMovePanel({ lineNumX ,divTable[divID] - 1 }, DIR_RIGHT);
		}
		else if (pzData[divTable[divID] - 1][lineNumX] == blankID)
		{
			SetMovePanel({ lineNumX ,divTable[divID] - 2 }, DIR_DOWN);
		}
		else if (pzData[divTable[divID] - 2][lineNumX] == blankID)
		{
			SetMovePanel({ lineNumX  + 1,divTable[divID] - 2 }, DIR_LEFT);
		}
		else
		{
			AST();
			jamaFlag = false;
		}
	}
}

// 縦列の最後を揃えたい
void YLineLastMove(void)
{
	if (pzData[divTable[divID] - 1][lineNumX + 2] == blankID)
	{
		SetMovePanel({ lineNumX + 2 , divTable[divID] - 2 }, DIR_DOWN);
	}
	else if (pzData[divTable[divID] - 2][lineNumX + 2] == blankID)
	{
		SetMovePanel({ lineNumX + 1 , divTable[divID] - 2 }, DIR_RIGHT);
	}
	else if (pzData[divTable[divID] - 2][lineNumX + 1] == blankID)
	{
		SetMovePanel({ lineNumX, divTable[divID] - 2 }, DIR_RIGHT);
	}
	else if (pzData[divTable[divID] - 2][lineNumX] == blankID)
	{
		SetMovePanel({ lineNumX, divTable[divID] - 1 }, DIR_UP);
	}
	else if (pzData[divTable[divID] - 1][lineNumX] == blankID)
	{
		// ここでおわり
		SetMovePanel({ lineNumX + 1, divTable[divID] - 1}, DIR_LEFT);
		autoID = (divTable[divID] - 2) * divTable[divID] + (++lineNumX);
		if (lineNumX >= divTable[divID] - 2)
		{
			autoFlag = false;
			cycleFlag = true;
		}
		yLineLast = false;
	}
	else
	{
		AST();
	}
}

// 最後の列を揃えたい
void LastLineMove(void)
{
	Pos autoPos = { divTable[divID],divTable[divID] };		// autoIDの場所,初期値は分割数に設定

	// autoIDを探す,
	for (int y = divTable[divID] - 2; y < divTable[divID]; y++)
	{
		if (autoPos.x != divTable[divID])
		{
			// 座標が変わってたら探索終了
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == autoID)
			{
				autoPos = { x,y };
				break;
			}
		}
	}

	if (autoPos.x == divTable[divID])
	{
		AST();
		autoFlag = false;
		autoLastLine = false;
		return;
	}

	lineNumY = autoID / divTable[divID];

	// 目的地設定
	Pos desPos = { autoID % divTable[divID] ,autoID / divTable[divID] };

	if (lineNumY == divTable[divID] - 2)
	{
		if (pzData[desPos.y][desPos.x] == autoID)
		{
			// 本当の目的地に置いてあったら回転
			cycleFlag = true;
			CycleMove();

			return;
		}
		desPos.y += 1;
		if ((pzData[desPos.y][desPos.x] == autoID + divTable[divID] &&
			autoPos.x == desPos.x + 1 && autoPos.y == desPos.y))
		{
			// 目的地に右端のﾊﾟﾈﾙがあったままだったら無限ループになる
			jamaFlag = true;
			JamaMove();
			return;
		}
	}
	else
	{
		desPos.x += 1;
	}

	// autoIDのﾊﾟﾈﾙを動かす向きを決める
	DIR moveDir;

	if (desPos.y - autoPos.y > 0)
	{
		moveDir = DIR_DOWN;
	}
	else if (desPos.x - autoPos.x < 0)
	{
		moveDir = DIR_LEFT;
	}
	else
	{
		// もう一致している状態
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			if (lineNumY == divTable[divID] - 2)
			{
				autoID += divTable[divID];
			}
			else
			{
				// 縦の二個ずつを移動させる
				yLineLast = true;
			}
		}
		return;
	}

	// 動かしたい向きに動かせるかﾁｪｯｸ(移動先がblankIDか)

	Pos nextPos;		// 動かしたい場所
	switch (moveDir)
	{
	case DIR_LEFT:
		nextPos = { autoPos.x - 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_LEFT);
			return;
		}
		break;

	case DIR_UP:
		nextPos = { autoPos.x, autoPos.y - 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_UP);
			return;
		}
		break;

	case DIR_RIGHT:
		nextPos = { autoPos.x + 1 , autoPos.y };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_RIGHT);
			return;
		}
		break;

	case DIR_DOWN:
		nextPos = { autoPos.x, autoPos.y + 1 };
		if (pzData[nextPos.y][nextPos.x] == blankID)
		{
			SetMovePanel(autoPos, DIR_DOWN);
			return;
		}
		break;

	default:
		AST();
		break;
	}

	// 動かしたい方向にblankIDがなかった場合
	// blankIDを探す
	Pos blankPos = { divTable[divID],divTable[divID] };
	DIR moveDir2;

	for (int y = divTable[divID] - 2; y < divTable[divID]; y++)
	{
		if (blankPos.x != divTable[divID])
		{
			// 座標が変わってたら探索終了
			break;
		}
		for (int x = 0; x < divTable[divID]; x++)
		{
			if (pzData[y][x] == blankID)
			{
				blankPos = { x,y };
				break;
			}
		}
	}

	blank = blankPos;

	// blankIDのﾊﾟﾈﾙを動かしたい方向を決めて動かす
	// まずblankが上の段にあったら下に下げる
	if (blank.y == divTable[divID] - 2 && pzData[blank.y + 1][blank.x] != autoID)
	{
		SetMovePanel({ blank.x,blank.y + 1 }, DIR_UP);
		return;
	}
	else if (nextPos.x - blankPos.x < 0)
	{
		// blankIDのﾊﾟﾈﾙの左にあるﾊﾟﾈﾙをうごかす
		blankPos.x -= 1;
		blankMoveDir = DIR_LEFT;
		moveDir2 = DIR_RIGHT;
	}
	else if (nextPos.x - blankPos.x > 0)
	{
		blankPos.x += 1;
		blankMoveDir = DIR_RIGHT;
		moveDir2 = DIR_LEFT;
	}
	else
	{
		// もう一致している状態、こないはず
		AST();
		return;
	}

	// 範囲ﾁｪｯｸ
	if (blankPos.x >= divTable[divID] || blankPos.y >= divTable[divID])
	{
		AST();
		return;
	}

	// 移動先がautoID,もしくはすでにそろえたﾊﾟﾈﾙかをﾁｪｯｸ、autoIDだったらblankMove
	if (pzData[blankPos.y][blankPos.x] != autoID)
	{
		SetMovePanel(blankPos, moveDir2);
		
	}
	else if (pzData[desPos.y][desPos.x] == autoID)
	{
		//目的地に到着してたらautoIDを進める
		if (autoID % divTable[divID] != divTable[divID] - 1)
		{
			if (lineNumY == divTable[divID] - 2)
			{
				autoID += divTable[divID];
			}
			else
			{
				// 縦の二個ずつを移動させる
				yLineLast = true;
			}
		}
	}
	else
	{
		blankMove = true;
		blankMoveCnt = 0;
	}
}

// 回転ﾓｰﾄﾞ
void CycleMove(void)
{
	if (blank.x == lineNumX)
	{
		if (blank.y == divTable[divID] - 1)
		{
			SetMovePanel({ blank.x + 1,blank.y }, DIR_LEFT);

			// ﾁｪｯｸ
			if (pzData[blank.y][blank.x - 1] == autoID)
			{
				cycleFlag = false;
				autoID += divTable[divID];
			}
		}
		else
		{
			SetMovePanel({ blank.x,blank.y + 1}, DIR_UP);
		}
	}
	else
	{
		if (blank.y == divTable[divID] - 1)
		{
			SetMovePanel({ blank.x,blank.y - 1}, DIR_DOWN);
		}
		else
		{
			SetMovePanel({ blank.x - 1,blank.y }, DIR_RIGHT);
		}
	}
}