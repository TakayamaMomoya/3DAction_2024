//*****************************************************
//
// �{�X�G���[stateEnemyBoss.h]
// Author:���R����
//
//*****************************************************

#ifndef _STATEENEMYBOSS_H_
#define _STATEENEMYBOSS_H_

#include <assert.h>

class CEnemyBoss;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CStateBoss
{
public:
	CStateBoss();

	virtual void Init(CEnemyBoss *pBoss) {}
	virtual void Attack(CEnemyBoss *pBoss) {}
	virtual void Move(CEnemyBoss *pBoss) {}

protected:
	void CheckPointer(CEnemyBoss *pBoss) {
		if(pBoss == nullptr)assert(("�{�X���̎Q�Ƃ��ł��܂���", false));
	};

private:
};

class CStateBossApper : public CStateBoss
{// �o�����
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAttackBeam : public CStateBoss
{// �r�[���U��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;
};

class CStateBossAttackMissile : public CStateBoss
{// �~�T�C���U��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �~�T�C���J�E���^�[
};

class CStateBossLaunchDrone : public CStateBoss
{// �q�@�̎ˏo
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �ˏo�J�E���^�[
};

class CStateBossAttackMachinegun : public CStateBoss
{// �}�V���K��
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	int m_nCnt; // �ˏo�J�E���^�[
};
#endif