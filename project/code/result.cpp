//*****************************************************
//
// ���U���g�̏���[result.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "result.h"
#include "fade.h"
#include "inputManager.h"
#include "object.h"
#include "meshcylinder.h"
#include "texture.h"
#include "camera.h"
#include "manager.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const float SPEED_ROTATION = 0.003f;	// ���X�s�[�h
const float HEIGHT_CYLINDER = 800.0f;	// �V�����_�[�̍���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CResult::CResult()
{
	m_pCylinder = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CResult::~CResult()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CResult::Init(void)
{
	m_pCylinder = CMeshCylinder::Create();

	if (m_pCylinder != nullptr)
	{
		m_pCylinder->SetRadius(1500.0f);
		m_pCylinder->SetHeight(HEIGHT_CYLINDER);
		m_pCylinder->SetVtx();

		int nIdx = Texture::GetIdx("data\\TEXTURE\\BG\\result.jpg");
		m_pCylinder->SetIdxTexture(nIdx);
	}

	// �J�����ʒu�̐ݒ�
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
		return E_FAIL;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	pInfoCamera->posV = { 0.0f,HEIGHT_CYLINDER * 0.5f,0.0f };
	pInfoCamera->posR = { 0.0f,HEIGHT_CYLINDER * 0.5f,1.0f };

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CResult::Uninit(void)
{
	if (m_pCylinder != nullptr)
	{
		m_pCylinder->Uninit();
		m_pCylinder = nullptr;
	}

	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CResult::Update(void)
{
	// �t�F�[�h���鑀��
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager != nullptr)
	{
		if (pInputManager->GetTrigger(CInputManager::BUTTON_ENTER))
		{
			CFade *pFade = CFade::GetInstance();

			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_TITLE);
			}
		}
	}

	if (m_pCylinder != nullptr)
	{
		D3DXVECTOR3 rot = m_pCylinder->GetRotation();

		rot.y += SPEED_ROTATION;

		universal::LimitRot(&rot.y);

		m_pCylinder->SetRotation(rot);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CResult::Draw(void)
{

}