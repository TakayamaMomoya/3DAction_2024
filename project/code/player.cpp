//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "player.h"
#include "collision.h"
#include "motion.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "universal.h"
#include "slow.h"
#include "camera.h"
#include "manager.h"
#include "bullet.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const char* BODY_PATH = "data\\MOTION\\motionArms01.txt";	// �����ڂ̃p�X
const float GRAVITY = 0.30f;	// �d��
const float SPEED_ROLL_CAMERA = 0.03f;	// �J������]���x
const float SPEED_BULLET = 50.0f;	// �e��
const float POW_JUMP = 20.0f;	// �W�����v��
const float SPEED_MOVE = 2.0f;	// �ړ����x
const float FACT_MOVE = 0.04f;	// �ړ��̌����W��
const float SPEED_ASSAULT = 4.0f;	// �ːi�̈ړ����x
const float POW_ADDMELEE = 50.0f;	// �ǌ��̐��i��
const float SPEED_DODGE = 50.0f;	// ��𐄐i��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// ���g�̃|�C���^

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;

	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	ZeroMemory(&m_fragMotion, sizeof(CPlayer::SFragMotion));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// ��������
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// ID�ɑΉ��������f���̐ݒ�
	CMotion::Load((char*)BODY_PATH);

	// �p���N���X�̏�����
	CMotion::Init();

	// �����蔻��̐���
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(20.0f);
		}
	}

	if (m_info.pClsnAttack == nullptr)
	{// ���̓����蔻�萶��
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// ���̐ݒ�
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// �����蔻�萶��
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER, this);

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = { 20.0f,100.0f,20.0f };
			D3DXVECTOR3 vtxMin = { -20.0f,0.0f,-20.0f };

			m_info.pCollisionCube->SetPosition(GetPosition());

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	m_info.fLife = m_param.fInitialLife;
	m_param.fSpeedMove = SPEED_MOVE;

	// �e�̗L����
	SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	EnableShadow(true);

	SetMotion(MOTION_WALK_FRONT);

	m_info.bLand = true;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->Uninit();
		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{
		m_info.pCollisionCube->Uninit();
		m_info.pCollisionCube = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	CSlow *pSlow = CSlow::GetInstance();

	// �p���N���X�̍X�V
	CMotion::Update();

	// ����
	Input();

	// �v���C���[�̉�]
	Rotation();

	// �ʒu�̔��f
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	
	// �O��̈ʒu��ۑ�
	SetPositionOld(pos);

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		pos += move * fScale;
		SetPosition(pos);
	}
	else
	{
		pos += move;
		SetPosition(pos);
	}

	// �d��
	int nMotion = GetMotion();
	
	if (nMotion != MOTION_SHOT && nMotion != MOTION_ASSAULT && nMotion != MOTION_MELEE && nMotion != MOTION_MELEE2)
	{
		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			move.x += (0 - move.x) * FACT_MOVE * fScale;
			move.z += (0 - move.z) * FACT_MOVE * fScale;

			move.y -= GRAVITY * fScale;
		}
		else
		{
			move.x *= FACT_MOVE;
			move.z *= FACT_MOVE;

			move.y -= GRAVITY;
		}
	}
	else
	{
		move.x += (0 - move.x) * 0.05f;
		move.y += (0 - move.y) * 0.5f;
		move.z += (0 - move.z) * 0.05f;
	}

	SetMove(move);

	// �����蔻��̊Ǘ�
	ManageCollision();

	// ��ԊǗ�
	ManageState();

	// ���[�V�����Ǘ�
	ManageMotion();

// �f�o�b�O����
#if _DEBUG

#endif // _DEBUG
}

//=====================================================
// ���͏���
//=====================================================
void CPlayer::Input(void)
{
	// �ړ�����
	InputMove();

	// �J��������
	InputCamera();

	// �U������
	InputAttack();
}

//=====================================================
// �ړ��̓���
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
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

	int nMotion = GetMotion();

	if (nMotion != MOTION_ASSAULT && nMotion != MOTION_MELEE && nMotion != MOTION_MELEE2)
	{
		// �������͂̎擾
		CInputManager::SAxis axis = pInputManager->GetAxis();
		D3DXVECTOR3 axisMove = axis.axisMove;

		D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };

		vecMove += {sinf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x, 0.0f, cosf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x};
		vecMove += {sinf(pInfoCamera->rot.y) * axis.axisMove.z, 0.0f, cosf(pInfoCamera->rot.y) * axis.axisMove.z};

		float fLengthAxis = D3DXVec3Length(&axisMove);

		if (m_fragMotion.bMove && fLengthAxis <= 0.3f)
		{// �}��~�t���O
			m_fragMotion.bStop = true;
		}

		// �ړ����x�̐ݒ�
		D3DXVECTOR3 move = GetMove();

		D3DXVec3Normalize(&vecMove, &vecMove);
		vecMove *= m_param.fSpeedMove;

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			vecMove *= fScale;
		}

		if (m_info.bLand)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// �W�����v����
				m_fragMotion.bJump = true;
				m_fragMotion.bMove = false;
			};
		}
		else
		{
			if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
			{// �u�[�X�g�㏸
				vecMove.y += 1.0f;
			};
		}
		
		float fAngleInput = atan2f(axisMove.x, axisMove.z);

		if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
		{
			vecMove +=
			{
				sinf(fAngleInput) * SPEED_DODGE,
				0.0f,
				cosf(fAngleInput) * SPEED_DODGE,
			};
		}

		move += vecMove;

		SetMove(move);
	}
	else if (nMotion == MOTION_ASSAULT)
	{
		AddMoveForward(SPEED_ASSAULT);
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// �X���[�ɂ���
			if (pSlow != nullptr)
			{
				pSlow->SetScale(0.25f);
			}
		}

		if (pKeyboard->GetRelease(DIK_RETURN))
		{// �X���[����
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}

		if (pKeyboard->GetTrigger(DIK_O))
		{// ���p�[�c���Z�b�g
			ResetEnableMotion();
		}
	}
