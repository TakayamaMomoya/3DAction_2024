//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "missile.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "universal.h"
#include "debugproc.h"
#include "particle.h"
#include "collision.h"
#include "orbit.h"
#include "anim3D.h"
#include "animEffect3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MAX_SPEED = 100.0f;	// �ړ����x
const float MAX_ACCL = 8.0f;	// �ő�����x
const float CHASE_SPEED = 3.0f;	// �ǐՑ��x
const int INITIAL_LIFE = 1;	// �����̗�
const int DEATH_TIME = 240;	// ���ł܂ł̎���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CMissile::CMissile()
{
	m_fTimerHit = 0.0f;
	m_nDeathTimer = 0;
	m_bEndChase = false;
	m_fChaseSpeed = 0.0f;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// ��������
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// ����������
		pMissile->Init();
	}

	return pMissile;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMissile::Init(void)
{
	m_fChaseSpeed = MAX_ACCL;

	Load("data\\MOTION\\motionMissileSmall.txt");

	// �p���N���X�̏�����
	CEnemy::Init();

	// �����蔻����~�T�C���ɕύX
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		pCollision->SetTag(CCollision::TAG::TAG_ROCKET);
	}

	m_fTimerHit = 5.0f;

	return S_OK;
}

//=====================================================
// ���S����
//=====================================================
void CMissile::Death(void)
{
	D3DXVECTOR3 pos = GetPosition();

	// �A�j���G�t�F�N�g����
	CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

	if (pAnim3D != nullptr)
	{
		pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);
	}

	// ���g�̏I��
	Uninit();
}

//=====================================================
// �I������
//=====================================================
void CMissile::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMissile::Update(void)
{
	D3DXVECTOR3 vecMove;
	D3DXMATRIX mtx;

	// �ʒu�ۑ�
	SetPositionOld(GetPosition());

	// �ǐՏ���
	ChasePlayer();

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// �X���̐���
	Tilt();

	// �����蔻����~�T�C���ɕύX
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		float fRadius = pCollision->GetRadius();

		pCollision->SetRadius(fRadius * 1.4);
		
		if (pCollision->OnEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = pCollision->GetOther();

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(0.3f);

				Death();

				return;
			}
		}

		pCollision->SetRadius(fRadius);
	}

	CParticle::Create(GetPosition(), CParticle::TYPE::TYPE_MISSILE_SMOKE);

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �ǐՏ���
//=====================================================
void CMissile::ChasePlayer(void)
{
	if (m_bEndChase)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		// �ڕW�����̎擾
		D3DXVECTOR3 pos = GetMtxPos(0);
		D3DXVECTOR3 move = GetMove();

		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);
		D3DXVECTOR3 movePlayer = pPlayer->GetMove();

		posPlayer = universal::LinePridiction(pos, MAX_SPEED, posPlayer, movePlayer * 0.1f);

		float fSpeed = D3DXVec3Length(&move);

		D3DXVECTOR3 vecDiff = posPlayer - pos;
		float fLengthDiff = D3DXVec3Length(&vecDiff);

		if (fLengthDiff < 1000.0f)
		{
			m_bEndChase = true;
		}

		float fDeltaTime = CManager::GetDeltaTime();

		m_fTimerHit -= fDeltaTime;

		if (m_fTimerHit > 0.0f)
		{
			D3DXVECTOR3 acceleration = 2.0f * (vecDiff - move);

			if (D3DXVec3Length(&acceleration) > m_fChaseSpeed)
			{
				D3DXVec3Normalize(&acceleration, &acceleration);

				acceleration *= m_fChaseSpeed;
			}

			// �ړ��ʂ𐳖ʂɑ���
			move += acceleration;

			if (D3DXVec3Length(&move) > MAX_SPEED)
			{
				D3DXVec3Normalize(&move, &move);

				move *= MAX_SPEED;
			}

			SetMove(move);
		}
	}
}

//=====================================================
// �X���̊Ǘ�
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y�����ʂ̊p�x���擾
	rot.y = atan2f(move.x, move.z);

	// Y�����ʂ̈ړ��ʂ��擾
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// �����ʂ̊p�x���擾
	rot.x = atan2f(fLength,move.y);

	// ��]�̐ݒ�
	SetRotation(rot);
}

//=====================================================
// �`�揈��
//=====================================================
void CMissile::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}

//=====================================================
// �n�ʂɓ��������Ƃ��̏���
//=====================================================
void CMissile::HitField(void)
{
	Death();
}