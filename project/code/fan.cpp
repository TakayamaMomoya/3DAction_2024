//*****************************************************
//
// 扇ポリゴンの処理[fan.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"
#include "UIManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
	const float INITIAL_RADIUS = 50.0f;	// 初期の半径
	const char* TEX_PATH = "data\\TEXTURE\\UI\\gauge.png";	// テクスチャのパス
}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFan::CFan(int nPriority) : CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pos = { 0,0,0 };
	m_rot = { 0,0,0 };
	m_fRadius = 0.0f;
	m_nNumVtx = 0;
	m_fRateAngle = 1.0f;
	m_fAngleMax = 0.0f;
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CFan::~CFan()
{

}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan::Init(void)
{
	// テクスチャ読み込み
	int nIdx = CTexture::GetInstance()->Regist(TEX_PATH);
	SetIdxTexture(nIdx);

	m_fRadius = INITIAL_RADIUS;
	m_fAngleMax = D3DX_PI * 2;
	m_pos = D3DXVECTOR3{ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };

	// 色初期化
	SetCol(m_col);

	// 頂点設定
	SetVtx();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自分自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CFan::Update(void)
{

}

//=====================================================
// 描画
//=====================================================
void CFan::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr && m_pVtxBuff != nullptr)
	{
		// テクスチャ設定
		int nIdxTexture = GetIdxTexture();
		LPDIRECT3DTEXTURE9 pTexture = CTexture::GetInstance()->GetAddress(nIdxTexture);
		pDevice->SetTexture(0, pTexture);

		// 描画
		int nNumVtx = GetNumVtx();
		pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, nNumVtx);
	}
}

//=====================================================
// 設定処理
//=====================================================
void CFan::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 向き設定処理
//=====================================================
void CFan::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;

	universal::LimitRot(&m_rot.x);
	universal::LimitRot(&m_rot.y);
	universal::LimitRot(&m_rot.z);
}

//=====================================================
// 色設定処理
//=====================================================
void CFan::SetCol(D3DXCOLOR col)
{
	m_col = col;

	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	if (m_pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nNumVtx + 2; nCnt++)
		{
			pVtx[nCnt].col = (D3DCOLOR)m_col;

			pVtx[nCnt].rhw = 1.0;
		}

		// 頂点バッファのアンロック
		m_pVtxBuff->Unlock();
	}
}