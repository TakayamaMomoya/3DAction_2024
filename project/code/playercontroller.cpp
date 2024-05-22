//*****************************************************
//
// �v���C���[����[playercontroller.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "playercontroller.h"
#include "player.h"
#include "inputManager.h"
#include "camera.h"
#include "slow.h"
#include "manager.h"
#include "debugproc.h"
#include "enemyManager.h"
#include "sound.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_ASSAULT = 7.0f;	// �ːi�̈ړ����x
const float MIN_ANGLE_CAMERA = D3DX_PI * 0.1f;	// �J�����̉������鐧��
const float MAX_ANGLE_CAMERA = D3DX_PI * 0.9f;	// �J�����̏�����鐧��
const float SPEED_ROLL_CAMERA = 0.03f;	// �J������]���x
}

//===================================================================================
// �R���g���[���[���N���X
//===================================================================================
//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerController::CPlayerController()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerController::~CPlayerController()
{

}

//===================================================================================
// �ړ�����N���X
//===================================================================================
//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerControllerMove::CPlayerControllerMove()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerControllerMove::~CPlayerControllerMove()
{

}

//=====================================================
// �X�V����
//=====================================================
void CPlayerControllerMove::Update(CPlayer *pPlayer)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �v���C���[���擾
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	// �ڕW�����̐ݒ�
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// ���͕����̎擾
	D3DXVECTOR3 vecInput = { 0.0f,0.0f,0.0f };

	vecInput += {sinf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x, 0.0f, cosf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x};
	vecInput += {sinf(pInfoCamera->rot.y) * axis.axisMove.z, 0.0f, cosf(pInfoCamera->rot.y) * axis.axisMove.z};

	float fLengthAxis = D3DXVec3Length(&axisMove);

	int nMotion = pPlayer->GetMotion();

	if ((fLengthAxis >= 0.3f && nMotion != CPlayer::MOTION_SHOT) || nMotion == CPlayer::MOTION_DODGE)
	{// �ʏ�ړ����̖ڕW�����ݒ�
		if (nMotion == CPlayer::MOTION_DODGE)
		{
			D3DXVECTOR3 move = pPlayer->GetMove();

			pInfo->rotDest.y = atan2f(move.x, move.z);
		}
		else
		{
			pInfo->rotDest.y = atan2f(vecInput.x, vecInput.z);
		}

		CDebugProc *pDebugProc = CDebugProc::GetInstance();

		pDebugProc->Print("\n�ʏ�ړ�");
	}

	if (nMotion != CPlayer::MOTION_DODGE && 
		(nMotion == CPlayer::MOTION_SHOT || nMotion == CPlayer::MOTION_ASSAULT || nMotion == CPlayer::MOTION_MELEE || nMotion == CPlayer::MOTION_MELEE2 || nMotion == CPlayer::MOTION_THROW || pInfo->bLockTarget))
	{// �G�̕�������������
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

			if (pEnemyLockon != nullptr)
			{
				D3DXVECTOR3 pos = pPlayer->GetPosition();
				pos.y += 200.0f;

				D3DXVECTOR3 posEnemy = pEnemyLockon->GetMtxPos(0);

				D3DXVECTOR3 vecDiff = posEnemy - pos;

				pInfo->rotDest = universal::VecToRot(vecDiff);
				pInfo->rotDest.x -= D3DX_PI * 0.5f;
			}
		}
	}

	if (nMotion != CPlayer::MOTION_ASSAULT &&
		nMotion != CPlayer::MOTION_MELEE &&
		nMotion != CPlayer::MOTION_MELEE2 &&
		nMotion != CPlayer::MOTION_GRAB &&
		nMotion != CPlayer::MOTION_THROW &&
		nMotion != CPlayer::MOTION_DODGE &&
		pFragMotion->bStamp == false)
	{
		// �������͂̎擾
		CInputManager::SAxis axis = pInputManager->GetAxis();
		D3DXVECTOR3 axisMove = axis.axisMove;

		D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = pPlayer->GetRotation();

		float fLengthAxis = D3DXVec3Length(&axisMove);

		if (pFragMotion->bMove && fLengthAxis <= 0.3f)
		{// �}��~�t���O
			pFragMotion->bStop = true;
		}

		fLengthAxis *= param.fSpeedMove;

		if (nMotion == CPlayer::MOTION_SHOT || pInfo->bLockTarget)
		{// ���_�Œ莞
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			vecMove += {sinf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x, 0.0f, cosf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x};
			vecMove += {sinf(rotCamera.y) * axisMove.z, 0.0f, cosf(rotCamera.y) * axisMove.z};
		}
		else
		{// ���������R�Ȏ�
			vecMove -= {sinf(rot.y) * fLengthAxis, 0.0f, cosf(rot.y) * fLengthAxis};
		}

		// �ړ����x�̐ݒ�
		D3DXVECTOR3 move = pPlayer->GetMove();

		D3DXVec3Normalize(&vecMove, &vecMove);
		vecMove *= param.fSpeedMove;

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			vecMove *= fScale;
		}

		// �W�����v����
		if (pInfo->bLand)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// �n��p�̃W�����v
				pFragMotion->bJump = true;
				pFragMotion->bMove = false;

				Sound::Play(CSound::LABEL_SE_BOOST00);
			};
		}
		else
		{
			if (pInfo->stateBoost != CPlayer::STATEBOOST_OVERHEAT)
			{// �󒆂̃W�����v����̓u�[�X�g�e�ʂ��K�v
				if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
				{// �W�����v�{�^�����������u�Ԃ�������炷
					Sound::Play(CSound::LABEL_SE_BOOST00);
				}

				if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
				{// �u�[�X�g�㏸
					vecMove.y += 1.0f;

					pPlayer->AddBoost(-3.0f);

					pFragMotion->bFall = false;
				}
				else if(pFragMotion->bAir)
				{
					pFragMotion->bFall = true;
				}
			}

			// ���݂�����
			pPlayer->Stamp();
		}

		// ���͕����̎擾
		float fAngleInput = atan2f(axisMove.x, axisMove.z);

		if (pInfo->stateBoost != CPlayer::STATEBOOST_OVERHEAT)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
			{// �u�[�X�g���
				vecMove +=
				{
					sinf(pInfoCamera->rot.y + fAngleInput) * param.fSpeedDodge,
					0.0f,
					cosf(pInfoCamera->rot.y + fAngleInput) * param.fSpeedDodge,
				};

				pPlayer->AddBoost(-50.0f);

				pFragMotion->bDodge = true;

				Sound::Play(CSound::LABEL_SE_DASH00);
			}
		}

		// �ړ��ʂ̔��f
		move += vecMove;

		pPlayer->SetMove(move);
	}
	else if (nMotion == CPlayer::MOTION_ASSAULT)
	{// �ːi���̐��i
		pPlayer->AddMoveForward(SPEED_ASSAULT);
	}
}

