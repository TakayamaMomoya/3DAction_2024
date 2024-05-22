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
#include "enemyManager.h"
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
#include "effect3D.h"
#include "meshfield.h"
#include "cameraBehavior.h"
#include "heat.h"
#include "particle.h"
#include "debrisSpawner.h"
#include "game.h"
#include "boostEffect.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "continue.h"
#include "pause.h"
#include "inpact.h"
#include "sound.h"
#include "UIManager.h"
#include "orbit.h"
#include "playercontroller.h"
#include "rader.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float LIMIT_HEIGHT = 4000.0f;	// ��������
const D3DXVECTOR3 POS_PARAM[CPlayer::PARAM_MAX] =
{// �p�����[�^�\���̈ʒu
	{SCREEN_WIDTH * 0.5f - 370.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// �e
	{SCREEN_WIDTH * 0.5f + 370.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// �ߐ�
};
const char* PATH_PARAM[CPlayer::PARAM_MAX] =
{// �p�����[�^UI�̃e�N�X�`���p�X
	"data\\TEXTURE\\UI\\frame00.png",
	"data\\TEXTURE\\UI\\frame01.png",
};
const int RAND_SHOT = 60;	// �ˌ����x�̃����_����
const float SIZE_HIT_EFFECT = 600.0f;	// �q�b�g�G�t�F�N�g�̃T�C�Y
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
	// �p�����[�^�[�Ǎ�
	Load();

	// ���f���̐ݒ�
	CMotion::Load((char*)&m_param.pathBody[0]);

	// �p���N���X�̏�����
	CMotion::Init();

	m_info.rotDest = D3DXVECTOR3(0.0f, 1.57f, 0.0f);
	SetRotation(D3DXVECTOR3(0.0f, -1.57f, 0.0f));

	// �����蔻��̐���
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(50.0f);
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

	m_info.state = STATE_NORMAL;
	m_info.stateBoost = STATEBOOST_NORMAL;
	m_info.bLand = true;

	SetMotion(MOTION_WALK_FRONT);

	// �J�����̍s���ݒ�
	Camera::ChangeBehavior(new CFollowPlayer);

	// UI�̐���
	for (int i = 0; i < PARAM_MAX; i++)
	{
		m_info.apHeatUI[i] = CHeat::Create();

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetPosition(POS_PARAM[i]);
			m_info.apHeatUI[i]->BindTextureFrame(PATH_PARAM[i]);
		}
	}

	for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
	{// �R���g���[���[�̐���
		CreateController((TYPE_CONTROLLER)i);
	}

	// ���[�_�[����
	CRader::Create();

	return S_OK;
}

