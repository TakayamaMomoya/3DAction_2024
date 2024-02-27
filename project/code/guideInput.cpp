//*****************************************************
//
// 操作ガイドの処理[guideInput.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "guideInput.h"
#include "texture.h"
#include "inputManager.h"
#include "UI.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GUIDE = { 90.0f,25.0f };	// ガイド表示のサイズ
const D3DXVECTOR3 POS_GUIDE = { SCREEN_WIDTH - SIZE_GUIDE.x - 10.0f,30.0f,0.0f };	// ガイド表示の始めの位置
const char* PATH_GUIDE[CGuideInput::INPUT_MAX] =
{// ガイド表示のテクスチャパス

};
const char* PATH_FRAME = "data\\TEXTURE\\UI\\isLock00.png";	// フレームのパス
}

//=====================================================
// コンストラクタ
//=====================================================
CGuideInput::CGuideInput(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aGuide[0], sizeof(m_aGuide));
}

//=====================================================
// デストラクタ
//=====================================================
CGuideInput::~CGuideInput()
{

}

//=====================================================
// 生成処理
//=====================================================
CGuideInput *CGuideInput::Create(void)
{
	CGuideInput *pGuideInput = nullptr;

	if (pGuideInput == nullptr)
	{
		pGuideInput = new CGuideInput;

		if (pGuideInput != nullptr)
		{
			pGuideInput->Init();
		}
	}

	return pGuideInput;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGuideInput::Init(void)
{
	int nNum = 0;

	for (SInfoGuide guide : m_aGuide)
	{
		guide.pFrame = CUI::Create();

		if (guide.pFrame != nullptr)
		{
			D3DXVECTOR3 pos = POS_GUIDE;

			pos.y += SIZE_GUIDE.y * nNum * 2;

			guide.pFrame->SetSize(SIZE_GUIDE.x, SIZE_GUIDE.y);
			guide.pFrame->SetPosition(pos);
			guide.pFrame->SetVtx();

			int nIdx = Texture::GetIdx(PATH_FRAME);
			guide.pFrame->SetIdxTexture(nIdx);
		}

		nNum++;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGuideInput::Uninit(void)
{
	for (SInfoGuide guide : m_aGuide)
	{
		Object::DeleteObject((CObject**)&guide.pFrame);
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CGuideInput::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CGuideInput::Draw(void)
{
	
}