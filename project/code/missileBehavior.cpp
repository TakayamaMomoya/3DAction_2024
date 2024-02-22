//*****************************************************
//
// �~�T�C���̃r�w�C�r�A[missileBehavior.xpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "missileBehavior.h"
#include "missile.h"
#include "player.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// ���N���X
//=====================================================
CMissileBehavior::CMissileBehavior()
{// �R���X�g���N�^
	m_bEndChase = false;
}

CMissileBehavior::~CMissileBehavior()
{// �f�X�g���N�^

}

//=====================================================
// ���������̃~�T�C��
//=====================================================
CMissileLine::CMissileLine()
{// �R���X�g���N�^
	
}

CMissileLine::~CMissileLine()
{// �f�X�g���N�^

}

void CMissileLine::Init(CMissile *pMissile)
{// ������

}

void CMissileLine::Uninit(CMissile *pMissile)
{// �I��

}

void CMissileLine::Update(CMissile *pMissile)
{// �X�V
	bool bEndChase = IsEndChase();

	if (bEndChase)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// �ڕW�����̎擾
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

			// �ړ��ʂ𐳖ʂɑ���
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