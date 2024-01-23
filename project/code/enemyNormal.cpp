//*****************************************************
//
// �ʏ�G�̏���[enemyNormal.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "enemyNormal.h"
#include "player.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyNormal::CEnemyNormal()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// �p���N���X�̏�����
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyNormal::Update(void)
{
	// �p���N���X�̍X�V
	CEnemy::Update();

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// ���S���[�V����
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}

	CPlayer *pPlayer = CPlayer::GetInstance();
	D3DXVECTOR3 rot = GetRot();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(0);

		// �����p�x�̎擾
		D3DXVECTOR3 vecDiff = posPlayer - pos;

		D3DXVECTOR3 rotDest = universal::VecToRot(vecDiff);

		universal::FactingRot(&rot.x, -rotDest.x + D3DX_PI * 0.5f, 0.1f);
		universal::FactingRot(&rot.y, rotDest.y, 0.1f);

		SetRot(rot);
	}
}

//=====================================================
// �ڕW�̒ǐ�
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyNormal::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}