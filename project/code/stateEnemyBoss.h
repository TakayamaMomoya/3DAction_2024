//*****************************************************
//
// ボス敵状態[stateEnemyBoss.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _STATEENEMYBOSS_H_
#define _STATEENEMYBOSS_H_

#include <assert.h>

class CEnemyBoss;

//*****************************************************
// クラスの定義
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
		if(pBoss == nullptr)assert(("ボス情報の参照ができません", false));
	};

private:
};

class CStateBossApper : public CStateBoss
{// 出現状態
	void Init(CEnemyBoss *pBoss) override;
	void Move(CEnemyBoss *pBoss) override;
};

class CStateBossAttackBeam : public CStateBoss
{// ビーム攻撃
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;
};

class CStateBossAttackMissile : public CStateBoss
{// ミサイル攻撃
	void Init(CEnemyBoss *pBoss) override;
	void Attack(CEnemyBoss *pBoss) override;

	// ミサイルタイマー
	int m_nTimerMissile;
};

#endif