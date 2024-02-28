//*****************************************************
//
// チュートリアル処理[tutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorial.h"
#include "object.h"
#include "player.h"
#include "slow.h"
#include "manager.h"
#include "camera.h"
#include "cameraBehavior.h"
#include "UIManager.h"
#include "UI.h"
#include "enemyManager.h"
#include "meshfield.h"
#include "limit.h"
#include "object3D.h"
#include "renderer.h"
#include "texture.h"
#include "animEffect3D.h"
#include "guideInput.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_SPAWN = 3.0f;	// スポーンまでの間隔
const D3DXVECTOR3 POS_SKIP = { SCREEN_WIDTH * 0.9f,SCREEN_HEIGHT * 0.85f, 0.0f };	// スキップ表示の位置
const D3DXVECTOR2 SIZE_SKIP = { 80.0f,60.0f };	// スキップ表示のサイズ
const D3DXVECTOR2 SIZE_GAUGE = { 40.0f,20.0f };	// スキップゲージのサイズ
const char* PATH_SKIP = "data\\TEXTURE\\UI\\tutorial06.png";	// スキップ表示のパス
}

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial()
{
	m_fSpawnCnt = 0.0f;
	m_pSkipGauge = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// 初期化処理
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
		int nIdx = Texture::GetIdx("data\\TEXTURE\\MATERIAL\\concrete.jpg");
		pMesh->SetIdxTexture(nIdx);
	}

	D3DXVECTOR3 aPos[4] =
	{
		{0.0f,0.0f,10000.0f},
		{15000.0f,0.0f,0.0f},
		{0.0f,0.0f,-10000.0f},
		{-3000.0f,0.0f,0.0f},
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

	// フォグをかける
	CRenderer *pRenderer = CRenderer::GetInstance();

	if (pRenderer != nullptr)
	{
		pRenderer->EnableFog(false);
	}

	// 初期敵スポーン
	SpawnEnemy();

	// スキップのキャプション
	CUI *pCaption = CUI::Create();

	if (pCaption != nullptr)
	{
		D3DXVECTOR3 pos = POS_SKIP;

		pCaption->SetSize(SIZE_SKIP.x, SIZE_SKIP.y);
		pCaption->SetPosition(pos);
		pCaption->SetVtx();

		int nIdx = Texture::GetIdx(PATH_SKIP);
		pCaption->SetIdxTexture(nIdx);
	}

	// スキップのゲージ
	if (m_pSkipGauge == nullptr)
	{
		m_pSkipGauge = CUI::Create();

		if (m_pSkipGauge != nullptr)
		{
			D3DXVECTOR3 pos = POS_SKIP;

			m_pSkipGauge->SetSize(SIZE_SKIP.x, SIZE_SKIP.y);
			m_pSkipGauge->SetPosition(pos);
			m_pSkipGauge->SetVtx();

			int nIdx = Texture::GetIdx(PATH_SKIP);
			m_pSkipGauge->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	if (m_pSkipGauge != nullptr)
	{
		m_pSkipGauge->Uninit();
		m_pSkipGauge = nullptr;
	}

	// オブジェクト全棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorial::Update(void)
{
	// シーンの更新
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
		float fDeltaTime = CManager::GetDeltaTime();

		m_fSpawnCnt += fDeltaTime;

		if (m_fSpawnCnt >= TIME_SPAWN)
		{
			SpawnEnemy();

			m_fSpawnCnt = 0.0f;
		}
	}

	SetCursorPos((int)SCRN_MID.x, (int)SCRN_MID.y);
}

//=====================================================
// 敵のスポーン
//=====================================================
void CTutorial::SpawnEnemy(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
		return;

	D3DXVECTOR3 aPosEnemy[] =
	{
		// 爆弾練習
		{ 3000.0f, 1000.0f, 0.0f },
		{ 6500.0f, 1000.0f, 500.0f },
		{ 6500.0f, 1000.0f, 0.0f },
		{ 6500.0f, 1000.0f, -500.0f },
		{ 6500.0f, 1500.0f, -250.0f },
		{ 6500.0f, 1500.0f, 250.0f },

		// 通常敵
		{ 10000.0f, 1000.0f, 4000.0f },
		{ 10000.0f, 1000.0f, 3000.0f },
		{ 10000.0f, 1000.0f, 2000.0f },
		{ 10000.0f, 1000.0f, 1000.0f },
		{ 10000.0f, 1000.0f, 0.0f },
		{ 10000.0f, 1000.0f, -4000.0f },
		{ 10000.0f, 1000.0f, -3000.0f },
		{ 10000.0f, 1000.0f, -2000.0f },
		{ 10000.0f, 1000.0f, -1000.0f },
	};

	for (int i = 0; i < sizeof(aPosEnemy) / sizeof(D3DXVECTOR3); i++)
	{
		CEnemy::TYPE type = CEnemy::TYPE::TYPE_TUTORIAL;

		if (i % 10 == 0)
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
// 描画処理
//=====================================================
void CTutorial::Draw(void)
{

}