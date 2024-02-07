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
#include "animEffect3D.h"
#include "fade.h"
#include "explosionspawner.h"
#include "particle.h"
#include "anim3D.h"
#include "camera.h"
#include "meshfield.h"
#include "blockManager.h"
#include "orbit.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float TIME_MISSILE = 0.5f;	// �~�T�C�����˂̎���
const int NUM_MISSILE = 10;	// �~�T�C���̔��ː�
const float TIME_DRONE = 1.0f;	// �h���[�����˂̎���
const int NUM_DRONE = 3;	// �h���[���̔��ː�
const int RANGE_HEIGHT_DRONE = 500;	// �h���[���̍����̕�
const float MOVE_DRONE = 50.0f;	// �h���[���̎ˏo���̈ړ���
const float TIME_MG = 0.15f;	// �}�V���K�����˂̎���
const int NUM_MG = 30;	// �}�V���K���̔��ː�
const float SPEED_BULLET = 200.0f;	// �}�V���K���e�̑��x
const int ACCURACY_MG = 10;	// �}�V���K���̐��x
const float GRAVITY = 0.4f;	// �d��
const float SPEED_SLASH = 0.05f;	// �a�����̓ˌ��X�s�[�h
const float SPEED_STEP = 0.02f;	// �X�e�b�v�̃X�s�[�h
const float RANGE_SLASH = 1000.0f;	// �a���Ɉڂ�܂ł̋���
const D3DXVECTOR3 POS_CENTER = { 5170.0f,-6245.0f,3791.0f };	// �X�e�[�W�̒��S���W
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

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SHOT);
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
		CMissile *pMissile = CMissile::Create(pBoss->GetMtxPos(1));

		if (pMissile != nullptr)
		{
			D3DXVECTOR3 rot = pBoss->GetRotation();


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

			pMissile->SetRotation(rot);
		}

		m_nCnt++;

		if (m_nCnt > NUM_MISSILE)
		{// ��萔�������玟�̍s����
			pBoss->ChangeState(new CStateBossAttackMachinegun);
		}
	}

	// ��ޏ���
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
		D3DXVECTOR3 posMazzle = pBoss->GetMtxPos(5);
		D3DXVECTOR3 moveBullet;
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �΍��̌v�Z
		D3DXVECTOR3 posPrediction = universal::LinePridiction(posMazzle, SPEED_BULLET, posPlayer, movePlayer);

		posPrediction.x += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.y += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);
		posPrediction.z += (float)universal::RandRange(ACCURACY_MG, -ACCURACY_MG);

		D3DXVECTOR3 vecDiffBullet = posMazzle - posPrediction;
		D3DXVec3Normalize(&vecDiffBullet, &vecDiffBullet);

		moveBullet = vecDiffBullet * SPEED_BULLET;

		// �e�̔���
		CBullet::Create(posMazzle, -moveBullet, 5, CBullet::TYPE::TYPE_ENEMY, false, 50.0f, 0.01f);

		// �A�j���G�t�F�N�g����
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			pAnim3D->CreateEffect(posMazzle, CAnimEffect3D::TYPE::TYPE_MUZZLEFLUSH);
		}

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

//=====================================================
// ���`�Ԃֈڍs����
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
			{// �t�F�[�h�ő��`�ԂɈڂ�
				Evolve(pBoss);
			}
		}

		if (state == CFade::FADE::FADE_OUT)
		{
			if (nMotion != CEnemyBoss::MOTION::MOTION_DEATH)
			{// ���[�r�[�ɂ���
				TransMovie(pBoss);
			}

			if (m_fTimeTrans > 6.0f)
			{// �n�`�̕ϓ�
				// �u���b�N�폜
				CBlockManager *pBlockManager = CBlockManager::GetInstance();

				if (pBlockManager != nullptr)
					pBlockManager->DeleteAll();

				// ���b�V���t�B�[���h�ό`
				CMeshField *pMesh = CMeshField::GetInstance();

				if (pMesh != nullptr)
					pMesh->Load("data\\MAP\\field01.bin");

				pBoss->ChangeState(new CStateBossBeforeTrans);
			}
		}
	}
}

void CStateBossTrans::TransMovie(CEnemyBoss *pBoss)
{// ���[�r�[�Ɉڍs
	D3DXVECTOR3 pos = { 7000.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = { 0.0f,D3DX_PI * 0.5f,0.0f };

	// �{�X�ʒu��ݒ�
	pBoss->SetPosition(pos);
	pBoss->SetRotation(rot);

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// �v���C���[�ʒu��ݒ�
		D3DXVECTOR3 posPlayer = { 5000.0f,0.0f,0.0f };
		D3DXVECTOR3 rotPlayer = { 0.0f,0.0f,0.0f };

		pPlayer->SetPosition(posPlayer);
		pPlayer->SetRotation(rotPlayer);
	}

	// �����G�t�F�N�g
	pos.y += 200.0f;
	CExplSpawner::Create(pos, 300.0f, 360);
	CParticle::Create(pos, CParticle::TYPE::TYPE_TURN_EXPLOSION);

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_DEATH);
}

