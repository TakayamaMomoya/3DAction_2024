//*****************************************************
//
// レーダーの処理[rader.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "rader.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"
#include "universal.h"
#include "enemyManager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define ADD_HEIGHT	(100.0f)	// 目標からの高さ
#define WAVE_ANGLE	(D3DX_PI * 0.1f)	// 波打つ範囲の角度
#define WAVE_HEIGHT	(10.0f)	// 波打つ高さ
#define WAVE_LENGTH	(100.0f)	// 波打つ範囲の距離
#define CHANGE_COl	(0.15f)	// 色の変わる割合
#define RAND_HEIGHT	(25.0f)	// 振れる幅
#define WAVE_SIZE	(0.04f)	// 波の大きさ
#define SHAKE_SIZE	(0.005f)	// 揺れの大きさ
#define WAVE_SPEED	(0.08f)	// 波の動く速度
#define MESH_LENGTH	(230.0f)	// メッシュの一辺の長さ
#define MESH_HEIGHT	(10.0f)	// メッシュの高さ

//=====================================================
// コンストラクタ
//=====================================================
CRader::CRader(void)
{
	m_pPosInitial = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CRader::~CRader()
{

}

//=====================================================
// 生成処理
//=====================================================
CRader *CRader::Create(void)
{
	CRader *pRader = new CRader;

	if (pRader != nullptr)
	{
		pRader->Init();
	}

	return pRader;
}

//=====================================================
//初期化処理
//=====================================================
HRESULT CRader::Init(void)
{
	SetNumMeshU(255);
	SetNumMeshV(1);

	// 継承クラスの初期化
	CMeshCylinder::Init();

	SetRadius(MESH_LENGTH);
	SetHeight(MESH_HEIGHT);
	SetVtx();

	// テクスチャをなくす
	SetIdxTexture(-1);

	// 初期位置情報生成
	if (m_pPosInitial == nullptr)
	{
		int nNumVtx = GetNumVtx();

		m_pPosInitial = new D3DXVECTOR3[nNumVtx];

		if (m_pPosInitial != nullptr)
		{
			// 情報初期化
			ZeroMemory(m_pPosInitial, sizeof(D3DXVECTOR3) * nNumVtx);

			// 初期位置代入
			// 頂点バッファ取得
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CMeshCylinder::GetVtxBuff();

			// 頂点情報のポインタ
			VERTEX_3D *pVtx;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
			{
				m_pPosInitial[nCnt] = pVtx[nCnt].pos;
			}

			// 頂点バッファをアンロック
			pVtxBuff->Unlock();

		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRader::Uninit(void)
{
	if (m_pPosInitial != nullptr)
	{// 位置情報の破棄
		delete m_pPosInitial;
		m_pPosInitial = nullptr;
	}

	// 継承クラスの終了
	CMeshCylinder::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CRader::Update(void)
{
	// 継承クラスの更新
	CMeshCylinder::Update();

	// 目標についていく
	ChaseTarget();

	// 波形に動く処理
	Wave();
}

//=====================================================
// 目標についていく
//=====================================================
void CRader::ChaseTarget(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
	{
		return;
	}

	// 位置の取得
	D3DXVECTOR3 pos = pPlayer->GetPosition();

	pos.y += ADD_HEIGHT;

	SetPosition(pos);
}

//=====================================================
// 波形に動く処理
//=====================================================
void CRader::Wave(void)
{
	// 頂点バッファ取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CMeshCylinder::GetVtxBuff();

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	// 計算用変数
	int nIdxStart = 0;
	int nCntEnd = 0;
	int nIdx;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 pos = pPlayer->GetPosition();
	D3DXVECTOR3 *pPosDest = nullptr;
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	float fRot;
	float fRotDiff;
	float fSin;

	// 頂点数分の目的座標生成
	pPosDest = new D3DXVECTOR3[GetNumVtx()];

	if (pPosDest == nullptr)
	{// 生成できなかった場合処理を終了
		return;
	}

	MeshCylinder mesh = *GetMeshCylinder();

	fRot = D3DX_PI * 2 / mesh.nMeshU;	// 一角あたりの回転量

	nCntEnd = (int)(WAVE_ANGLE / fRot) * 2;	// 波形の幅分の角数

	// 頂点情報のポインタ
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < mesh.nMeshU + 1; nCnt++)
	{// 各頂点の目標位置をリセット
		pPosDest[nCnt] = m_pPosInitial[nCnt];
		pPosDest[nCnt + mesh.nMeshU + 1] = m_pPosInitial[nCnt + mesh.nMeshU + 1];
		pVtx[nCnt].col = col;
		pVtx[nCnt + mesh.nMeshU + 1].col = col;
	}

	// 1角あたりにかかるsinfへの引数の増加量
	fSin = D3DX_PI / nCntEnd;
	fSin += 0.5f;

	std::list<CEnemy*> listEnemy = pEnemyManager->GetList();

	for (CEnemy *pEnemy : listEnemy)
	{
		// 差分角度を計算
		vecDiff = pos - pEnemy->GetPosition();

		fRotDiff = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;

		// スタートの割合を計算
		nIdxStart = (int)((fRotDiff - WAVE_ANGLE) / fRot);

		if (nIdxStart < 0)
		{
			nIdxStart = mesh.nMeshU + nIdxStart;
		}

		nIdx = nIdxStart;

		// 振れ幅の算出
		float fDiff = (float)(rand() % (int)(RAND_HEIGHT * 10) * 0.1f);

		// 目標方向にある頂点を検出
		for (int nCnt = 0; nCnt < nCntEnd; nCnt++)
		{

			// 差分距離の割合を求める
			float fRate = sinf(fSin * nCnt);
			float fHeight = fRate * WAVE_HEIGHT + fDiff;

			pPosDest[nIdx].x += m_pPosInitial[nIdx].x * WAVE_SIZE * fRate + m_pPosInitial[nIdx].x * SHAKE_SIZE * fDiff;
			pPosDest[nIdx].z += m_pPosInitial[nIdx].z * WAVE_SIZE * fRate + m_pPosInitial[nIdx].z * SHAKE_SIZE * fDiff;

			pPosDest[nIdx + mesh.nMeshU + 1].x += m_pPosInitial[nIdx].x * WAVE_SIZE * fRate + m_pPosInitial[nIdx].x * SHAKE_SIZE * fDiff;
			pPosDest[nIdx + mesh.nMeshU + 1].z += m_pPosInitial[nIdx].z * WAVE_SIZE * fRate + m_pPosInitial[nIdx].z * SHAKE_SIZE * fDiff;

			col = (D3DXCOLOR)pVtx[nIdx].col;

			col.g -= CHANGE_COl;
			col.b -= CHANGE_COl;

			pVtx[nIdx].col = col;
			pVtx[nIdx + mesh.nMeshU + 1].col = col;

			// ループカウントアップ
			nIdx = (nIdx + 1) % (mesh.nMeshU + 1);
		}
	}

	for (int nCnt = 0; nCnt < mesh.nMeshU + 1; nCnt++)
	{
		pVtx[nCnt].pos += (pPosDest[nCnt] - pVtx[nCnt].pos) * WAVE_SPEED;
		pVtx[nCnt + mesh.nMeshU + 1].pos += (pPosDest[nCnt + mesh.nMeshU + 1] - pVtx[nCnt + mesh.nMeshU + 1].pos) * WAVE_SPEED;
	}

	// 端の頂点同士をつなげる
	pVtx[mesh.nMeshU].pos = pVtx[0].pos;
	pVtx[mesh.nMeshU * 2 + 1].pos = pVtx[mesh.nMeshU + 1].pos;

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();

	if (pPosDest != nullptr)
	{// 目的座標の破棄
		delete[] pPosDest;
		pPosDest = nullptr;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CRader::Draw(void)
{
	CMeshCylinder::Draw();
}