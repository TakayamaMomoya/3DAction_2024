//*****************************************************
//
// 通常敵の処理[enemyNormal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "enemyNormal.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DIST_FIRE = 500.0f;	// 発射するまでの視線とプレイヤーの差分距離
const float SPEED_BULLET = 70.0f;	// 弾の速度
const float TIME_FIRE = 1.0f;	// 発射間隔
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyNormal::CEnemyNormal()
{
	m_fTimeFire = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyNormal::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// 死亡モーション
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}

	CPlayer *pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 rot = GetRot();

	if (pPlayer != nullptr)
	{
		// プレイヤーの方を向く
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// 差分角度の取得
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.x, -rotDest.x + D3DX_PI * 0.5f, 0.1f);
		universal::FactingRot(&rot.y, rotDest.y, 0.1f);

		SetRot(rot);

		// プレイヤーに向かって弾を撃つ
		float fDist = D3DXVec3Length(&vecDiff);

		D3DXMATRIX mtxEye;
		D3DXMATRIX mtx = *GetMatrix();

		universal::SetOffSet(&mtxEye, mtx,D3DXVECTOR3(0.0f,0.0f, fDist));

		D3DXVECTOR3 vecEye = { mtxEye._41,mtxEye._42 ,mtxEye._43 };

		D3DXVECTOR3 vecDiffEye = posPlayer - vecEye;

		float fDistEyeToPlayer = D3DXVec3Length(&vecDiffEye);

		// 狙いが近かったら発射
		if (fDistEyeToPlayer < DIST_FIRE)
		{
			float fDeltaTime = CManager::GetDeltaTime();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{// タイムスケールをかける
				float fScale = pSlow->GetScale();

				fDeltaTime *= fScale;
			}

			// タイマー加算
			m_fTimeFire += fDeltaTime;

			if (m_fTimeFire > TIME_FIRE)
			{// 弾の発射
				D3DXVECTOR3 posMazzle = GetMtxPos(0);
				D3DXVECTOR3 moveBullet;
				D3DXVECTOR3 movePlayer = pPlayer->GetMove();

				D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);
				D3DXVECTOR3 vecDiffBullet = pos - posPrediction;
				D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

				moveBullet = vecDiffBullet * SPEED_BULLET;

				CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f,0.2f);

				m_fTimeFire = 0;
			}
		}
	}
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CEnemyNormal::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}