//*****************************************************
//
// チェックポイントのBehavior[checkpointBehavior.h]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"
#include "UI.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "enemyManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SIZE_CURSOR = 20.0f;	// カーソルサイズ
const char* CURSOR_PATH = "data\\TEXTURE\\UI\\checkPoint00.png";	// カーソルのテクスチャ
const float DIST_PROGRESS = 1000.0f;	// 進行する距離
}

//=====================================================
// コンストラクタ
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// デストラクタ
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// 移動パート
//=====================================================
CCheckPointMove::CCheckPointMove()
{// コンストラクタ
	m_pCursor = nullptr;
}

CCheckPointMove::~CCheckPointMove()
{// デストラクタ

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// 初期化
	if (m_pCursor == nullptr)
	{// カーソル生成
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
{// 終了
	Object::DeleteObject((CObject**)&m_pCursor);
}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// 更新
	int nProgress = pCheckPoint->GetProgress();

	D3DXVECTOR3 posNext = pCheckPoint->GetCheckPosition(nProgress + 1);

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

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{// 距離の計算
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vecDiff = posNext - posPlayer;

		float fDist = D3DXVec3Length(&vecDiff);

		if (fDist < DIST_PROGRESS)
		{// 進行状況の加算、戦闘へ移行
			pCheckPoint->ChangeBehavior(new CCheckPointBattle);
		}
	}
}

//=====================================================
// 戦闘状態
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// コンストラクタ
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// デストラクタ

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// 初期化

}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// 終了
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// 更新
	int nNumEnemy = CEnemy::GetNumAll();

	if (nNumEnemy == 0)
	{
		pCheckPoint->AddProgress();
		pCheckPoint->ChangeBehavior(new CCheckPointMove);
	}
}