//=====================================================
// �Ǎ�����
//=====================================================
void CPlayer::Load(void)
{
	char cTemp[256];
	int nThruster = 0;

	FILE *pFile = fopen("data\\TEXT\\playerParam.txt", "r");

	if (pFile != nullptr)
	{
		while (true)
		{
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "BODY_PATH") == 0)
			{// �����ڂ̃p�X
				char aPath[MAX_STRING];

				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%s", &aPath[0]);

				m_param.pathBody = aPath;
			}

			if (strcmp(cTemp, "LIFE") == 0)
			{// �����̗�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fInitialLife);

				m_info.fLife = m_param.fInitialLife;
			}


			if (strcmp(cTemp, "SPEED_MOVE") == 0)
			{// �ړ����x
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedMove);
			}

			if (strcmp(cTemp, "FACT_MOVE") == 0)
			{// �ړ��ʂ̌����W��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fFactMove);
			}

			if (strcmp(cTemp, "BOOST") == 0)
			{// �u�[�X�g�ő��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fInitialBoost);

				m_info.fBoost = m_param.fInitialBoost;
			}

			if (strcmp(cTemp, "REGEN_BOOST") == 0)
			{// �u�[�X�g�񕜗�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fRegenBoost);
			}

			if (strcmp(cTemp, "GRAVITY") == 0)
			{// �d��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fGravity);
			}

			if (strcmp(cTemp, "SPEED_BULLET") == 0)
			{// �e�̑��x
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedBullet);
			}

			if (strcmp(cTemp, "DAMAGE_BULLET") == 0)
			{// �e�̈З�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fDamageBullet);
			}

			if (strcmp(cTemp, "ACC_SHOT") == 0)
			{// �ˌ����x
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fAccuracyBullet);
			}

			if (strcmp(cTemp, "POW_JUMP") == 0)
			{// �W�����v��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowJump);
			}

			if (strcmp(cTemp, "SPEED_DODGE") == 0)
			{// ��𑬓x
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedDodge);
			}

			if (strcmp(cTemp, "SPEED_STAMP") == 0)
			{// ���݂��̐������i��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedStamp);
			}

			if (strcmp(cTemp, "POW_ADDMELEE") == 0)
			{// �ǌ����i��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowAddMelee);
			}

			if (strcmp(cTemp, "POW_THROW") == 0)
			{// �����̗�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowThrow);
			}

			if (strcmp(cTemp, "POW_STAMP") == 0)
			{// ���݂��̐������i��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowStamp);
			}

			if (strcmp(cTemp, "LENGTH_LOCKON") == 0)
			{// ���b�N�I������
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fLengthLockOn);
			}

			if (strcmp(cTemp, "MELEE_DIST") == 0)
			{// �ːi����i���Ɉڂ鋗��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fDistMelee);
			}

			if (strcmp(cTemp, "DECREASE_PARAM") == 0)
			{// �p�����[�^�S�񕜂ɂ����鎞��
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fTimeRegenParam);
			}

			if (strcmp(cTemp, "SIZE_HITEFFECT") == 0)
			{// �i���q�b�g�G�t�F�N�g�̃T�C�Y
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSizeHitEffect);
			}

			if (strcmp(cTemp, "QUAKESET") == 0)
			{
				while (true)
				{
					SDamageQuake m_quake;

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LINE") == 0)
					{// �h��̂������l
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_quake.fLineMin);
						(void)fscanf(pFile, "%f", &m_quake.fLineMax);
					}

					if (strcmp(cTemp, "POW") == 0)
					{// �h��̋���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_quake.fPowQuake);
					}

					if (strcmp(cTemp, "FRAME") == 0)
					{// �h��̒���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_quake.nFrameQuake);
					}

					if (strcmp(cTemp, "END_QUAKESET") == 0)
					{
						m_listDamageQuake.push_back(m_quake);

						break;
					}
				}
			}

			if (strcmp(cTemp, "NUM_THRUSTER") == 0)
			{// �X���X�^���Ǎ�
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nNumThruster);

				if (m_info.pThruster == nullptr)
				{
					m_info.pThruster = new SInfoThruster[m_info.nNumThruster];

					if (m_info.pThruster != nullptr)
					{
						ZeroMemory(m_info.pThruster, sizeof(SInfoThruster) * m_info.nNumThruster);
					}
				}
			}

			if (strcmp(cTemp, "THRUSTERSET") == 0)
			{
				while (true)
				{
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARENT") == 0)
					{// �e�ԍ�
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nIdxParent);
					}

					if (strcmp(cTemp, "OFFSET") == 0)
					{// �I�t�Z�b�g�ʒu
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.z);
					}

					if (strcmp(cTemp, "VECTOR") == 0)
					{// �x�N�g��
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.z);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// �T�C�Y
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.y);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// ����
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nRot);
					}

					if (strcmp(cTemp, "END_THRUSTER") == 0)
					{
						if (m_info.pThruster[nThruster].pFire == nullptr)
						{
							m_info.pThruster[nThruster].pFire = CBoostEffect::Create();
						
							if (m_info.pThruster[nThruster].pFire != nullptr)
							{
								m_info.pThruster[nThruster].pFire->SetRadius(m_info.pThruster[nThruster].size.x);
								m_info.pThruster[nThruster].pFire->SetHeight(m_info.pThruster[nThruster].size.y);
							}
						}

						nThruster++;

						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	Object::DeleteObject((CObject**)&m_info.pCollisionSphere);
	Object::DeleteObject((CObject**)&m_info.pCollisionCube);
	Object::DeleteObject((CObject**)&m_info.pClsnAttack);
	Object::DeleteObject((CObject**)&m_info.apHeatUI[0],PARAM_MAX);
	Object::DeleteObject((CObject**)&m_info.pOrbitWeapon);

	if (m_info.pThruster != nullptr)
	{// �X���X�^�[���̔j��
		for (int i = 0; i < m_info.nNumThruster; i++)
		{
			Object::DeleteObject((CObject**)&m_info.pThruster[i].pFire);
		}
	}

	for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
	{// �R���g���[���[�̔j��
		DeleteController((TYPE_CONTROLLER)i);
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �R���g���[���[����
//=====================================================
void CPlayer::CreateController(TYPE_CONTROLLER type)
{
	if (m_info.apController[type] != nullptr)
	{// ���ɃR���g���[���[������ꍇ�͈�x�폜����
		delete m_info.apController[type];
		m_info.apController[type] = nullptr;
	}

	// �R���g���[���[�̐���
	switch (type)
	{
	case CPlayer::TYPE_MOVE:
		m_info.apController[type] = new CPlayerControllerMove;
		break;
	case CPlayer::TYPE_CAMERA:
		m_info.apController[type] = new CPlayerControllerCamera;
		break;
	case CPlayer::TYPE_ATTACK:
		m_info.apController[type] = new CPlayerControllerAttack;
		break;
	default:
		assert(("�v���C���[�R���g���[���[�������ɕs���Ȏ�ނ����͂���Ă��܂�", false));
		break;
	}
}

//=====================================================
// �R���g���[���[�폜
//=====================================================
void CPlayer::DeleteController(TYPE_CONTROLLER type)
{
	if (m_info.apController[type] != nullptr)
	{
		delete m_info.apController[type];
		m_info.apController[type] = nullptr;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	CSlow *pSlow = CSlow::GetInstance();

	// ���b�N�I��
	Lockon();

	if (GetMotion() != MOTION_APPER)
	{
		// ����
		Input();

		// �v���C���[�̉�]
		Rotation();
	}

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
	
	if (nMotion != MOTION_SHOT && 
		nMotion != MOTION_ASSAULT && 
		nMotion != MOTION_MELEE && 
		nMotion != MOTION_MELEE2 &&
		nMotion != MOTION_GRAB &&
		nMotion != MOTION_THROW &&
		nMotion != MOTION_DODGE &&
		m_fragMotion.bStamp == false)
	{
		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			move.x += (0 - move.x) * m_param.fFactMove * fScale;
			move.z += (0 - move.z) * m_param.fFactMove * fScale;

			move.y -= m_param.fGravity * fScale;
		}
		else
		{
			float fScale = pSlow->GetScale();

			move.x *= m_param.fFactMove;
			move.z *= m_param.fFactMove;

			move.y -= m_param.fGravity * fScale;
		}
	}
	else
	{
		if (nMotion == MOTION_DODGE)
		{
			move.x += (0 - move.x) * 0.02f;
			move.y += (0 - move.y) * 0.5f;
			move.z += (0 - move.z) * 0.02f;
		}
		else
		{
			move.x += (0 - move.x) * 0.05f;
			move.y += (0 - move.y) * 0.1f;
			move.z += (0 - move.z) * 0.05f;
		}
	}

	if (move.y > 24.0f)
	{// �㏸�͐���
		move.y = 24.0f;
	}

	SetMove(move);

	// �����蔻��̊Ǘ�
	ManageCollision();

	// ��ԊǗ�
	ManageState();

	// ���[�V�����Ǘ�
	ManageMotion();

	// �p�����[�^�[�Ǘ�
	ManageParam();

	if (m_info.pEnemyGrab != nullptr)
	{// ��ɒǏ]
		D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
		D3DXMATRIX mtxParent;
		D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

		universal::SetOffSet(&mtxParent, mtxPart, offset);

		CDebugProc::GetInstance()->Print("\n�݈͂ʒu[%f,%f,%f]", mtxParent._41, mtxParent._42, mtxParent._43);

		m_info.pEnemyGrab->SetMatrix(mtxParent);
	}

	// �󒆂̏ꍇ�A�u�[�X�g�̉񕜗ʂ�������
	if (m_info.bLand)
	{
		AddBoost(m_param.fRegenBoost);
	}
	else
	{
		AddBoost(m_param.fRegenBoost * 0.2f);
	}
	
	if (nMotion == MOTION_WALK_FRONT)
	{// �����ɍ������o��
		D3DXVECTOR3 posToe = GetMtxPos(11);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);

		posToe = GetMtxPos(14);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);
	}

	// �u�[�X�g�G�t�F�N�g����
	Boost();

	// �p���N���X�̍X�V
	CMotion::Update();

// �f�o�b�O����
#if _DEBUG
	Debug();
#endif // _DEBUG
}

//=====================================================
// ���b�N�I��
//=====================================================
void CPlayer::Lockon(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	// �G�����b�N�I��
	pEnemyManager->Lockon(m_info.pEnemyGrab);
}

//=====================================================
// ���͏���
//=====================================================
void CPlayer::Input(void)
{
	for (CPlayerController *pController : m_info.apController)
	{// �e�R���g���[���[�̍X�V
		if (pController != nullptr)
		{
			pController->Update(this);
		}
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CInputManager *pInputManager = CInputManager::GetInstance();

		if (pInputManager != nullptr)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
			{
				CPause::Create();
			}
		}
	}
}

//=====================================================
// ���݂�
//=====================================================
void CPlayer::Stamp(void)
{
	if (m_info.pClsnAttack == nullptr || m_fragMotion.bStamp)
	{// ����̃G���[
		return;
	}

	D3DXVECTOR3 pos = GetPosition();

	m_info.pClsnAttack->SetRadius(100.0f);
	m_info.pClsnAttack->SetPosition(pos);

	if (m_info.pClsnAttack->IsTriggerEnter(CCollision::TAG::TAG_ENEMY))
	{// �ΏۂƂ̓����蔻��
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{// ���߂�G���`�F�b�N
			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager == nullptr)
			{
				return;
			}

			std::list<CEnemy*> listEnemy = pEnemyManager->GetList();

			for (auto pEnemy : listEnemy)
			{
				if ((CObject*)pEnemy == pObj)
				{
					bool bStamp = pEnemy->IsStamp();

					if (bStamp)
					{
						m_fragMotion.bStamp = true;

						D3DXVECTOR3 move = GetMove();

						move *= 0.0;

						SetMove(move);

						// ���������I�u�W�F�N�g�̃q�b�g����
						pObj->Hit(5.0f);
					}
				}
			}
		}
	}
}

