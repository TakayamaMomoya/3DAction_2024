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

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float MAX_SPEED = 30.0f;	// �ړ����x
const float CHASE_SPEED = 3.0f;	// �ǐՑ��x
const int INITIAL_LIFE = 1;	// �����̗�
const int DEATH_TIME = 240;	// ���ł܂ł̎���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CMissile::CMissile()
{
	m_nDeathTimer = 0;
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
	Load("data\\MOTION\\motionMissileSmall.txt");

	// �p���N���X�̏�����
	CEnemy::Init();

	// �����蔻����~�T�C���ɕύX
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		pCollision->SetTag(CCollision::TAG::TAG_ROCKET);
	}

	return S_OK;
}

//=====================================================
// ���S����
//=====================================================
void CMissile::Death(void)
{
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

	// ���x����
	vecMove = GetMove();

	if (D3DXVec3Length(&vecMove) > MAX_SPEED)
	{
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecMove *= MAX_SPEED;

		SetMove(vecMove);
	}

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
		if (pCollision->IsTriggerEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = pCollision->GetOther();

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(0.3f);

				Death();
			}
		}
	}

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �ǐՏ���
//=====================================================
void CMissile::ChasePlayer(void)
{
	// ���擾
	CPlayer *pPlayer = CPlayer::GetInstance();

	// �v�Z�p�ϐ�
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (pPlayer == nullptr)
	{// ���S
		Uninit();

		return;
	}

	pos = pPlayer->GetMtxPos(0);

	// �������擾
	vecDiff = pos - GetPosition();

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= CHASE_SPEED;

	// �ړ��ʐݒ�
	SetMove(GetMove() + vecDiff);
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
	SetRot(rot);
}

//=====================================================
// �`�揈��
//=====================================================
void CMissile::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}