//*****************************************************
//
// UIマネージャー[UIManager.cpp]
// Author:基盤（髙山桃也）+ 酒井南勝
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "UIManager.h"
#include "UI.h"
#include "inputkeyboard.h"
#include "boost.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR3 POS_FRAME = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CUIManager *CUIManager::m_pUIManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CUIManager::CUIManager()
{
	m_bDisp = false;
	m_pFrame = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CUIManager::~CUIManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CUIManager *CUIManager::Create(void)
{
	if (m_pUIManager == nullptr)
	{// インスタンス生成
		m_pUIManager = new CUIManager;

		// 初期化処理
		m_pUIManager->Init();
	}

	return m_pUIManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CUIManager::Init(void)
{
	m_bDisp = true;

	// コックピット演出表示
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

	// ブースト表示の生成
	CBoost::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CUIManager::Uninit(void)
{
	m_pUIManager = nullptr;

	Release();
}

//=====================================================
// 更新処理
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
}

//=====================================================
// 描画処理
//=====================================================
void CUIManager::Draw(void)
{
#ifdef _DEBUG

#endif
}