//=====================================================
// ���b�N���Ă�G�̐؂�ւ�
//=====================================================
void CPlayer::SwitchLockEnemy(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pInputManager == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_RIGHT))
	{// �E����
		pEnemyManager->SwitchTarget(1, 0,m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_LEFT))
	{// ������
		pEnemyManager->SwitchTarget(-1, 0, m_info.pEnemyGrab);
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_UP))
	{// �����
		pEnemyManager->SwitchTarget(0, 1, m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_DOWN))
	{// ������
		pEnemyManager->SwitchTarget(0, -1, m_info.pEnemyGrab);
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

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT || nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2 || nMotion == MOTION_THROW)
	{
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager == nullptr)
		{
			return;
		}

		CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

		if (pEnemyLockon == nullptr)
		{
			// �J�����擾
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera == nullptr)
			{
				return;
			}

			CCamera::Camera *pInfoCamera = pCamera->GetCamera();

			// �����̕␳
			D3DXVECTOR3 rot = GetRotation();
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			rotCamera.x -= D3DX_PI * 0.5f;
			rotCamera.y;

			universal::LimitRot(&rotCamera.x);
			universal::LimitRot(&rotCamera.y);

			universal::FactingRot(&m_info.rotDest.x, -rotCamera.x, 0.15f);
			universal::FactingRot(&m_info.rotDest.y, rotCamera.y, 0.15f);
		}
	}
	else
	{
		if (fLenghtMove >= 6.0f)
		{
			// �����̕␳
			D3DXVECTOR3 rot = GetRotation();

			SetRotation(rot);

			if (m_info.bLand)
			{
				m_fragMotion.bMove = true;
			}
		}
		else
		{
			m_fragMotion.bMove = false;
		}

		m_info.rotDest.x = 0.0f;
	}

	// �����̕␳
	D3DXVECTOR3 rot = GetRotation();

	float fFact = 0.1f;

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT)
	{
		fFact = 0.4f;
	}

	universal::FactingRot(&rot.y, m_info.rotDest.y + D3DX_PI, fFact);
	universal::FactingRot(&rot.x, m_info.rotDest.x, fFact);

	SetRotation(rot);
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
		bool bLandMesh = false;
		bool bLandBlock = false;
		int nMotion = GetMotion();

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetMtxPos(0);
		D3DXVECTOR3 move = GetMove();

		// �G�Ƃ̐ڐG����
		m_info.pCollisionSphere->SetPosition(posWaist + move);

		if (m_info.pCollisionCube != nullptr)
		{
			pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();

			// ����̒Ǐ]
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// �u���b�N�Ƃ̉����o������
			bLandBlock = m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			// ���b�V���t�B�[���h�Ƃ̓����蔻��
			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			CMeshField *pMesh = CMeshField::GetInstance();

			if (pMesh != nullptr)
			{
				float fHeight = pMesh->GetHeight(pos,&move);

				if (pos.y <= fHeight)
				{
					pos.y = fHeight;
					move.y = 0.0f;

					SetPosition(pos);

					bool bFinish = IsFinish();

					bLandMesh = true;
				}
			}

			SetMove(move);
		}

		m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_ENEMY);

		if (pos.y > LIMIT_HEIGHT)
		{
			pos.y = LIMIT_HEIGHT;
		}

		SetPosition(pos);

		m_info.bLand = bLandMesh || bLandBlock;
		m_fragMotion.bAir = !m_info.bLand;

		if (m_info.bLand)
		{
			if (nMotion == MOTION_AIR || nMotion == MOTION_FALL)
			{
				m_fragMotion.bJump = false;
				m_fragMotion.bAir = false;
				m_fragMotion.bFall = false;
			}
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

	if (nMotion == MOTION_APPER)
	{
		if (bFinish)
		{
			SetMotion(MOTION_NEUTRAL);

			Camera::ChangeBehavior(new CFollowPlayer);

			CUIManager *pUIManager = CUIManager::GetInstance();

			if (pUIManager != nullptr)
			{
				pUIManager->EnableDisp(true);
			}
		}
	}
	else if (nMotion == MOTION_DEATH)
	{
		if (bFinish)
		{
			Death();
		}
	}
	else if (m_fragMotion.bDodge)
	{
		if (nMotion != MOTION_DODGE)
		{
			SetMotion(MOTION_DODGE);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bDodge = false;
			}
		}
	}
	else if (m_fragMotion.bStamp || nMotion == MOTION_STAMP)
	{// ���݂����[�V����
		if (nMotion != MOTION_STAMP)
		{
			SetMotion(MOTION_STAMP);
		}
		else
		{
			if (bFinish)
			{
				SetMotion(MOTION_AIR);
			}
		}
	}
	else if (nMotion == MOTION_THROW)
	{// �������[�V����
		if (bFinish)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bGrab)
	{// �͂݃��[�V����
		if (nMotion != MOTION_GRAB)
		{
			SetMotion(MOTION_GRAB);

			AddMoveForward(m_param.fPowGrab);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bGrab = false;
			}
		}
	}
	else if (nMotion == MOTION_MELEE2)
	{
		if (bFinish)
		{
			SetMotion(MOTION_AIR);

			EndMelee();
		}
	}
	else if (nMotion == MOTION_MELEE)
	{
		if (bFinish)
		{
			if (m_fragMotion.bAddAttack)
			{
				Sound::Play(CSound::LABEL_SE_DASH00);

				SetMotion(MOTION_MELEE2);
				m_fragMotion.bAddAttack = false;
				m_fragMotion.bMelee = false;
				m_info.bMelee = false;

				CEnemy *pEnemyLockon = GetLockOn();

				if (pEnemyLockon != nullptr)
				{
					D3DXVECTOR3 pos = GetPosition();
					D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

					if (universal::DistCmp(pos, posEnemy, m_param.fDistMelee, nullptr) == false)
					{
						Camera::ChangeBehavior(new CMoveCylinder);

						AddMoveForward(m_param.fPowAddMelee);
					}
				}

				m_info.aParam[PARAM_MELEE] += 0.9f;
			}
			else
			{
				SetMotion(MOTION_AIR);

				EndMelee();
			}
		}
	}
	else if (m_fragMotion.bMelee)
	{// �ߐڍU�����[�V����
		if (nMotion != MOTION_ASSAULT)
		{
			Sound::Play(CSound::LABEL_SE_DASH00);

			SetMotion(MOTION_ASSAULT);

			D3DXVECTOR3 move = GetMove();

			move *= 0.01f;

			SetMove(move);
		}
		else
		{
			StartMelee();
		}
	}
	else if (m_fragMotion.bShot)
	{// �ˌ����[�V����
		if (nMotion != MOTION_SHOT || (nMotion == MOTION_SHOT && bFinish))
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
	else if (m_fragMotion.bFall)
	{// �~�����[�V����
		if (nMotion != MOTION_FALL)
		{
			SetMotion(MOTION_FALL);
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
// �p�����[�^�Ǘ�
//=====================================================
void CPlayer::ManageParam(void)
{
	for (int i = 0; i < PARAM_MAX; i++)
	{
		if (m_info.aParam[i] > 1.0f)
		{
			m_info.aParam[i] = 1.0f;
			m_info.aHeat[i] = true;
		}

		float fDeltaTime = CManager::GetDeltaTime();

		m_info.aParam[i] -= 1.0f * (fDeltaTime / m_param.fTimeRegenParam);

		if (m_info.aParam[i] < 0.0f)
		{
			m_info.aParam[i] = 0.0f;
			m_info.aHeat[i] = false;
		}

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetParam(m_info.aParam[i]);
		}
	}
}

//=====================================================
// �u�[�X�g�G�t�F�N�g�̐���
//=====================================================
void CPlayer::Boost(void)
{
	int nMotion = GetMotion();

	MultiplyMtx();

	if (m_info.pThruster == nullptr || nMotion == MOTION_APPER)
		return;

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// �ڕW�����̐ݒ�
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	D3DXVECTOR3 move = GetMove();
	float fSpeed = sqrtf(axisMove.x * axisMove.x + axisMove.z * axisMove.z);

	universal::LimitValue(&fSpeed, 1.0f, 0.0f);

	for (int i = 0; i < m_info.nNumThruster; i++)
	{
		if (m_info.pThruster[i].pFire != nullptr)
		{
			int nParent = m_info.pThruster[i].nIdxParent;
			D3DXVECTOR3 offset = m_info.pThruster[i].offset;
			D3DXVECTOR3 vector = m_info.pThruster[i].vector;

			D3DXVECTOR3 posBoost;
			D3DXVECTOR3 vecBoost;

			D3DXMATRIX mtxParent = *GetParts(nParent)->pParts->GetMatrix();
			D3DXMATRIX mtx;
			D3DXMATRIX mtxVec;

			universal::SetOffSet(&mtx, mtxParent, offset);
			universal::SetOffSet(&mtxVec, mtx, vector);

			posBoost = { mtx._41, mtx._42 ,mtx._43 };
			vecBoost = { mtxVec._41 - posBoost.x,mtxVec._42 - posBoost.y,mtxVec._43 - posBoost.z };

			D3DXVECTOR3 rot = universal::VecToRot(vecBoost);
			rot.x *= -1;
			rot.x += D3DX_PI;

			float fHeight = m_info.pThruster[i].pFire->GetHeight();
			
			if (!m_info.bLand)
			{
				fSpeed *= 0.5f;
			}

			if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
			{
				fSpeed = 1.0f;
			}

			float fDest = m_info.pThruster[i].size.y * fSpeed;
			
			float fDiff = fDest - fHeight;

			float fFact = 0.2f;

			if (fDiff < 0.0f)
			{
				fFact = 0.8f;
			}

			fHeight += fDiff * fFact;

			m_info.pThruster[i].pFire->SetHeight(fHeight);
			m_info.pThruster[i].pFire->SetRotation(rot);
			m_info.pThruster[i].pFire->SetPosition(posBoost);
		}
	}

	// ����̋O��
	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{
		if (m_info.pOrbitWeapon == nullptr)
		{
			D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();
			D3DXVECTOR3 offset = { 0.0f,0.0f,-200.0f };

			m_info.pOrbitWeapon = COrbit::Create(mtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), offset, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);
		}

		if (m_info.pOrbitWeapon != nullptr)
		{
			D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();

			m_info.pOrbitWeapon->SetPositionOffset(mtx, 0);
		}
	}
	else
	{
		if (m_info.pOrbitWeapon != nullptr)
		{
			Object::DeleteObject((CObject**)&m_info.pOrbitWeapon);
		}
	}
}

//=====================================================
// �ߐڍU���̎n�܂�^�C�~���O
//=====================================================
void CPlayer::StartMelee(void)
{
	bool bFinish = IsFinish();
	bool bNear = false;

	CEnemy *pEnemyLockon = GetLockOn();

	if (pEnemyLockon != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

		if (universal::DistCmp(pos, posEnemy, m_param.fDistMelee, nullptr))
		{
			Camera::ChangeBehavior(new CMoveCylinder);

			bNear = true;

			// �ړ����~�߂�
			D3DXVECTOR3 move = GetMove();

			move *= 0.7f;

			SetMove(move);
		}
	}

	if (bFinish || bNear)
	{// �����𖞂�������i���U���Ɉڍs
		m_fragMotion.bMelee = false;

		SetMotion(MOTION_MELEE);

		m_info.rotDest.x = 0.0f;

		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			pEnemyManager->EnableLockTarget(true);
		}

		m_info.aParam[PARAM_MELEE] += 0.7f;
	}
}

