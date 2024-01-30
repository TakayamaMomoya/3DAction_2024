//*****************************************************
//
// �{�X�G�̏���[enemyboss.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemyboss.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "bullet.h"
#include "effect3D.h"
#include "sound.h"
#include "frame.h"
#include "particle.h"
#include "stateEnemyBoss.h"
#include "beam.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float INITIAL_LIFE = 60.0f;	// �����̗�
const int INITIAL_SCORE = 30000;	// �����X�R�A
const int TIME_SHOT = 240;	// �ˌ��܂ł̃J�E���^�[
const float ROLL_FACT = 0.1f;	// ��]�W��
const float BULLET_SPEED = 2.0f;	// �e�̑��x
const float BULLET_SIZE = 2.5f;	// �e�̑傫��
const float GRAVITY = 0.3f;	// �d��
const int TIME_MISSILE = 20;	// �~�T�C�����˕p�x
const float MISSILE_UP = 3.0f;	// �~�T�C���̏���
const int NUM_MISSILE = 3;	// �~�T�C���̔��ː�
const float MOVE_FACT = 0.04f;	// �ړ��W��
const float LINE_END = 5.0f;	// �ړ��I���̂������l
const float MID_POINT = 2740.0f;	// �^�񒆂̒l
const float WIDTH_STAGE = 160.0f;	// �X�e�[�W�̕�
const int DAMAGE_FRAME = 10;	// �_���[�W��Ԃ̎���
const float FLOAT_HEIGTH = 180.0f;	// ����
const int SHOT_TIME = 3;	// �ˌ��̕p�x
const float SHOT_HEIGHT = 30.0f;	// �ˌ����̍��x
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CEnemyBoss *CEnemyBoss::m_pEnemyBoss = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyBoss::CEnemyBoss()
{
	// ���g�̏����N���A
	ZeroMemory(&m_info,sizeof(Sinfo));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyBoss::~CEnemyBoss()
{

}

//=====================================================
// ��������
//=====================================================
CEnemyBoss *CEnemyBoss::Create(void)
{
	if (m_pEnemyBoss == nullptr)
	{
		m_pEnemyBoss = new CEnemyBoss;

		if (m_pEnemyBoss != nullptr)
		{
			m_pEnemyBoss->Load("data\\MOTION\\robot01.txt");

			m_pEnemyBoss->Init();
		}
	}

	return m_pEnemyBoss;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyBoss::Init(void)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Stop();

		//pSound->Play(CSound::LABEL_BGM_BOSS);
	}

	// �t���[�����o�̐���
	CFrame::Create(100, 240, 70);

	// �p���N���X�̏�����
	CEnemy::Init();

	// �����̗̑͐ݒ�
	SetLife(INITIAL_LIFE);

	// ��Ԑݒ�
	ChangeState(new CStateBossApper);

	FollowCollision();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyBoss::Uninit(void)
{
	m_pEnemyBoss = nullptr;

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyBoss::Update(void)
{
	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	// �p���N���X�̍X�V
	CEnemy::Update();

	if (m_info.pState != nullptr)
	{// ��Ԃ��Ƃ̍s��
		m_info.pState->Attack(this);
		m_info.pState->Move(this);
	}

	// �����蔻��Ǘ�
	ManageCollision();
}

//=====================================================
// �����蔻��Ǘ�
//=====================================================
void CEnemyBoss::ManageCollision(void)
{
	// �����蔻��Ǐ]
	FollowCollision();
}

//=====================================================
// �C�x���g�̊Ǘ�
//=====================================================
void CEnemyBoss::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == MOTION_SHOT_UNDER)
	{
		// �r�[���̐���
		CBeam *pBeam = CBeam::Create();

		D3DXMATRIX mtxBeam;

		universal::SetOffSet(&mtxBeam, mtxParent, D3DXVECTOR3(0.0f, -1.0f, 1.0f));

		if (pBeam != nullptr)
		{
			D3DXVECTOR3 posMazzle = { mtxBeam._41,mtxBeam._42 ,mtxBeam._43 };
			D3DXVECTOR3 vecMazzle = posMazzle - pos;

			D3DXVECTOR3 rot = GetRot();

			pBeam->SetCol(D3DXCOLOR(0.6f, 0.6f, 1.0f, 1.0f));
			pBeam->SetMtx(mtxBeam);
			pBeam->SetRot(rot);
			pBeam->SetPosition(posMazzle);
			pBeam->SetAnimSize(1000.0f, 5000.0f);
			pBeam->SetShrink(5.0f);
			pBeam->SetExpand(80.0f);
			pBeam->SetExtend(100.0f);
		}
	}
}

