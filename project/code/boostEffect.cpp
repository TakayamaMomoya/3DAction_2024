//*****************************************************
//
// ビームブレードの処理[BoostEffect.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "boostEffect.h"
#include "meshcylinder.h"
#include "object3D.h"
#include "texture.h"
#include "anim3D.h"
#include "animEffect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int MESH_V = 6;	// メッシュの縦の分割数
}

//=====================================================
// コンストラクタ
//=====================================================
CBoostEffect::CBoostEffect(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_info, sizeof(SInfo));
}

//=====================================================
// デストラクタ
//=====================================================
CBoostEffect::~CBoostEffect()
{

}

//=====================================================
// 生成処理
//=====================================================
CBoostEffect *CBoostEffect::Create(void)
{
	CBoostEffect *pBoostEffect = nullptr;

	if (pBoostEffect == nullptr)
	{
		pBoostEffect = new CBoostEffect;

		if (pBoostEffect != nullptr)
		{
			// 初期化
			pBoostEffect->Init();
		}
	}

	return pBoostEffect;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBoostEffect::Init(void)
{
	m_info.fRadius = 100.0f;
	m_info.fHeight = 400.0f;
	m_info.col = { 1.0f,1.0f,1.0f,1.0f };

	if (m_info.pBlade == nullptr)
	{// ブレード部分の生成
		CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

		if(pAnim3D != nullptr)
			m_info.pBlade = pAnim3D->CreateEffect(D3DXVECTOR3(0.0f,0.0f,0.0f),CAnimEffect3D::TYPE::TYPE_BOOST);

		if (m_info.pBlade != nullptr)
		{
			m_info.pBlade->SetMode(CObject3D::MODE::MODE_STRETCHBILLBOARD);
			m_info.pBlade->SetRotation(D3DXVECTOR3(-1.57f, 0.0f, 0.0f));
			m_info.pBlade->SetSize(m_info.fRadius, m_info.fHeight);
			m_info.pBlade->SetPosition(D3DXVECTOR3(0.0f, m_info.fHeight, 0.0f));
			m_info.pBlade->EnableAdd(true);
		}
	}

	if (m_info.pCylinder == nullptr)
	{// シリンダーの生成
		m_info.pCylinder = CMeshCylinder::Create(16, MESH_V);

		if (m_info.pCylinder != nullptr)
		{
			m_info.pCylinder->SetHeight(m_info.fHeight / MESH_V * 1.2f);
			m_info.pCylinder->SetRadius(m_info.fRadius * 0.6f);
			m_info.pCylinder->SetVtx();
			m_info.pCylinder->EnableAdd(true);

			int nIdx = Texture::GetIdx("data\\TEXTURE\\EFFECT\\energy00.png");

			m_info.pCylinder->SetIdxTexture(nIdx);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBoostEffect::Uninit(void)
{
	if (m_info.pBlade != nullptr)
	{
		m_info.pBlade->Uninit();
		m_info.pBlade = nullptr;
	}

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->Uninit();
		m_info.pCylinder = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBoostEffect::Update(void)
{
	// シリンダーを丸くする
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

	// 見た目の追従
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetPosition(pos);
		m_info.pCylinder->SetRotation(rot);
	}

	if (m_info.pBlade != nullptr)
	{
		pos +=
		{
			sinf(rot.x) * sinf(rot.y) * m_info.fHeight,
				cosf(rot.x) * m_info.fHeight,
				sinf(rot.x) * cosf(rot.y) * m_info.fHeight
		};

		rot.x -= 1.57f;

		m_info.pBlade->SetPosition(pos);
		m_info.pBlade->SetRotation(rot);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBoostEffect::Draw(void)
{

}

// 半径設定
void CBoostEffect::SetRadius(float fRadius)
{
	m_info.fRadius = fRadius;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetRadius(fRadius * 0.6f);
	}

	if (m_info.pBlade != nullptr)
	{
		float fHeight = m_info.pBlade->GetHeight();

		m_info.pBlade->SetSize(fRadius, fHeight);
		m_info.pBlade->SetVtx();
	}
}

// 高さ設定
void CBoostEffect::SetHeight(float fHeight)
{
	m_info.fHeight = fHeight;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetHeight(fHeight / MESH_V * 1.2f);
	}

	if (m_info.pBlade != nullptr)
	{
		float fWidth = m_info.pBlade->GetWidth();

		m_info.pBlade->SetSize(fWidth, fHeight);
	}
}

// 色設定
void CBoostEffect::SetColor(D3DXCOLOR col)
{
	m_info.col = col;

	if (m_info.pCylinder != nullptr)
	{
		m_info.pCylinder->SetCol(col);
	}

	if (m_info.pBlade != nullptr)
	{
		m_info.pBlade->SetColor(col);
	}
}