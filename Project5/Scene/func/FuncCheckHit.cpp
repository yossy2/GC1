#include "FuncCheckHit.h"

// 当たり判定関数オブジェクト 
// 現状弾の判定のみ、今後敵と自機の判定が追加される可能性あり
bool FuncCheckHit::operator()(ActQueT & que, std::vector<sharedObj>& objList)
{
	// 当たり判定の対象となるオブジェクト
	UNIT_ID targetID = que.second.unitID() == UNIT_ID::PLAYER_BULLET ?
					   UNIT_ID::ENEMY :
					   UNIT_ID::PLAYER;	

	for (auto obj : objList)
	{
		if ((*obj).unitID() == targetID)
		{
			// 当たり判定
			if (abs(que.second.pos().y - (*obj).pos().y) < static_cast<double>((que.second.size().x + (*obj).size().y) / 2)
				&& abs(que.second.pos().x - (*obj).pos().x) < static_cast<double>((que.second.size().y + (*obj).size().x) / 2))
			{
				// queのサイズが逆になっているのは弾の向きが90度回転するから。わかりにくいので後でどうにかする
				(*obj).setAlive(false);
				que.second.setAlive(false);
				return true;
			}
		}
	}

	return false;
}