//=====================================================
// �����蔻��̒Ǐ]
//=====================================================
void CEnemyBoss::FollowCollision(void)
{
	CCollisionSphere *pCollision = GetClsnSphere();

	if (pCollision != nullptr)
	{
		D3DXVECTOR3 pos = GetMtxPos(IDX_WAIST);

		pos.y -= 25.0f;

#ifdef _DEBUG
		//dCEffect3D::Create(pos, pCollision->GetRadius(), 10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif

		pCollision->SetPositionOld(pCollision->GetPosition());
		pCollision->SetPosition(pos);
		pCollision->SetRadius(35.0f);
	}
}

//=====================================================
// �ڕW�ʒu�ւ̈ړ�
//=====================================================
bool CEnemyBoss::FollowDest(void)
{
	bool bEnd = false;

	D3DXVECTOR3 pos = GetPosition();

	D3DXVECTOR3 vecDiff = m_info.posDest - pos;

	// ���������̎擾
	float fLength = D3DXVec3Length(&vecDiff);

	if (LINE_END > fLength)
	{
		bEnd = true;
	}

	vecDiff *= MOVE_FACT;

	pos += vecDiff;

	SetPosition(pos);

	return bEnd;
}

//=====================================================
// ��Ԃ̐؂�ւ�
//=====================================================
void CEnemyBoss::ChangeState(CStateBoss *pNext)
{
	if (pNext == nullptr)
		assert(("�{�X�̏�Ԃ�nullptr�œn����܂���",false));

	if (m_info.pState != nullptr)
	{
		delete m_info.pState;
		m_info.pState = nullptr;
	}

	m_info.pState = pNext;
	
	if (m_info.pState != nullptr)
	{
		m_info.pState->Init(this);
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CEnemyBoss::Hit(float fDamage)
{
	CEnemy::STATE state = CEnemy::GetState();

	if (state == CEnemy::STATE_NORMAL)
	{
		float fLife = CEnemy::GetLife();

		fLife -= fDamage;

		CSound *pSound = CSound::GetInstance();

		if (pSound != nullptr)
		{// �q�b�g�T�E���h
			//pSound->Play(CSound::LABEL_SE_HIT_BOSS);
		}

		if (fLife <= 0.0f)
		{// ���S���
			SetMotion(MOTION_DEATH);

			//CParticle::Create(GetMtxPos(0), CParticle::TYPE_FIRE);

			fLife = 0.0f;

			CGame::SetState(CGame::STATE_END);

			state = CEnemy::STATE_DEATH;

			// �X�R�A�Ǘ�
			ManageScore();

			// �����蔻��폜
			DeleteCollision();
		}
		else
		{
			state = CEnemy::STATE_DAMAGE;
		}

		CEnemy::SetLife(fLife);
	}

	CEnemy::SetState(state);
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyBoss::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();

#ifdef _DEBUG
	CDebugProc::GetInstance()->Print("\n�{�X�ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CDebugProc::GetInstance()->Print("\n�{�X�̗́F[%f]", GetLife());
	CDebugProc::GetInstance()->Print("\n�{�X��ԁF[%d]",CEnemy::GetState());
	CDebugProc::GetInstance()->Print("\n�{�X���[�V�����F[%d]", GetMotion());
	CDebugProc::GetInstance()->Print("\n�L�[�F[%d]", GetKey());
	CDebugProc::GetInstance()->Print("\n�t���[���F[%d]", GetFrame());
	CDebugProc::GetInstance()->Print("\nIsFinish�F[%d]", IsFinish());
#endif
}