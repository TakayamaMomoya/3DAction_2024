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
#include "enemyDrone.h"
#include "manager.h"
#include "missile.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.25f;	// ミサイル発射の時間
const int NUM_MISSILE = 10;	// ミサイルの発射数
const float TIME_DRONE = 1.0f;	// ドローン発射の時間
const int NUM_DRONE = 10;	// ドローンの発射数
const int RANGE_HEIGHT_DRONE = 200;	// ドローンの高さの幅
const float MOVE_DRONE = 20.0f;	// ドローンの射出時の移動量
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
		pBoss->ChangeState(new CStateBossLaunchDrone);
	}
}

//=====================================================
// ミサイル攻撃
//=====================================================
void CStateBossAttackMissile::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMissile::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	if (pBoss->AttackTimer(TIME_MISSILE))
	{// ミサイルの発射
		CMissile *pMissile = CMissile::Create(pBoss->GetPosition());

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRot();

			rot.x = -D3DX_PI * 0.5f;

			pMissile->SetRot(rot);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossLaunchDrone);
		}
	}

	// 後退処理
	pBoss->Back();
}

//=====================================================
// 子機の射出
//=====================================================
void CStateBossLaunchDrone::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossLaunchDrone::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	if (pBoss->AttackTimer(TIME_DRONE))
	{// 射出する
		CEnemyDrone *pDrone = new CEnemyDrone;

		if (pDrone != nullptr)
		{
			pDrone->Init();

			D3DXVECTOR3 pos = pBoss->GetPosition();

			pDrone->SetPosition(pos);

			pos.y += universal::RandRange(RANGE_HEIGHT_DRONE, -RANGE_HEIGHT_DRONE);

			// 射出してからの位置設定
			pDrone->SetPositionDest(pos);

			// 移動量を足す
			D3DXVECTOR3 move = { 0.0f,MOVE_DRONE,0.0f };
			pDrone->SetMove(move);
		}

		m_nCnt++;

		if (m_nCnt > NUM_DRONE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackBeam);
		}
	}

	// 後退処理
	pBoss->Back();
}