//*****************************************************
//
// ビームブレードの処理[beamBlade.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "beamBlade.h"
#include "meshcylinder.h"
#include "object3D.h"
#include "texture.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// コンストラクタ
//=====================================================
CBeamBlade::CBeamBlade(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CBeamBlade::~CBeamBlade()
{

}

//=====================================================
// 生成処理
//=====================================================
CBeamBlade *CBeamBlade::Create(void)
{
	CBeamBlade *pBeamBlade = nullptr;

	if (pBeamBlade == nullptr)
	{
		pBeamBlade = new CBeamBlade;

		if (pBeamBlade != nullptr)
		{
			// 初期化
			pBeamBlade->Init();
		}
	}

	return pBeamBlade;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBeamBlade::Init(void)
{
	if (m_info.pCylinder == nullptr)
	{// シリンダーの生成
		m_info.pCylinder = CMeshCylinder::Create(16,6);

		if (m_info.pCylinder != nullptr)
		{
			m_info.pCylinder->SetHeight(70.0f);
			m_info.pCylinder->SetRadius(80.0f);
			m_info.pCylinder->SetVtx();

			int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\energy00.png");

			m_info.pCylinder->SetIdxTexture(nIdx);
		}
	}

	if (m_info.pBlade == nullptr)
	{// ブレード部分の生成
		m_info.pBlade = CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		if (m_info.pBlade != nullptr)
		{
			m_info.pBlade->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_info.pBlade->SetRot(D3DXVECTOR3(-1.57f, 0.0f, 0.0f));
			m_info.pBlade->SetSize(100.0f, 400.0f);
			m_info.pBlade->SetPosition(D3DXVECTOR3(0.0f, 400.0f, 0.0f));

			int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\blade.png");

			m_info.pBlade->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBeamBlade::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBeamBlade::Update(void)
{
	// シリンダーを丸くする
		// 情報取得
	CMeshCylinder *pMesh = m_info.pCylinder;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_info.pCylinder->GetVtxBuff();

	if (pMesh == nullptr || pVtxBuff == nullptr)
	{
		return;
	}

	CMeshCylinder::MeshCylinder *pInfoMesh = pMesh->GetMeshCylinder();

	int nMeshU = pInfoMesh->nMeshU;
	int nMeshV = pInfoMesh->nMeshV;
	float fRadius = pInfoMesh->fRadius;
	float fHeight = pInfoMesh->fHeight;

	//頂点情報のポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fRot;
	float fSin = D3DX_PI / nMeshV;

	for (int nCountV = 0; nCountV < nMeshV + 1; nCountV++)
	{//頂点座標の設定
		for (int nCountU = 0; nCountU < nMeshU + 1; nCountU++)
		{
			//角度算出
			fRot = nCountU * (D3DX_PI / nMeshU) * 2;
			
			float fAdd = 20.0f * sinf(fSin * nCountV);

			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.x = sinf(fRot) * (fRadius + fAdd);
			pVtx[nCountV * (nMeshU + 1) + nCountU].pos.z = cosf(fRot) * (fRadius + fAdd);
		}
	}

	//頂点バッファをアンロック
	pVtxBuff->Unlock();
}

//=====================================================
// 描画処理
//=====================================================
void CBeamBlade::Draw(void)
{

}