#endif
}

//=====================================================
// �J��������
//=====================================================
void CPlayer::InputCamera(void)
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

	// �������͂̎擾
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisCamera = axis.axisCamera;

	// �J�����̉�]
	pInfoCamera->rot.x += axisCamera.y * SPEED_ROLL_CAMERA;
	pInfoCamera->rot.y += axisCamera.x * SPEED_ROLL_CAMERA;

	universal::LimitRot(&pInfoCamera->rot.x);
	universal::LimitRot(&pInfoCamera->rot.y);
}

//=====================================================
// �U���̓���
//=====================================================
void CPlayer::InputAttack(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	int nMotion = GetMotion();

	if (pInputManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_SHOT))
	{// �ˌ�����
		m_fragMotion.bShot = true;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE))
	{// �ߐڍU������
		m_fragMotion.bMelee = true;

		if (nMotion == MOTION_MELEE)
		{
			m_fragMotion.bAddAttack = true;
		}
	}
}

//=====================================================
// �v���C���[�̉�]
//=====================================================
void CPlayer::Rotation(void)
{
	// �ړ������̎擾
	D3DXVECTOR3 move = GetMove();

	float fAngleMove = atan2f(-move.x, -move.z);
	float fLenghtMove = sqrtf(move.x * move.x + move.z * move.z);

	int nMotion = GetMotion();

	if (nMotion == MOTION_SHOT)
	{
		// �J�����擾
		CCamera *pCamera = CManager::GetCamera();

		if (pCamera == nullptr)
		{
			return;
		}

		CCamera::Camera *pInfoCamera = pCamera->GetCamera();

		// �����̕␳
		D3DXVECTOR3 rot = GetRot();
		D3DXVECTOR3 rotCamera = pInfoCamera->rot;

		rotCamera.x -= D3DX_PI * 0.5f;
		rotCamera.y += D3DX_PI;

		universal::LimitRot(&rotCamera.x);
		universal::LimitRot(&rotCamera.y);

		universal::FactingRot(&rot.x, rotCamera.x, 0.15f);
		universal::FactingRot(&rot.y, rotCamera.y, 0.15f);

		SetRot(rot);
	}
	else
	{
		if (fLenghtMove >= 10.0f)
		{
			// �����̕␳
			D3DXVECTOR3 rot = GetRot();

			universal::FactingRot(&rot.y, fAngleMove, 0.1f);

			SetRot(rot);

			if (m_info.bLand)
			{
				m_fragMotion.bMove = true;
			}
		}
		else
		{
			m_fragMotion.bMove = false;
		}

		// �����̕␳
		D3DXVECTOR3 rot = GetRot();

		rot.x += (0 - rot.x) * 0.1f;

		SetRot(rot);
	}
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// �ʏ���

	}
		break;
	case STATE_DAMAGE:
	{// �_���[�W���

	}
		break;
	case STATE_DEATH:
	{// ���S���

	}
		break;
	default:
		break;
	}
}