//=====================================================
// �O���Ɉړ��ʂ�������
//=====================================================
void CPlayer::AddMoveForward(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

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
// ��Ɉړ��ʂ�������
//=====================================================
void CPlayer::AddMoveUp(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };

	vecMove.y += fSpeed;

	move += vecMove;

	SetMove(move);
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == MOTION_APPER)
	{// �o�����̉�
		Sound::Play(CSound::LABEL_SE_LAND00);

		D3DXVECTOR3 posParticle = GetPosition();

		CParticle::Create(posParticle, CParticle::TYPE::TYPE_APPER_SMOKE);
	}

	if (nMotion == MOTION_DODGE)
	{// ���
		D3DXMATRIX *pMtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

		universal::SetOffSet(pMtxPart, *pMtxPart,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));

		CInpact::Create(0.1f, pMtxPart);
	}

	if (nMotion == MOTION_SHOT)
	{// �e�𔭎�
		Shot(pos);
	}

	if (nMotion == MOTION_JUMP)
	{// �W�����v
		D3DXVECTOR3 move = GetMove();

		move.y += m_param.fPowJump;

		SetMove(move);
	}

	if (nMotion == MOTION_STAMP)
	{// ���݂�
		AddMoveUp(m_param.fPowStamp);

		AddMoveStamp();

		m_fragMotion.bStamp = false;
	}

	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{// �ߐڍU��
		ManageAttack(pos,300.0f);
	}

	if (nMotion == MOTION_GRAB)
	{// �͂�
		// �G��͂�
		if (m_info.pClsnAttack == nullptr)
		{// ����̃G���[
			return;
		}

		m_info.pClsnAttack->SetPosition(pos);
		m_info.pClsnAttack->SetRadius(m_param.fRadiusGrab);


		if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
		{// �ΏۂƂ̓����蔻��
			CObject *pObj = m_info.pClsnAttack->GetOther();

			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager != nullptr)
			{// �G�`�F�b�N
				// �͂�
				CEnemy *pEnemyGrab = nullptr;

				std::list<CEnemy*> listEnemy = pEnemyManager->GetList();

				for (auto pEnemy : listEnemy)
				{
					CEnemy::TYPE type = pEnemy->GetType();

					if ((CObject*)pEnemy == pObj && type != CEnemy::TYPE_BOSS)
					{
						pEnemyGrab = pEnemy;

						pEnemyGrab->DeleteCollision();
						pEnemyGrab->EnableIndependent(true);
						SetMotion(MOTION_THROW);

						// �X���[�ɂ���
						CSlow *pSlow = CSlow::GetInstance();

						if (pSlow != nullptr)
						{
							pSlow->SetScale(0.1f);
						}

						m_fragMotion.bGrab = false;
					}
				}

				// �͂ޓG�̌���
				m_info.pEnemyGrab = pEnemyGrab;

				if (m_info.pEnemyGrab != nullptr)
				{// �͂�ł���G���烍�b�N���O��
					CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

					if (pEnemyManager != nullptr)
					{
						pEnemyManager->SetEnemyLock(nullptr);
					}
				}
			}
		}
	}

	if (nMotion == MOTION_THROW)
	{// �͂񂾓G�𓊂���΂�
		if (m_info.pEnemyGrab != nullptr)
		{
			Sound::Play(CSound::LABEL_SE_IMPACT00);
			Sound::Play(CSound::LABEL_SE_IMPACT01);

			m_info.pEnemyGrab->EnableIndependent(false);

			D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
			D3DXMATRIX mtxParent;
			D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

			universal::SetOffSet(&mtxParent, mtxPart, offset);

			D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42, mtxParent._43 };

			m_info.pEnemyGrab->SetPosition(pos);
			m_info.pEnemyGrab->CreateCollision();
			m_info.pEnemyGrab->SetState(CEnemy::STATE::STATE_THROWN);

			// ���������Ɉړ��ʂ�ݒ�
			D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
			D3DXVECTOR3 rot = GetRotation();

			vecMove =
			{
				sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * m_param.fPowThrow,
				cosf(rot.x - D3DX_PI * 0.5f) * m_param.fPowThrow,
				sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * m_param.fPowThrow
			};

			m_info.pEnemyGrab->SetMove(vecMove);

			m_info.pEnemyGrab = nullptr;

			// �X���[�ɂ���
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}

