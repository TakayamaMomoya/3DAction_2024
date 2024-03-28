//*****************************************************
//
// 2D円の処理[fan2D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan2D.h"
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

}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFan2D::CFan2D(int nPriority) : CFan(nPriority)
{

}

//=====================================================
// デストラクタ
//=====================================================
CFan2D::~CFan2D()
{

}

//=====================================================
// 生成処理
//=====================================================
CFan2D *CFan2D::Create(int nPriority, int nNumVtx)
{
	CFan2D *pFan2D = nullptr;

	// インスタンス生成
	pFan2D = new CFan2D(nPriority);

	if (pFan2D != nullptr)
	{
		pFan2D->SetNumVtx(nNumVtx);

		// 初期化処理
		pFan2D->Init();
	}

	return pFan2D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan2D::Init(void)
{
	CFan::Init();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		LPDIRECT3DVERTEXBUFFER9 *pVtxBuff = GetVtxBuff();
		int nNumVtx = GetNumVtx();

		if (*pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// 頂点設定
	SetVtx();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan2D::Uninit(void)
{
	CFan::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CFan2D::Update(void)
{
	CFan::Update();
}

//=====================================================
// 頂点位置設定
//=====================================================
void CFan2D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_2D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pVtxBuff != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		
		// 中心の頂点の設定
		pVtx[0].pos = pos;
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };

		int nNumVtx = GetNumVtx();
		float fAngleMax = GetAngleMax();
		float fRadius = GetRadius();
		float fRateAngle = GetRateAngle();
		D3DXVECTOR3 rot = GetRotation();

		for (int i = 1; i < nNumVtx + 2; i++)
		{// 円周の頂点の設定
			// 頂点のある角度を計算
			float fAngle = (fAngleMax * fRateAngle) * ((float)(i - 1) / (float)nNumVtx) + rot.z;

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{// 頂点位置設定
				pos.x + sinf(fAngle) * fRadius,
				pos.y - cosf(fAngle) * fRadius,
				0.0f,
			};

			D3DXVECTOR2 tex =
			{// テクスチャ座標の中心を基準とするので0.5fずらす
				0.5f + sinf(fAngle) * 0.5f,
				0.5f - cosf(fAngle) * 0.5f,
			};

			pVtx[i].tex = tex;
		}

		// 頂点バッファのアンロック
		pVtxBuff->Unlock();
	}
}

//=====================================================
// 描画
//=====================================================
void CFan2D::Draw(void)
{
	bool bDisp = true;

	CUIManager *pUIManager = CUIManager::GetInstance();

	// 非表示化のチェック
	if (pUIManager != nullptr)
		bDisp = pUIManager->IsDisp();

	if (!bDisp)
		return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		CFan::Draw();
	}
}