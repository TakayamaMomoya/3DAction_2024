//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyNormal.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float DIST_FIRE = 500.0f;	// ���˂���܂ł̎����ƃv���C���[�̍�������
const float SPEED_BULLET = 70.0f;	// �e�̑��x
const float TIME_FIRE = 1.0f;	// ���ˊԊu
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyNormal::CEnemyNormal()
{
	m_fTimeFire = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// �p���N���X�̏�����
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyNormal::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// ���S���[�V����
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
		// �v���C���[�̕�������
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �����p�x�̎擾
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.x, -rotDest.x + D3DX_PI * 0.5f, 0.1f);
		universal::FactingRot(&rot.y, rotDest.y, 0.1f);

		SetRot(rot);

		// �v���C���[�Ɍ������Ēe������
		float fDist = D3DXVec3Length(&vecDiff);

		D3DXMATRIX mtxEye;
		D3DXMATRIX mtx = *GetMatrix();

		universal::SetOffSet(&mtxEye, mtx,D3DXVECTOR3(0.0f,0.0f, fDist));

		D3DXVECTOR3 vecEye = { mtxEye._41,mtxEye._42 ,mtxEye._43 };

		D3DXVECTOR3 vecDiffEye = posPlayer - vecEye;

		float fDistEyeToPlayer = D3DXVec3Length(&vecDiffEye);

		// �_�����߂������甭��
		if (fDistEyeToPlayer < DIST_FIRE)
		{
			float fDeltaTime = CManager::GetDeltaTime();

			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{// �^�C���X�P�[����������
				float fScale = pSlow->GetScale();

				fDeltaTime *= fScale;
			}

			// �^�C�}�[���Z
			m_fTimeFire += fDeltaTime;

			if (m_fTimeFire > TIME_FIRE)
			{// �e�̔���
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
// �ڕW�̒ǐ�
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyNormal::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}