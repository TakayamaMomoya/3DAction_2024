//*****************************************************
//
// チェックポイント管理の処理[checkPointmanager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "checkPointManager.h"
#include "object2D.h"
#include "texture.h"
#include "player.h"
#include "fade.h"
#include "manager.h"
#include "enemyBoss.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* FILE_PATH = "data\\TEXT\\checkPoint.txt";	// ファイルのパス
const float SIZE_CURSOR = 20.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// カーソルのテクスチャ
const float DIST_PROGRESS = 1000.0f;	// 進行する距離
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CCheckPointManager *CCheckPointManager::m_pCheckPointManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CCheckPointManager::CCheckPointManager()
{
	m_nProgress = 0;
	m_nNumCheckPoint = 0;
	m_pPosCheckPoint = nullptr;
	m_pCursor = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CCheckPointManager::~CCheckPointManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CCheckPointManager *CCheckPointManager::Create(void)
{
	if (m_pCheckPointManager == nullptr)
	{
		m_pCheckPointManager = new CCheckPointManager;

		if (m_pCheckPointManager != nullptr)
		{
			m_pCheckPointManager->Init();
		}
	}

	return m_pCheckPointManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CCheckPointManager::Init(void)
{
	// 読込
	Load();

	// 進行状況取得


	if (m_pCursor == nullptr)
	{// カーソル生成
		m_pCursor = CObject2D::Create(7);

		if (m_pCursor != nullptr)
		{
			m_pCursor->SetSize(SIZE_CURSOR, SIZE_CURSOR);
			m_pCursor->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
			int nIdx = CTexture::GetInstance()->Regist(CURSOR_PATH);
			m_pCursor->SetIdxTexture(nIdx);
			m_pCursor->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CCheckPointManager::Load(void)
{
	// チェックポイント情報読込
	LoadInfoCheckPoint();
}

//=====================================================
// チェックポイント情報読込
//=====================================================
void CCheckPointManager::LoadInfoCheckPoint(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntPos = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(FILE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_CHECKPOINT") == 0)
			{// チェックポイント数読込
				fscanf(pFile, "%s", &cTemp[0]);

				fscanf(pFile, "%d", &m_nNumCheckPoint);

				if (m_pPosCheckPoint == nullptr)
				{// 判定情報の生成
					m_pPosCheckPoint = new D3DXVECTOR3[m_nNumCheckPoint];

					for (int i = 0; i < m_nNumCheckPoint; i++)
					{// 情報のクリア
						m_pPosCheckPoint[i] = { 0.0f,0.0f,0.0f };
					}
				}
				else
				{
					break;
				}

				if (m_pPosCheckPoint == nullptr)
				{// 生成できなかった場合
					break;
				}

				while (true)
				{
					// 文字読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "CHECKPOINTSET") == 0)
					{// パラメーター読込開始
						while (true)
						{
							// 文字読み込み
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "POS") == 0)
							{//位置読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								for (int i = 0; i < 3; i++)
								{
									fscanf(pFile, "%f", &m_pPosCheckPoint[nCntPos][i]);
								}
							}

							if (strcmp(cTemp, "END_CHECKPOINTSET") == 0)
							{// パラメーター読込終了
								nCntPos++;

								break;
							}
						}
					}

					if (m_nNumCheckPoint <= nCntPos)
					{
						break;
					}
				}
			}
			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CCheckPointManager::Uninit(void)
{
	m_pCheckPointManager = nullptr;

	if (m_pPosCheckPoint != nullptr)
	{
		delete[] m_pPosCheckPoint;
		m_pPosCheckPoint = nullptr;
	}

	if (m_pCursor != nullptr)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CCheckPointManager::Update(void)
{
	if (m_nProgress >= m_nNumCheckPoint - 1)
	{
		CFade *pFade = CFade::GetInstance();

		if (pFade != nullptr)
		{
			CFade::FADE state = pFade->GetState();

			if (state == CFade::FADE::FADE_OUT)
			{// ボス敵の生成
				CEnemyBoss *pEnemyBoss = CEnemyBoss::Create();
			}
		}

		return;
	}

	D3DXVECTOR3 posNext = m_pPosCheckPoint[m_nProgress + 1];

	// カーソルの表示
	if (m_pCursor != nullptr)
	{
		D3DXMATRIX mtx;
		D3DXVECTOR3 posScreen;

		bool bInScreen = universal::IsInScreen(posNext, mtx, &posScreen);
		
		if (bInScreen == false)
		{
			// 画面内に入らないように設定
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

	// チェックポイント位置の更新
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// 距離から計算
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posNext - posPlayer;

		float fDist = D3DXVec3Length(&vecDiff);

		if (fDist < DIST_PROGRESS)
		{
			m_nProgress++;

			if (m_nProgress >= m_nNumCheckPoint - 1)
			{// 最後のチェックポイントに到着
				CFade *pFade = CFade::GetInstance();

				if (pFade != nullptr)
				{
					pFade->SetFade(CScene::MODE_RANKING,false);

					if (m_pCursor != nullptr)
					{
						m_pCursor->Uninit();
						m_pCursor = nullptr;
					}
				}
			}
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CCheckPointManager::Draw(void)
{

}