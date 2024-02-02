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
#include "bullet.h"
#include "player.h"
#include "animEffect3D.h"
#include "fade.h"
#include "explosionspawner.h"
#include "particle.h"
#include "anim3D.h"
#include "camera.h"
#include "meshfield.h"
#include "blockManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.5f;	// ミサイル発射の時間
const int NUM_MISSILE = 10;	// ミサイルの発射数
const float TIME_DRONE = 1.0f;	// ドローン発射の時間
const int NUM_DRONE = 3;	// ドローンの発射数
const int RANGE_HEIGHT_DRONE = 500;	// ドローンの高さの幅
const float MOVE_DRONE = 50.0f;	// ドローンの射出時の移動量
const float TIME_MG = 0.15f;	// マシンガン発射の時間
const int NUM_MG = 30;	// マシンガンの発射数
const float SPEED_BULLET = 200.0f;	// マシンガン弾の速度
const int ACCURACY_MG = 10;	// マシンガンの精度
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
		pBoss->ChangeState(new CStateBossAttackMachinegun);
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
		CMissile *pMissile = CMissile::Create(pBoss->GetMtxPos(1));

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRot();


			if (m_nCnt % 2 == 0)
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = D3DX_PI * 0.5f;
			}
			else
			{
				rot.x = -D3DX_PI * 0.3f;
				rot.y = -D3DX_PI * 0.3f;
			}

			pMissile->SetRot(rot);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackMachinegun);
		}
	}

	// 後退処理
	pBoss->AimPlayer();
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

			pos.y += universal::RandRange(0, -RANGE_HEIGHT_DRONE);

			// 射出してからの位置設定
			pDrone->SetPositionDest(pos);

			// 移動量を足す
			D3DXVECTOR3 move = { 0.0f,MOVE_DRONE,0.0f };
			pDrone->SetMove(move);
		}

		m_nCnt++;

		if (m_nCnt > NUM_DRONE)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackMissile);
		}
	}

	// 後退処理
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// マシンガン
//=====================================================
void CStateBossAttackMachinegun::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_nCnt = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMachinegun::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	if (pBoss->AttackTimer(TIME_MG))
	{// 射出する
		D3DXVECTOR3 posMazzle = pBoss->GetMtxPos(5);
		D3DXVECTOR3 moveBullet;
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// 偏差の計算
		D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

		posPrediction.x += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.y += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.z += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);

		D3DXVECTOR3 vecDiffBullet = posMazzle - posPrediction;
		D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

		moveBullet = vecDiffBullet * SPEED_BULLET;

		// 弾の発射
		CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.01f);

		// アニメエフェクト生成
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			pAnim3D->CreateEffect(posMazzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MG)
		{// 一定数撃ったら次の行動へ
			pBoss->ChangeState(new CStateBossAttackBeam);
		}
	}

	// 後退処理
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// 第二形態へ移行する
//=====================================================
void CStateBossTrans::Init(CEnemyBoss *pBoss)
{
	m_fTimeTrans = 0.0f;

	CheckPointer(pBoss);

}

void CStateBossTrans::Move(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_fTimeTrans += CManager::GetDeltaTime();

	CFade *pFade = CFade::GetInstance();

	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (pFade != nullptr)
	{
		CFade::FADE state = pFade->GetState();

		if (state == CFade::FADE::FADE_NONE)
		{
			if (m_fTimeTrans > 6.0f)
			{// フェードで第二形態に移る
				Evolve(pBoss);
			}
		}

		if (state == CFade::FADE::FADE_OUT)
		{
			if (nMotion != CEnemyBoss::MOTION::MOTION_DEATH)
			{// ムービーにいく
				TransMovie(pBoss);
			}

			if (m_fTimeTrans > 6.0f)
			{// 地形の変動
				// ブロック削除
				CBlockManager *pBlockManager = CBlockManager::GetInstance();

				if (pBlockManager != nullptr)
					pBlockManager->DeleteAll();

				// メッシュフィールド変形


				pBoss->ChangeState(new CStateBossBeforeTrans);
			}
		}
	}
}

void CStateBossTrans::TransMovie(CEnemyBoss *pBoss)
{// ムービーに移行
	D3DXVECTOR3 pos = { 7000.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,D3DX_PI * 0.5f,0.0f };

	// ボス位置を設定
	pBoss->SetPosition(pos);
	pBoss->SetRot(rot);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// プレイヤー位置を設定
		D3DXVECTOR3 posPlayer = { 5000.0f,0.0f,0.0f };
		D3DXVECTOR3 rotPlayer = { 0.0f,0.0f,0.0f };

		pPlayer->SetPosition(posPlayer);
		pPlayer->SetRot(rotPlayer);
	}

	// 爆発エフェクト
	pos.y += 200.0f;
	CExplSpawner::Create(pos, 300.0f, 360);
	CParticle::Create(pos, CParticle::TYPE::TYPE_TURN_EXPLOSION);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);
}

void CStateBossTrans::Evolve(CEnemyBoss *pBoss)
{// 第二形態に進化
	// 大爆発を起こす
	CAnimEffect3D *pAnim = CAnimEffect3D::GetInstance();

	if (pAnim != nullptr)
	{
		D3DXVECTOR3 pos = pBoss->GetMtxPos(0);

		CAnim3D *pExplosion;

		pExplosion = pAnim->CreateEffect(pos, CAnimEffect3D::TYPE_EXPLOSION);

		if (pExplosion != nullptr)
		{
			pExplosion->SetSize(2000.0f, 2000.0f);
		}
	}

	// カメラを揺らす
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetQuake(0.2f, 0.2f, 120);
	}

	// 白いフェードをかける
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->SetFade(CScene::MODE_GAME, false);
	}
}

//=====================================================
// 第二形態に移ったときの処理
//=====================================================
void CStateBossBeforeTrans::Init(CEnemyBoss *pBoss)
{

}

void CStateBossBeforeTrans::Move(CEnemyBoss *pBoss)
{

}