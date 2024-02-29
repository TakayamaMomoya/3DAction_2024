//*****************************************************
//
// UI�}�l�[�W���[[UIManager.cpp]
// Author:��Ձi���R����j+ ����쏟
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "UIManager.h"
#include "UI.h"
#include "inputkeyboard.h"
#include "boost.h"
#include "life.h"
#include "texture.h"
#include "player.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_FRAME = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
const D3DXCOLOR COL_NORMAL = { 1.0f,1.0f,1.0f,1.0f };
const D3DXCOLOR COL_DAMAGE = { 1.0f,0.0f,0.0f,1.0f };
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CUIManager *CUIManager::m_pUIManager = nullptr;	// ���g�̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CUIManager::CUIManager()
{
	m_bDisp = false;
	m_pFrame = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CUIManager::~CUIManager()
{

}

//=====================================================
// ��������
//=====================================================
CUIManager *CUIManager::Create(void)
{
	if (m_pUIManager == nullptr)
	{// �C���X�^���X����
		m_pUIManager = new CUIManager;

		// ����������
		m_pUIManager->Init();
	}

	return m_pUIManager;
}

//=====================================================
// ����������
//=====================================================
HRESULT CUIManager::Init(void)
{
	m_bDisp = true;

	// �R�b�N�s�b�g���o�\��
	m_pFrame = CUI::Create();

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
		m_pFrame->SetPosition(POS_FRAME);
		m_pFrame->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\UI\\Frame03.png");
		m_pFrame->SetIdxTexture(nIdx);
		m_pFrame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	}

	CBoost::Create();
	CLife::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CUIManager::Uninit(void)
{
	m_pUIManager = nullptr;

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CUIManager::Update(void)
{
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_1))
		{
			m_bDisp = m_bDisp ? false : true;
		}
	}

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr && m_pFrame != nullptr)
	{
		float fLifeInitial = pPlayer->GetParam().fInitialLife;
		float fLife = pPlayer->GetLife();

		float fRate = fLife / fLifeInitial;

		D3DXCOLOR colDIff = COL_DAMAGE - COL_NORMAL;

		D3DXCOLOR col = COL_NORMAL + colDIff * (1.0f - fRate);

		col.a = 0.4f;

		m_pFrame->SetCol(col);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CUIManager::Draw(void)
{
#ifdef _DEBUG

#endif
}