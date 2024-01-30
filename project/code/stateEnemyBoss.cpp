//*****************************************************
//
// �{�X�G���[stateEnemyBoss.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "stateEnemyBoss.h"
#include "enemyBoss.h"
#include "manager.h"
#include "missile.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.25f;	// �~�T�C�����˂̎���
const int NUM_MISSILE = 10;	// �~�T�C���̔��ː�
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CStateBoss::CStateBoss()
{

}

//=====================================================
// �o�����
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
	{// ���[�V�����I���Ŏ��̏�Ԃֈڂ�
		pBoss->ChangeState(new CStateBossAttackBeam);
	}
}

//=====================================================
// �r�[���U��
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
	{// ���[�V�����I���Ŏ��̏�Ԃֈڂ�
		pBoss->ChangeState(new CStateBossAttackMissile);
	}
}

//=====================================================
// �~�T�C���U��
//=====================================================
void CStateBossAttackMissile::Init(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	m_fTimerMissile = 0.0f;
	m_nCntMissile = 0;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_MISSILE);
}

void CStateBossAttackMissile::Attack(CEnemyBoss *pBoss)
{
	CheckPointer(pBoss);

	float fDeltaTime = CManager::GetDeltaTime();

	m_fTimerMissile += fDeltaTime;

	if (m_fTimerMissile > TIME_MISSILE)
	{// �~�T�C���̔���
		CMissile::Create(pBoss->GetPosition());

		m_nCntMissile++;

		if (m_nCntMissile > NUM_MISSILE)
		{// ��萔�������玟�̍s����
			pBoss->ChangeState(new CStateBossAttackBeam);
		}

		m_fTimerMissile = 0.0f;
	}
}