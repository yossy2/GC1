#include "FuncCheckHit.h"

// �����蔻��֐��I�u�W�F�N�g 
// ����e�̔���̂݁A����G�Ǝ��@�̔��肪�ǉ������\������
bool FuncCheckHit::operator()(ActQueT & que, std::vector<sharedObj>& objList)
{
	// �����蔻��̑ΏۂƂȂ�I�u�W�F�N�g
	UNIT_ID targetID = que.second.unitID() == UNIT_ID::PLAYER_BULLET ?
					   UNIT_ID::ENEMY :
					   UNIT_ID::PLAYER;	

	for (auto obj : objList)
	{
		if ((*obj).unitID() == targetID)
		{
			// �����蔻��
			if (abs(que.second.pos().y - (*obj).pos().y) < static_cast<double>((que.second.size().x + (*obj).size().y) / 2)
				&& abs(que.second.pos().x - (*obj).pos().x) < static_cast<double>((que.second.size().y + (*obj).size().x) / 2))
			{
				// que�̃T�C�Y���t�ɂȂ��Ă���̂͒e�̌�����90�x��]���邩��B�킩��ɂ����̂Ō�łǂ��ɂ�����
				(*obj).setAlive(false);
				que.second.setAlive(false);
				return true;
			}
		}
	}

	return false;
}
