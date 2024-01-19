//*****************************************************
//
// 敵マネージャー[enemyManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "manager.h"
#include "enemyManager.h"
#include "enemyNormal.h"
#include "enemyBoss.h"
#include "inputkeyboard.h"
#include "effect3D.h"
#include "player.h"
#include "object2D.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float TIME_SPAWN = 5.0f;	// 敵のスポーン
const float DIST_LOCKON = 5000.0f;	// ロックオン距離
const float SIZE_CURSOR = 60.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\lockon01.png";	// カーソルのテクスチャ
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CEnemyManager *CEnemyManager::m_pEnemyManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CEnemyManager::CEnemyManager()
{
	m_pEnemyLockon = nullptr;
	m_bLockTarget = false;
	m_fTimer = 0.0f;
	m_pHead = nullptr;
	m_pTail = nullptr;
	m_pEnemyLockon = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyManager::~CEnemyManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CEnemyManager *CEnemyManager::Create(void)
{
	if (m_pEnemyManager == nullptr)
	{// インスタンス生成
		m_pEnemyManager = new CEnemyManager;

		// 初期化処理
		m_pEnemyManager->Init();
	}

	return m_pEnemyManager;
}

//=====================================================
// 敵の生成処理
//=====================================================
CEnemy *CEnemyManager::CreateEnemy(D3DXVECTOR3 pos, CEnemy::TYPE type)
{
	char *apPath[CEnemy::TYPE_MAX] = 
	{
		nullptr,
		"data\\MOTION\\motionHeli.txt",
		"data\\MOTION\\motionArms00.txt",
	};

	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// インスタンス生成
		switch (type)
		{
		case CEnemy::TYPE_NORMAL:

			pEnemy = new CEnemyNormal;

			break;
		default:
			break;
		}

		if (pEnemy != nullptr)
		{
			// 位置設定
			pEnemy->SetPosition(pos);

			// モーション読込
			pEnemy->Load(apPath[type]);

			// 初期化処理
			pEnemy->Init();
		}
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyManager::Init(void)
{
	// 読込処理
	Load();

	CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
	CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);

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
// 読込処理
//=====================================================
void CEnemyManager::Load(void)
{

}

//=====================================================
// 終了処理
//=====================================================
void CEnemyManager::Uninit(void)
{
	m_pEnemyManager = nullptr;

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
void CEnemyManager::Update(void)
{
#ifdef _DEBUG

	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_C))
		{
			CreateEnemy(D3DXVECTOR3(1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(0.0f, 500.0f, 500.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1000.0f, 150.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(-1200.0f, 400.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
			CreateEnemy(D3DXVECTOR3(1500.0f, 1000.0f, 0.0f), CEnemy::TYPE::TYPE_NORMAL);
		}
	}

	if(m_pEnemyLockon != nullptr)
		CEffect3D::Create(m_pEnemyLockon->GetPosition(), 30.0f, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

//=====================================================
// ロックオン処理
//=====================================================
CEnemy *CEnemyManager::Lockon(CEnemy *pEnemyExclusive)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return nullptr;
	}

	bool bLock = IsLockTarget();

	bool bInAny = false;
	CEnemy *pEnemy = GetHead();
	float fDistMax = FLT_MAX;
	D3DXVECTOR3 posCenter = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	D3DXVECTOR3 posScreen = posCenter;

	while (pEnemy != nullptr)
	{
		CEnemy::STATE state = pEnemy->GetState();

		CEnemy *pEnemyNext = pEnemy->GetNext();

		pEnemy->EnableLock(false);

		if (state != CEnemy::STATE::STATE_DEATH && pEnemyExclusive != pEnemy)
		{
			D3DXVECTOR3 pos = pEnemy->GetMtxPos(0);
			D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
			D3DXMATRIX mtx = *pEnemy->GetMatrix();

			D3DXVECTOR3 vecDiffPlayer = pos - posPlayer;
			float fDistPlayer = D3DXVec3Length(&vecDiffPlayer);

			// 距離制限
			if (DIST_LOCKON > fDistPlayer)
			{
				D3DXVECTOR3 posScreenTemp;

				// ロックオンする敵の決定
				if (universal::IsInScreen(pos, mtx, &posScreenTemp))
				{
					bInAny = true;

					pEnemy->EnableLock(true);
					pEnemy->SetPositionCursor(posScreenTemp);

					if (bLock == false)
					{
						D3DXVECTOR3 vecDiff = posScreenTemp - posCenter;

						// 画面中心からの距離を計算
						float fDist = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

						if (fDist < fDistMax)
						{
							posScreen = posScreenTemp;

							m_pEnemyLockon = pEnemy;

							fDistMax = fDist;
						}
					}
				}
			}
		}

		pEnemy = pEnemyNext;
	}

	D3DXVECTOR3 posDestCursor = posCenter;

	if (bInAny == false)
	{
		if (bLock == false)
		{
			m_pEnemyLockon = nullptr;
		}
	}
	else
	{
		posDestCursor = posScreen;
	}

	if (m_pCursor != nullptr)
	{// カーソルの位置補正
		D3DXVECTOR3 posCursor = m_pCursor->GetPosition();
		D3DXVECTOR3 vecDiffCursor = posDestCursor - posCursor;

		posCursor += vecDiffCursor * 0.1f;

		m_pCursor->SetPosition(posCursor);
		m_pCursor->SetVtx();
	}

	return m_pEnemyLockon;
}

//=====================================================
// ターゲットのロック
//=====================================================
void CEnemyManager::EnableLockTarget(bool bLock)
{
	m_bLockTarget = bLock;
}

//=====================================================
// 死亡した敵がロックオンしてるものと同じかどうか
//=====================================================
void CEnemyManager::CheckDeathLockon(CEnemy *pEnemy)
{
	if (pEnemy == m_pEnemyLockon)
	{
		m_pEnemyLockon = nullptr;
		EnableLockTarget(false);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyManager::Draw(void)
{
#ifdef _DEBUG
	//CDebugProc::GetInstance()->Print("\n敵の位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
#endif
}