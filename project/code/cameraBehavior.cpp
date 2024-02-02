//*****************************************************
//
// カメラビヘイビア[cameraBehavior.h]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "cameraBehavior.h"
#include "camera.h"
#include "enemyManager.h"
#include "player.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DIST_CYLINDER = 1000.0f;
const float DIST_LOOK = 1500.0f;
}

//=====================================================
// プレイヤーの追従
//=====================================================
void CFollowPlayer::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	bool bLock = pPlayer->IsTargetLock();
	CEnemy *pEnemyLock = pEnemyManager->GetLockon();

	D3DXVECTOR3 pos = pPlayer->GetMtxPos(2);

	pInfoCamera->posRDest = pos;

	//目標の視点設定
	pInfoCamera->posVDest =
	{
		pInfoCamera->posRDest.x - sinf(pInfoCamera->rot.x) * sinf(pInfoCamera->rot.y - D3DX_PI * 0.02f) * pInfoCamera->fLength,
		pInfoCamera->posRDest.y - cosf(pInfoCamera->rot.x) * pInfoCamera->fLength,
		pInfoCamera->posRDest.z - sinf(pInfoCamera->rot.x) * cosf(pInfoCamera->rot.y - D3DX_PI * 0.02f) * pInfoCamera->fLength
	};

	if (pEnemyLock != nullptr && bLock)
	{
		pCamera->ChangeBehavior(new CLookEnemy);
	}

#ifdef _DEBUG
	CEffect3D::Create(pInfoCamera->posRDest, 20.0f, 1, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
#endif
}

//=====================================================
// 敵を見る処理
//=====================================================
void CLookEnemy::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	CEnemy *pEnemyLock = pEnemyManager->GetLockon();
	D3DXVECTOR3 posEnemy = { 0.0f,0.0f,0.0f };

	if (pEnemyLock != nullptr)
	{
		posEnemy = pEnemyLock->GetMtxPos(0);
	}

	// 位置の差分を取得
	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(2);
	D3DXVECTOR3 vecDiff = posEnemy - posPlayer;
	D3DXVECTOR3 vecDiffFlat = { vecDiff.x,0.0f,vecDiff.z };

	// 差分の向きを計算
	D3DXVECTOR3 rot;
	float fLegnthDiff = D3DXVec3Length(&vecDiff);
	float fLegnthFlat = D3DXVec3Length(&vecDiffFlat);

	rot.x = atan2f(fLegnthFlat, vecDiff.y) + D3DX_PI * 0.01f;
	rot.y = atan2f(vecDiff.x, vecDiff.z);

	// 注視点位置の設定
	pInfoCamera->posRDest = posPlayer + vecDiff * 0.5f;

	float fLengthView = fLegnthDiff + pInfoCamera->fLength;

	// 視点位置の設定
	pInfoCamera->posVDest =
	{
		posEnemy.x - sinf(rot.x) * sinf(rot.y) * fLengthView,
		posEnemy.y - cosf(rot.x) * fLengthView,
		posEnemy.z - sinf(rot.x) * cosf(rot.y) * fLengthView
	};

	pInfoCamera->rot = rot;

	bool bLock = pPlayer->IsTargetLock();

	if (pEnemyLock == nullptr || bLock== false)
	{// 通常の注視
		pCamera->ChangeBehavior(new CFollowPlayer);
	}

	if (fLegnthFlat < DIST_CYLINDER)
	{// シリンダー状の注視に移行
		Camera::ChangeBehavior(new CMoveCylinder);
	}
}

//=====================================================
// シリンダー状の動きをする
//=====================================================
void CMoveCylinder::Update(CCamera *pCamera)
{
	if (pCamera == nullptr)
		return;

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();
	CPlayer *pPlayer = CPlayer::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pPlayer == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	CEnemy *pEnemyLock = pEnemyManager->GetLockon();
	D3DXVECTOR3 posEnemy = { 0.0f,0.0f,0.0f };

	if (pEnemyLock != nullptr)
	{
		posEnemy = pEnemyLock->GetMtxPos(0);
	}

	// 位置の差分を取得
	D3DXVECTOR3 posPlayer = pPlayer->GetMtxPos(2);
	D3DXVECTOR3 vecDiff = posEnemy - posPlayer;
	D3DXVECTOR3 vecDiffFlat = { vecDiff.x,0.0f,vecDiff.z };

	// 差分の向きを計算
	D3DXVECTOR3 rot;
	float fLegnthDiff = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

	float fLegnthFlat = D3DXVec3Length(&vecDiffFlat);

	rot.x = pInfoCamera->rot.x;
	rot.y = pInfoCamera->rot.y;

	// 注視点位置の設定
	pInfoCamera->posRDest = posPlayer + vecDiff * 0.5f;

	// 元の距離と差分距離を足す
	float fLengthView = fLegnthDiff + pInfoCamera->fLength;

	// 視点位置の設定
	pInfoCamera->posVDest =
	{
		posEnemy.x - sinf(rot.x) * sinf(rot.y) * fLengthView,
		posEnemy.y - cosf(rot.x) * fLengthView,
		posEnemy.z - sinf(rot.x) * cosf(rot.y) * fLengthView
	};

	pInfoCamera->rot = rot;

	bool bLock = pPlayer->IsTargetLock();

	if (pEnemyLock == nullptr || bLock == false)
	{// プレイヤー追従に移行
		pCamera->ChangeBehavior(new CFollowPlayer);
	}

	if (fLegnthFlat > DIST_LOOK)
	{// 普通の注視に移行
		Camera::ChangeBehavior(new CLookEnemy);
	}
}