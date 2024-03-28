//*****************************************************
//
// 3D円の処理[fan3D.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "fan3D.h"
#include "renderer.h"
#include "texture.h"
#include "universal.h"
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CFan3D::CFan3D(int nPriority) : CFan(nPriority)
{
	m_bBillboard = false;
	ZeroMemory(&m_mtxWorld, sizeof(D3DXMATRIX));
}

//=====================================================
// デストラクタ
//=====================================================
CFan3D::~CFan3D()
{

}

//=====================================================
// 生成処理
//=====================================================
CFan3D *CFan3D::Create(int nPriority, int nNumVtx)
{
	CFan3D *pFan3D = nullptr;

	// インスタンス生成
	pFan3D = new CFan3D(nPriority);

	if (pFan3D != nullptr)
	{
		pFan3D->SetNumVtx(nNumVtx);

		// 初期化処理
		pFan3D->Init();
	}

	return pFan3D;
}

//=====================================================
// 初期化
//=====================================================
HRESULT CFan3D::Init(void)
{
	CFan::Init();

	m_bBillboard = true;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	if (pDevice != nullptr)
	{
		LPDIRECT3DVERTEXBUFFER9 *pVtxBuff = GetVtxBuff();
		int nNumVtx = GetNumVtx();

		if (*pVtxBuff == nullptr)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVtx + 2),
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				pVtxBuff,
				nullptr);
		}
	}

	// 頂点設定
	SetVtx();

	EnableLighting(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CFan3D::Uninit(void)
{
	CFan::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CFan3D::Update(void)
{
	SetVtx();
}

//=====================================================
// 頂点位置設定
//=====================================================
void CFan3D::SetVtx(void)
{
	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();
	int nNumVtx = GetNumVtx();
	float fAngleMax = GetAngleMax();
	float fRadius = GetRadius();

	if (pVtxBuff != nullptr)
	{
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 中心の頂点の設定
		pVtx[0].pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pVtx[0].tex = D3DXVECTOR2{ 0.5f,0.5f };

		for (int i = 1;i < nNumVtx + 2;i++)
		{// 円周の頂点の設定
			float fAngle = (6.28f * fAngleMax) * ((float)(i - 1) / (float)nNumVtx);

			universal::LimitRot(&fAngle);

			pVtx[i].pos =
			{
				sinf(fAngle) * fRadius,
				cosf(fAngle) * fRadius,
				0.0f,
			};

			D3DXVECTOR2 tex =
			{
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
void CFan3D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = *GetVtxBuff();

	if (pDevice != nullptr && pVtxBuff != nullptr)
	{
		//ワールドマトリックス初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		if (m_bBillboard)
		{// ビルボード描画
			DrawBillboard();
		}
		else
		{// 通常の描画
			DrawNormal();
		}

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ポリゴンの描画
		CFan::Draw();
	}
}

//=====================================================
// 通常描画
//=====================================================
void CFan3D::DrawNormal(void)
{
	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//=====================================================
// ビルボード描画
//=====================================================
void CFan3D::DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();

	//ビューマトリックス取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに向ける
	D3DXMatrixInverse(&m_mtxWorld, nullptr, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}