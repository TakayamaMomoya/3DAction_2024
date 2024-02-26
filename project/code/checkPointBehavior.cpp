//*****************************************************
//
// �`�F�b�N�|�C���g��Behavior[checkpointBehavior.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "enemyManager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SIZE_CURSOR = 20.0f;	// �J�[�\���T�C�Y
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// �J�[�\���̃e�N�X�`��
const float DIST_PROGRESS = 1000.0f;	// �i�s���鋗��
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// �ړ��p�[�g
//=====================================================
CCheckPointMove::CCheckPointMove()
{// �R���X�g���N�^
	m_pCursor = nullptr;
}

CCheckPointMove::~CCheckPointMove()
{// �f�X�g���N�^

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// ������
	if (m_pCursor == nullptr)
	{// �J�[�\������
		m_pCursor = CUI::Create();

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}
}

void CCheckPointMove::Uninit(CCheckPointManager *pCheckPoint)
{// �I��
	Object::DeleteObject((CObject**)&m_pCursor);
}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// �X�V
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 posNext = pCheckPoint->GetCheckPosition(nProgress + 1);

	// �J�[�\���̕\��
	if (m_pCursor != nullptr)
	{
		D3DXMATRIX mtx;
		D3DXVECTOR3 posScreen;

		bool bInScreen = universal::IsInScreen(posNext, mtx, &posScreen);

		if (bInScreen == false)
		{
			// ��ʓ��ɓ���Ȃ��悤�ɐݒ�
			if (posScreen.x > -SIZE_CURSOR && posScreen.x < SCREEN_WIDTH + SIZE_CURSOR)
			{
				posScreen.x = -SIZE_CURSOR;
			}

			if (posScreen.y > -SIZE_CURSOR && posScreen.y < SCREEN_HEIGHT + SIZE_CURSOR)
			{
				posScreen.y = -SIZE_CURSOR;
			}
		}

		m_pCursor->SetPosition(posScreen);
		m_pCursor->SetVtx();
	}

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// �����̌v�Z
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posNext - posPlayer;

		float fDist = D3DXVec3Length(&vecDiff);

		if (fDist < DIST_PROGRESS)
		{// �i�s�󋵂̉��Z�A�퓬�ֈڍs
			pCheckPoint->ChangeBehavior(new CCheckPointBattle);
		}
	}
}

//=====================================================
// �퓬���
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// �R���X�g���N�^
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// �f�X�g���N�^

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// ������

}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// �I��
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// �X�V
	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy == 0)
	{
		pCheckPoint->AddProgress();
		pCheckPoint->ChangeBehavior(new CCheckPointMove);
	}
}