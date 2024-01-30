//*****************************************************
//
// ボス敵状態[stateEnemyBoss.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "stateEnemyBoss.h"
#include "enemyBoss.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_MISSILE = 5.0f;	// ミサイル発射の時間
}

//=====================================================
// コンストラクタ
//=====================================================
CStateBoss::CStateBoss()
{

}

//=====================================================
// 出現状態
//=====================================================
void CStateBossApper::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_APPER);
}

void CStateBossApper::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// モーション終了で次の状態へ移る
		pBoss->ChangeState(new CStateBossAttackBeam);
	}
}

//=====================================================
// ビーム攻撃
//=====================================================
void CStateBossAttackBeam::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SHOT_UNDER);
}

void CStateBossAttackBeam::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	pBoss->AimPlayer();

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// モーション終了で次の状態へ移る
		pBoss->ChangeState(new CStateBossAttackBeam);
	}
}

//=====================================================
// ミサイル攻撃
//=====================================================
void CStateBossAttackMissile::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nTimerMissile = 0.0f;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMissile::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);
}