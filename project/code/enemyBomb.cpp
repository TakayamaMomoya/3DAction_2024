//*****************************************************
//
// �ʏ�G�̏���[EnemyBomb.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "EnemyBomb.h"
#include "player.h"
#include "effect3D.h"
#include "debugproc.h"
#include "bullet.h"
#include "manager.h"
#include "slow.h"
#include "animEffect3D.h"
#include "anim3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float RADIUS_EXPLOSION = 500.0f;	// �����͈�
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyBomb::CEnemyBomb()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyBomb::~CEnemyBomb()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyBomb::Init(void)
{
	// �p���N���X�̏�����
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyBomb::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// ���S���̏���
//=====================================================
void CEnemyBomb::Death(void)
{
	CCollisionSphere *pCollision = GetClsnSphere();

	if (pCollision != nullptr)
	{// ���͂̓G�Ƀ_���[�W��^����
		pCollision->SetRadius(RADIUS_EXPLOSION);
		pCollision->DamageAll(CCollision::TAG_ENEMY, 15.0f);

		// �A�j���[�V�����G�t�F�N�g����
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if (pAnim3D != nullptr)
		{
			D3DXVECTOR3 pos = GetMtxPos(0);

			CAnim3D *pAnim = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_EXPLOSION);

			if (pAnim != nullptr)
			{
				pAnim->SetSize(RADIUS_EXPLOSION, RADIUS_EXPLOSION);
			}
		}
	}

	CEnemy::Death();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyBomb::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyBomb::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}