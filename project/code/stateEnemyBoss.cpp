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
#include "enemyDrone.h"
#include "manager.h"
#include "missile.h"
#include "bullet.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.25f;	// �~�T�C�����˂̎���
const int NUM_MISSILE = 10;	// �~�T�C���̔��ː�
const float TIME_DRONE = 1.0f;	// �h���[�����˂̎���
const int NUM_DRONE = 10;	// �h���[���̔��ː�
const int RANGE_HEIGHT_DRONE = 500;	// �h���[���̍����̕�
const float MOVE_DRONE = 50.0f;	// �h���[���̎ˏo���̈ړ���
const float TIME_MG = 0.15f;	// �}�V���K�����˂̎���
const int NUM_MG = 30;	// �}�V���K���̔��ː�
const float SPEED_BULLET = 200.0f;	// �}�V���K���e�̑��x
const int ACCURACY_MG = 10;	// �}�V���K���̐��x
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
		pBoss->ChangeState(new CStateBossAttackMachinegun);
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
		pBoss->ChangeState(new CStateBossLaunchDrone);
	}
}

//=====================================================
// �~�T�C���U��
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
	{// �~�T�C���̔���
		CMissile *pMissile = CMissile::Create(pBoss->GetPosition());

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRot();

			rot.x = -D3DX_PI * 0.5f;

			pMissile->SetRot(rot);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// ��萔�������玟�̍s����
			pBoss->ChangeState(new CStateBossAttackMachinegun);
		}
	}

	// ��ޏ���
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// �q�@�̎ˏo
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
	{// �ˏo����
		CEnemyDrone *pDrone = new CEnemyDrone;

		if (pDrone != nullptr)
		{
			pDrone->Init();

			D3DXVECTOR3 pos = pBoss->GetPosition();

			pDrone->SetPosition(pos);

			pos.y += universal::RandRange(0, -RANGE_HEIGHT_DRONE);

			// �ˏo���Ă���̈ʒu�ݒ�
			pDrone->SetPositionDest(pos);

			// �ړ��ʂ𑫂�
			D3DXVECTOR3 move = { 0.0f,MOVE_DRONE,0.0f };
			pDrone->SetMove(move);
		}

		m_nCnt++;

		if (m_nCnt > NUM_DRONE)
		{// ��萔�������玟�̍s����
			pBoss->ChangeState(new CStateBossAttackMissile);
		}
	}

	// ��ޏ���
	pBoss->Back();
	pBoss->AimPlayer();
}

//=====================================================
// �}�V���K��
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
	{// �ˏo����
		D3DXVECTOR3 posMazzle = pBoss->GetMtxPos(0);
		D3DXVECTOR3 moveBullet;
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

		posPrediction.x += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.y += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.z += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);

		D3DXVECTOR3 vecDiffBullet = posMazzle - posPrediction;
		D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

		moveBullet = vecDiffBullet * SPEED_BULLET;

		CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.01f);

		m_nCnt++;

		if (m_nCnt > NUM_MG)
		{// ��萔�������玟�̍s����
			pBoss->ChangeState(new CStateBossAttackBeam);
		}
	}

	// ��ޏ���
	pBoss->Back();
	pBoss->AimPlayer();
}