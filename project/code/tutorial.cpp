//*****************************************************
//
// �`���[�g���A������[tutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorial.h"
#include "object.h"
#include "player.h"
#include "slow.h"
#include "manager.h"
#include "camera.h"
#include "cameraBehavior.h"
#include "UIManager.h"
#include "enemyManager.h"
#include "meshfield.h"
#include "limit.h"
#include "object3D.h"
#include "renderer.h"
#include "texture.h"
#include "animEffect3D.h"
#include "guideInput.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{

}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial()
{

}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorial::Init(void)
{
	CAnimEffect3D::Create();

	CSlow::Create();

	CPlayer::Create();

	CUIManager::Create();

	CGuideInput::Create();

	CEnemyManager::Create();

	Camera::ChangeBehavior(new CFollowPlayer);

	CMeshField *pMesh = CMeshField::Create();

	if (pMesh != nullptr)
	{
		pMesh->Reset();
	}

	D3DXVECTOR3 aPos[4] =
	{
		{0.0f,0.0f,15000.0f},
		{22000.0f,0.0f,0.0f},
		{0.0f,0.0f,-15000.0f},
		{-7000.0f,0.0f,0.0f},
	};
	D3DXVECTOR3 aRot[4] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,D3DX_PI * 0.5f,0.0f},
		{0.0f,D3DX_PI,0.0f},
		{0.0f,-D3DX_PI * 0.5f,0.0f},
	};

	for (int i = 0; i < 4; i++)
	{
		CLimit *pLimit = CLimit::Create();

		if (pLimit != nullptr)
		{
			pLimit->SetPosition(aPos[i]);
			pLimit->SetRotation(aRot[i]);

			CObject3D *pWall = CObject3D::Create(aPos[i]);

			if (pWall != nullptr)
			{
				aRot[i].x = -D3DX_PI * 0.5f;
				pWall->SetPosition(aPos[i] * 1.001f);
				pWall->SetRotation(aRot[i]);
				pWall->SetSize(30000.0f, 30000.0f);
				pWall->SetVtx();

				D3DXVECTOR2 lu = { 0.0f,0.0f };
				D3DXVECTOR2 rd = { 20.0f,20.0f };
				pWall->SetTex(rd, lu);

				int nIdx = Texture::GetIdx("data\\TEXTURE\\MATERIAL\\metal000.jpg");
				pWall->SetIdxTexture(nIdx);
			}
		}
	}

	// �t�H�O��������
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorial::Update(void)
{
	// �V�[���̍X�V
	CScene::Update();

	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	pCamera->Update();
	pCamera->Quake();

	pCamera->MoveDist(0.3f);

	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy == 0)
	{
		SpawnEnemy();
	}

	SetCursorPos((int)SCRN_MID.x, (int)SCRN_MID.y);
}

//=====================================================
// �G�̃X�|�[��
//=====================================================
void CTutorial::SpawnEnemy(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
		return;

	D3DXVECTOR3 aPosEnemy[] =
	{
		{0.0f,1000.0f,0.0f},
		{1000.0f,1000.0f,0.0f},
		{-1000.0f,1000.0f,0.0f},
	};

	for (int i = 0; i < sizeof(aPosEnemy) / sizeof(D3DXVECTOR3); i++)
	{
		CEnemy::TYPE type = CEnemy::TYPE::TYPE_NORMAL;

		if (i % 3 == 0)
		{
			type = CEnemy::TYPE::TYPE_BOMB;
		}

		CEnemy *pEnemy = pEnemyManager->CreateEnemy(aPosEnemy[i], type);

		if (pEnemy != nullptr)
		{
			pEnemy->SetPosDest(aPosEnemy[i]);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{

}