//=====================================================
// �ˌ�
//=====================================================
void CPlayer::Shot(D3DXVECTOR3 posMazzle)
{
	Sound::Play(CSound::LABEL_SE_SHOT00);

	D3DXVECTOR3 rot = GetRotation();

	D3DXVECTOR3 move =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * m_param.fSpeedBullet,
		cosf(rot.x - D3DX_PI * 0.5f) * m_param.fSpeedBullet,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * m_param.fSpeedBullet
	};

	CEnemyManager *pEnemymanager = CEnemyManager::GetInstance();

	if (pEnemymanager != nullptr)
	{
		CEnemy *pEnemyLock = pEnemymanager->GetLockon();

		if (pEnemyLock != nullptr)
		{
			D3DXVECTOR3 posEnemy = pEnemyLock->GetMtxPos(0);
			D3DXVECTOR3 moveEnemy = pEnemyLock->GetMove();

			D3DXVECTOR3 posPridiction = universal::LinePridiction(posMazzle, m_param.fSpeedBullet, posEnemy, moveEnemy);

			posPridiction +=
			{
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
			};

			D3DXVECTOR3 vecDiff = posEnemy - posMazzle;

			universal::VecConvertLength(&vecDiff, m_param.fSpeedBullet);

			move = vecDiff;
		}
	}

	CBullet *pBullet = CBullet::Create(posMazzle, move, 5, CBullet::TYPE_PLAYER, false, 40.0f, m_param.fDamageBullet,
		D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

	// �M�ʂ����Z
	m_info.aParam[PARAM_GUN] += 0.1f;

	// �G�t�F�N�g����
	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim = pAnimManager->CreateEffect(posMazzle, CAnimEffect3D::TYPE_MUZZLEFLUSH);

		if (pAnim != nullptr)
		{
			pAnim->EnableZtest(false);
		}
	}
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
	{// �ΏۂƂ̓����蔻��
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr && !m_info.bMelee)
		{
			Sound::Play(CSound::LABEL_SE_HIT01);

			// �_���[�W�G�t�F�N�g�̐���
			CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

			if (pAnim3D != nullptr)
			{
				CAnim3D *pEffect = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);

				if (pEffect != nullptr)
				{
					pEffect->SetSize(SIZE_HIT_EFFECT, SIZE_HIT_EFFECT);
					pEffect->EnableZtest(true);
				}
			}

			// �q�b�g�X�g�b�v
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetSlowTime(0.2f, 0.01f);
			}

			// �J�����h��
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				pCamera->SetQuake(1.01f, 1.01f, 10);
			}

			m_info.pClsnAttack->DamageAll(CCollision::TAG::TAG_ENEMY, 7.5f);
		}

		m_info.bMelee = true;
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
	if (m_info.state != STATE_DEATH)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// ���S����
			Sound::Play(CSound::LABEL_SE_WARNING00);

			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			SetMotion(MOTION::MOTION_DEATH);

			for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
			{// �R���g���[���[�̍폜
				DeleteController((TYPE_CONTROLLER)i);
			}
		}
		else
		{// �_���[�W����
			m_info.state = STATE_DAMAGE;

			// �J�����h��
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				for (SDamageQuake quake : m_listDamageQuake)
				{
					if (fDamage >= quake.fLineMin && fDamage < quake.fLineMax)
						pCamera->SetQuake(quake.fPowQuake, quake.fPowQuake, quake.nFrameQuake);
				}
			}
		}
	}
}