void CStateBossTrans::Evolve(CEnemyBoss *pBoss)
{// ���`�Ԃɐi��
	// �唚�����N����
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

	// �J������h�炷
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetQuake(0.2f, 0.2f, 120);
	}

	// �����t�F�[�h��������
	CFade *pFade = CFade::GetInstance();

	if (pFade != nullptr)
	{
		pFade->SetFade(CScene::MODE_GAME, false);
	}
}

//=====================================================
// ���`�ԂɈڂ����Ƃ��̏���
//=====================================================
void CStateBossBeforeTrans::Init(CEnemyBoss *pBoss)
{
	pBoss->Load("data\\MOTION\\robot00.txt");

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);

	pBoss->EnableTrans(true);
}

void CStateBossBeforeTrans::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 move = pBoss->GetMove();

	move.y -= GRAVITY;

	pBoss->SetMove(move);

	bool bFinish = pBoss->IsFinish();

	if (bFinish)
	{// �s���ɑJ��
		pBoss->ChangeState(new CStateBossSlash);

		// �����蔻��Đ���
		pBoss->CreateCollision(Boss::RADIUS_COLLISION);

		// ���C�t���Z�b�g
		pBoss->SetLife(Boss::INITIAL_LIFE);
	}
}

//=====================================================
// �a�����[�V����
//=====================================================
void CStateBossSlash::Init(CEnemyBoss *pBoss)
{
	m_pOrbit = nullptr;

	pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_PRE_SLASH);
}

void CStateBossSlash::Move(CEnemyBoss *pBoss)
{
	int nMotion = pBoss->GetMotion();
	bool bFinish = pBoss->IsFinish();

	if (nMotion == CEnemyBoss::MOTION::MOTION_PRE_SLASH)
	{
		pBoss->AimPlayer(0.0f, false);

		if (bFinish)
		{
			// �ړI�n��ݒ�
			CPlayer *pPlayer = CPlayer::GetInstance();

			if (pPlayer == nullptr)
				return;

			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXVECTOR3 pos = pBoss->GetPosition();

			universal::MoveToDest(&pos, posPlayer, SPEED_SLASH);

			pBoss->SetPosition(pos);

			if (universal::DistCmp(pos, posPlayer, RANGE_SLASH, nullptr))
			{
				pBoss->SetMotion(CEnemyBoss::MOTION::MOTION_SLASH);

				if (m_pOrbit == nullptr)
				{
					// �O�Ղ̐���
					D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

					D3DXVECTOR3 offset = { 0.0f,-1000.0f,0.0f };

					m_pOrbit = COrbit::Create(mtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), offset, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20);
				}
			}
		}
	}
	else if (nMotion == CEnemyBoss::MOTION::MOTION_SLASH)
	{
		if (m_pOrbit != nullptr)
		{// �O�Ղ̍X�V
			D3DXMATRIX mtx = *pBoss->GetParts(CEnemyBoss::IDX_HAND_L)->pParts->GetMatrix();

			m_pOrbit->SetPositionOffset(mtx, 0);
		}

		if (bFinish)
		{
			if (m_pOrbit != nullptr)
			{
				m_pOrbit->SetEnd(true);
				m_pOrbit = nullptr;
			}

			pBoss->ChangeState(new CStateBossStep);
		}
	}

	pBoss->BeamBlade();
}

//=====================================================
// �X�e�b�v���
//=====================================================
void CStateBossStep::Init(CEnemyBoss *pBoss)
{
	// �ړI�n��ݒ�
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	// �ړI�n��ݒ�
	m_posDest = universal::RelativeInversPos(posPlayer,POS_CENTER,1.0f);
	m_posDestMid = universal::RelativeInversPos(posPlayer, POS_CENTER, 0.0f);

	m_bMid = false;
}

void CStateBossStep::Move(CEnemyBoss *pBoss)
{
	D3DXVECTOR3 pos = pBoss->GetPosition();

	pBoss->SetAfterImage(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f), 20);

	if (m_bMid)
	{
		universal::MoveToDest(&pos, m_posDest, SPEED_STEP);

		if (universal::DistCmpFlat(pos, m_posDest, RANGE_SLASH, nullptr))
		{
			pBoss->ChangeState(new CStateBossSlash);
		}
	}
	else
	{
		universal::MoveToDest(&pos, m_posDestMid, SPEED_STEP);

		if (universal::DistCmpFlat(pos, m_posDestMid, RANGE_SLASH, nullptr))
		{
			m_bMid = true;
		}
	}

	pBoss->SetPosition(pos);
}