//=====================================================
// �����蔻��Ǘ�
//=====================================================
void CPlayer::ManageCollision(void)
{
	// �����蔻��̒Ǐ]
	if (m_info.pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetMtxPos(0);

		// �G�Ƃ̐ڐG����
		m_info.pCollisionSphere->SetPosition(posWaist);

		bool bHit = m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY);

		if (bHit)
		{
			Hit(5.0f);
		}

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();
			D3DXVECTOR3 move = GetMove();

			// ����̒Ǐ]
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// �u���b�N�Ƃ̉����o������
			m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			if (pos.y <= 0.0f && posOld.y >= 0.0f)
			{
				pos.y = 0.0f;
				move.y = 0.0f;

				SetPosition(pos);
				
				int nMotion = GetMotion();
				bool bFinish = IsFinish();

				if (nMotion == MOTION_AIR)
				{
					m_info.bLand = true;
					m_fragMotion.bAir = false;
					m_fragMotion.bJump = false;
				}
			}
			else
			{
				m_info.bLand = false;

				m_fragMotion.bAir = true;
			}

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			pos = GetPosition();

			// �G�Ƃ̉����o������
			m_info.pCollisionSphere->SetPosition(pos);

			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			SetPosition(pos);
		}
	}
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (nMotion == MOTION_MELEE2)
	{
		if (bFinish)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (nMotion == MOTION_MELEE)
	{
		if (bFinish)
		{
			if (m_fragMotion.bAddAttack)
			{
				SetMotion(MOTION_MELEE2);
				m_fragMotion.bAddAttack = false;
				m_fragMotion.bMelee = false;

				AddMoveForward(POW_ADDMELEE);
			}
			else
			{
				SetMotion(MOTION_AIR);
			}
		}
	}
	else if (m_fragMotion.bMelee)
	{// �ߐڍU�����[�V����
		if (nMotion != MOTION_ASSAULT)
		{
			SetMotion(MOTION_ASSAULT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bMelee = false;

				SetMotion(MOTION_MELEE);
			}
		}
	}
	else if (m_fragMotion.bShot)
	{// �ˌ����[�V����
		if (nMotion != MOTION_SHOT)
		{
			SetMotion(MOTION_SHOT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bShot = false;
			}
		}
	}
	else if (m_fragMotion.bAir)
	{// �؋󃂁[�V����
		if (nMotion != MOTION_AIR)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bJump)
	{// �W�����v���[�V����
		if (nMotion != MOTION_JUMP)
		{
			SetMotion(MOTION_JUMP);
		}
		else
		{

		}
	}
	else if (m_fragMotion.bStop)
	{// �}��~���[�V����
		if (nMotion != MOTION_STOP)
		{
			SetMotion(MOTION_STOP);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bStop = false;
			}
		}
	}
	else if (m_fragMotion.bMove)
	{// �������[�V����
		if (nMotion != MOTION_WALK_FRONT)
		{
			SetMotion(MOTION_WALK_FRONT);
		}
	}
	else
	{// �ҋ@���[�V����
		if (nMotion != MOTION_NEUTRAL)
		{
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// �O���Ɉړ��ʂ�������
//=====================================================
void CPlayer::AddMoveForward(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRot();

	vecMove =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * fSpeed,
		cosf(rot.x - D3DX_PI * 0.5f) * fSpeed,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * fSpeed
	};

	move += vecMove;

	SetMove(move);
}

//=====================================================
// �C�x���g�^�C�~���O�̊Ǘ�
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	if (nMotion == MOTION_SHOT)
	{// �e�𔭎�
		if (pEventInfo != nullptr)
		{
			D3DXVECTOR3 offset = pEventInfo->offset;
			D3DXMATRIX mtxMazzle;
			D3DXMATRIX mtxWeapon = *GetParts(15)->pParts->GetMatrix();

			universal::SetOffSet(&mtxMazzle, mtxWeapon, offset);

			D3DXVECTOR3 posMazzle = { mtxMazzle._41,mtxMazzle._42 ,mtxMazzle._43 };

			Shot(posMazzle);
		}
	}
	if (nMotion == MOTION_JUMP)
	{// �W�����v
		D3DXVECTOR3 move = GetMove();

		move.y += POW_JUMP;

		SetMove(move);
	}

	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{// �ߐڍU��
		D3DXVECTOR3 offset = pEventInfo->offset;
		D3DXMATRIX mtxMazzle;
		D3DXMATRIX mtxParts = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

		universal::SetOffSet(&mtxMazzle, mtxParts, offset);

		D3DXVECTOR3 posMazzle = { mtxMazzle._41,mtxMazzle._42 ,mtxMazzle._43 };

		ManageAttack(posMazzle,300.0f);
	}
}

//=====================================================
// �ˌ�
//=====================================================
void CPlayer::Shot(D3DXVECTOR3 posMazzle)
{
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * SPEED_BULLET,
		cosf(rot.x - D3DX_PI * 0.5f) * SPEED_BULLET,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * SPEED_BULLET
	};

	CBullet *pBullet = CBullet::Create(posMazzle, move, 5, CBullet::TYPE_PLAYER, false, 40.0f, 5.0f,
		D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
}

//=====================================================
// �U������̊Ǘ�
//=====================================================
void CPlayer::ManageAttack(D3DXVECTOR3 pos, float fRadius)
{
	if (m_info.pClsnAttack == nullptr)
	{// ����̃G���[
		return;
	}

	m_info.pClsnAttack->SetPosition(pos);
	m_info.pClsnAttack->SetRadius(fRadius);

	if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
	{// �ΏۂƂ̓����蔻��1
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{
			// ���������I�u�W�F�N�g�̃q�b�g����
			pObj->Hit(0.5f);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �p���N���X�̕`��
	CMotion::Draw();

	// �f�o�b�O�\��
	Debug();
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_info.state == STATE_NORMAL)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// ���S����
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			Uninit();
		}
		else
		{// �_���[�W����
			m_info.state = STATE_DAMAGE;
		}
	}
}

//=====================================================
// �f�o�b�O�\��
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n�v���C���[�̈ʒu[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\n�v���C���[�̈ړ���[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);

	int nMotion = GetMotion();

	pDebugProc->Print("\n���[�V����[%d]", nMotion);
	pDebugProc->Print("\n���n[%d]", m_info.bLand);
}