//=====================================================
// ���b�N�I���̓G�擾
//=====================================================
CEnemy *CPlayer::GetLockOn(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

	return pEnemyLockon;
}

//=====================================================
// �i���̏I��
//=====================================================
void CPlayer::EndMelee(void)
{
	if (m_info.pOrbitWeapon != nullptr)
	{
		D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();

		m_info.pOrbitWeapon->SetEnd(true);

		m_info.pOrbitWeapon = nullptr;
	}

	Camera::ChangeBehavior(new CLookEnemy);

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->EnableLockTarget(false);
	}

	m_info.bMelee = false;
}

//=====================================================
// �^�[�Q�b�g���b�N�̐؂�ւ�
//=====================================================
void CPlayer::ToggleLockTarget(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	pEnemyManager->EnableLockTarget(m_info.bLockTarget);
}

//=====================================================
// �u�[�X�g���Z
//=====================================================
void CPlayer::AddBoost(float fValue)
{
	m_info.fBoost += fValue;

	if (m_info.fBoost < 0.0f)
	{// �I�[�o�[�q�[�g����
		m_info.stateBoost = STATEBOOST_OVERHEAT;
	}

	if (m_info.stateBoost == STATEBOOST_OVERHEAT)
	{// �I�[�o�[�q�[�g����̕��A
		float fRate = m_info.fBoost / m_param.fInitialBoost;

		if (fRate > 0.5f)
		{
			m_info.stateBoost = STATEBOOST_NORMAL;
		}
	}

	universal::LimitValue(&m_info.fBoost, m_param.fInitialBoost, 0.0f);
}

