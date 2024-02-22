//*****************************************************
//
// ミサイルのビヘイビア[missileBehavior.xpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "missileBehavior.h"
#include "missile.h"
#include "player.h"
#include "manager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// 基底クラス
//=====================================================
CMissileBehavior::CMissileBehavior()
{// コンストラクタ
	m_bEndChase = false;
}

CMissileBehavior::~CMissileBehavior()
{// デストラクタ

}

//=====================================================
// 直線動きのミサイル
//=====================================================
CMissileLine::CMissileLine()
{// コンストラクタ
	
}

CMissileLine::~CMissileLine()
{// デストラクタ

}

void CMissileLine::Init(CMissile *pMissile)
{// 初期化

}

void CMissileLine::Uninit(CMissile *pMissile)
{// 終了

}

void CMissileLine::Update(CMissile *pMissile)
{// 更新
	bool bEndChase = IsEndChase();

	if (bEndChase)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// 目標向きの取得
		D3DXVECTOR3 pos = pMissile->GetMtxPos(0);
		D3DXVECTOR3 move = pMissile->GetMove();

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		float fSpeedMax = pMissile->GetSpeedMax();

		posPlayer = universal::LinePridiction(pos, fSpeedMax, posPlayer, movePlayer * 0.1f);

		float fSpeed = D3DXVec3Length(&move);

		D3DXVECTOR3 vecDiff = posPlayer - pos;
		float fLengthDiff = D3DXVec3Length(&vecDiff);

		if (fLengthDiff < 1000.0f)
		{
			bEndChase = true;

			EnableChase(bEndChase);
		}

		float fDeltaTime = CManager::GetDeltaTime();

		float fTime = pMissile->GetTimerHit();

		fTime -= fDeltaTime;

		pMissile->SetTimerHit(fTime);

		float fSpeedChase = pMissile->GetSpeedChase();

		if (fTime > 0.0f)
		{
			D3DXVECTOR3 acceleration = 2.0f * (vecDiff - move);

			if (D3DXVec3Length(&acceleration) > fSpeedChase)
			{
				D3DXVec3Normalize(&acceleration, &acceleration);

				acceleration *= fSpeedChase;
			}

			// 移動量を正面に足す
			move += acceleration;

			if (D3DXVec3Length(&move) > fSpeedMax)
			{
				D3DXVec3Normalize(&move, &move);

				move *= fSpeedMax;
			}

			pMissile->SetMove(move);
		}
	}
}