//===================================================================================
// �J��������N���X
//===================================================================================
//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerControllerCamera::CPlayerControllerCamera()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerControllerCamera::~CPlayerControllerCamera()
{

}

//=====================================================
// �X�V����
//=====================================================
void CPlayerControllerCamera::Update(CPlayer *pPlayer)
{
	// ���̓}�l�[�W���[�擾
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// �v���C���[���擾
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	if (pInputManager->GetTrigger(CInputManager::BUTTON_LOCK))
	{// �^�[�Q�b�g���b�N�؂�ւ�
		pInfo->bLockTarget = pInfo->bLockTarget ? false : true;

		pPlayer->ToggleLockTarget();
	}

	if (pInfo->bLockTarget)
	{// ���b�N���Ă�G�̐؂�ւ�
		pPlayer->SwitchLockEnemy();
	}

	// �������͂̎擾
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisCamera = axis.axisCamera;

	// �J�����̉�]
	pInfoCamera->rot.x += axisCamera.y * SPEED_ROLL_CAMERA;
	pInfoCamera->rot.y += axisCamera.x * SPEED_ROLL_CAMERA;

	universal::LimitValue(&pInfoCamera->rot.x, MAX_ANGLE_CAMERA, MIN_ANGLE_CAMERA);

	universal::LimitRot(&pInfoCamera->rot.x);
	universal::LimitRot(&pInfoCamera->rot.y);
}

//===================================================================================
// �U������N���X
//===================================================================================
//=====================================================
// �R���X�g���N�^
//=====================================================
CPlayerControllerAttack::CPlayerControllerAttack()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayerControllerAttack::~CPlayerControllerAttack()
{

}

//=====================================================
// �X�V����
//=====================================================
void CPlayerControllerAttack::Update(CPlayer *pPlayer)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	int nMotion = pPlayer->GetMotion();

	if (pInputManager == nullptr)
	{
		return;
	}

	// �v���C���[���擾
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	if (pInputManager->GetPress(CInputManager::BUTTON_SHOT) &&
		pInfo->aHeat[CPlayer::PARAM_GUN] == false)
	{// �ˌ�����
		pFragMotion->bShot = true;
	}
	else
	{
		pFragMotion->bShot = false;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE) &&
		pInfo->aHeat[CPlayer::PARAM_MELEE] == false)
	{// �ߐڍU��
		pFragMotion->bMelee = true;

		if (nMotion == CPlayer::MOTION_MELEE)
		{
			pFragMotion->bAddAttack = true;
		}
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_GRAB))
	{// �͂ݏ���

		if (nMotion != CPlayer::MOTION_THROW && nMotion != CPlayer::MOTION_GRAB)
		{
			pFragMotion->bGrab = true;
		}

		if (nMotion == CPlayer::MOTION_THROW)
		{
			// �X���[���L�����Z��
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}