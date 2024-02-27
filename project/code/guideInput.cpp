//*****************************************************
//
// ����K�C�h�̏���[guideInput.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "guideInput.h"
#include "texture.h"
#include "inputManager.h"
#include "UI.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const D3DXVECTOR2 SIZE_GUIDE = { 90.0f,25.0f };	// �K�C�h�\���̃T�C�Y
const D3DXVECTOR3 POS_GUIDE = { SCREEN_WIDTH - SIZE_GUIDE.x - 10.0f,30.0f,0.0f };	// �K�C�h�\���̎n�߂̈ʒu
const char* PATH_GUIDE[CGuideInput::INPUT_MAX] =
{// �K�C�h�\���̃e�N�X�`���p�X

};
const char* PATH_FRAME = "data\\TEXTURE\\UI\\isLock00.png";	// �t���[���̃p�X
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CGuideInput::CGuideInput(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_aGuide[0], sizeof(m_aGuide));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGuideInput::~CGuideInput()
{

}

//=====================================================
// ��������
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
// ����������
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
// �I������
//=====================================================
void CGuideInput::Uninit(void)
{
	for (SInfoGuide guide : m_aGuide)
	{
		Object::DeleteObject((CObject**)&guide.pFrame);
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CGuideInput::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CGuideInput::Draw(void)
{
	
}