//=====================================================
// ���݂��̈ړ��ʉ��Z
//=====================================================
void CPlayer::AddMoveStamp(void)
{
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

	CInputManager::SAxis axis = pInputManager->GetAxis();

	axis.axisMove;

	float fLengthInput = sqrtf(axis.axisMove.x * axis.axisMove.x + axis.axisMove.y * axis.axisMove.y);

	if (fLengthInput >= 0.3f)
	{
		float fAngleInput = atan2f(axis.axisMove.x, axis.axisMove.y);

		D3DXVECTOR3 move = GetMove();

		move +=
		{
			sinf(pInfoCamera->rot.y + fAngleInput) * m_param.fSpeedStamp,
			0.0f,
			cosf(pInfoCamera->rot.y + fAngleInput) * m_param.fSpeedStamp,
		};

		SetMove(move);
	}
}

//=====================================================
// ���S����
//=====================================================
void CPlayer::Death(void)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Stop(CSound::LABEL_SE_WARNING00);
	}

	Sound::Play(CSound::LABEL_SE_EXPLOSION01);

	D3DXVECTOR3 pos = GetMtxPos(0);

	// �G�t�F�N�g����
	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim = pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE_EXPLOSION);

		if (pAnim != nullptr)
		{
			pAnim->SetSize(600.0f, 600.0f);
		}
	}

	// �j�А���
	CDebrisSpawner::Create(pos, CDebrisSpawner::TYPE::TYPE_DEATH);
	
	Uninit();

	CContinue::Create();
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
	pDebugProc->Print("\n�ڕW�̌���[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);
	pDebugProc->Print("\n�u�[�X�g�c��[%f]", m_info.fBoost);
	pDebugProc->Print("\n�̗�[%f]", m_info.fLife);
	for (int i = 0; i < PARAM_MAX; i++)
		pDebugProc->Print("\n�M��[%f][%d]", m_info.aParam[i],m_info.aHeat[i]);

	int nMotion = GetMotion();

	pDebugProc->Print("\n���[�V����[%d]", nMotion);
	pDebugProc->Print("\n�^�[�Q�b�g���b�N[%d]", m_info.bLockTarget);
	pDebugProc->Print("\n���n[%d]", m_info.bLand);
	pDebugProc->Print("\n��[%d]", m_fragMotion.bAir);

	CInputKeyboard *pKey = CInputKeyboard::GetInstance();

	if (pKey != nullptr)
	{
		if (pKey->GetTrigger(DIK_O))
		{
			m_info.fLife = 1.0f;
		}
	}
}