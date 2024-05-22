//*****************************************************
//
// プレイヤー操作[playercontroller.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "playercontroller.h"
#include "player.h"
#include "inputManager.h"
#include "camera.h"
#include "slow.h"
#include "manager.h"
#include "debugproc.h"
#include "enemyManager.h"
#include "sound.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float SPEED_ASSAULT = 7.0f;	// 突進の移動速度
const float MIN_ANGLE_CAMERA = D3DX_PI * 0.1f;	// カメラの下を見る制限
const float MAX_ANGLE_CAMERA = D3DX_PI * 0.9f;	// カメラの上を見る制限
const float SPEED_ROLL_CAMERA = 0.03f;	// カメラ回転速度
}

//===================================================================================
// コントローラー基底クラス
//===================================================================================
//=====================================================
// コンストラクタ
//=====================================================
CPlayerController::CPlayerController()
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerController::~CPlayerController()
{

}

//===================================================================================
// 移動操作クラス
//===================================================================================
//=====================================================
// コンストラクタ
//=====================================================
CPlayerControllerMove::CPlayerControllerMove()
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerControllerMove::~CPlayerControllerMove()
{

}

//=====================================================
// 更新処理
//=====================================================
void CPlayerControllerMove::Update(CPlayer *pPlayer)
{
	CSlow *pSlow = CSlow::GetInstance();
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// カメラ取得
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// プレイヤー情報取得
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	// 目標方向の設定
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 入力方向の取得
	D3DXVECTOR3 vecInput = { 0.0f,0.0f,0.0f };

	vecInput += {sinf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x, 0.0f, cosf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x};
	vecInput += {sinf(pInfoCamera->rot.y) * axis.axisMove.z, 0.0f, cosf(pInfoCamera->rot.y) * axis.axisMove.z};

	float fLengthAxis = D3DXVec3Length(&axisMove);

	int nMotion = pPlayer->GetMotion();

	if ((fLengthAxis >= 0.3f && nMotion != CPlayer::MOTION_SHOT) || nMotion == CPlayer::MOTION_DODGE)
	{// 通常移動時の目標向き設定
		if (nMotion == CPlayer::MOTION_DODGE)
		{
			D3DXVECTOR3 move = pPlayer->GetMove();

			pInfo->rotDest.y = atan2f(move.x, move.z);
		}
		else
		{
			pInfo->rotDest.y = atan2f(vecInput.x, vecInput.z);
		}

		CDebugProc *pDebugProc = CDebugProc::GetInstance();

		pDebugProc->Print("\n通常移動");
	}

	if (nMotion != CPlayer::MOTION_DODGE && 
		(nMotion == CPlayer::MOTION_SHOT || nMotion == CPlayer::MOTION_ASSAULT || nMotion == CPlayer::MOTION_MELEE || nMotion == CPlayer::MOTION_MELEE2 || nMotion == CPlayer::MOTION_THROW || pInfo->bLockTarget))
	{// 敵の方向を向く処理
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

			if (pEnemyLockon != nullptr)
			{
				D3DXVECTOR3 pos = pPlayer->GetPosition();
				pos.y += 200.0f;

				D3DXVECTOR3 posEnemy = pEnemyLockon->GetMtxPos(0);

				D3DXVECTOR3 vecDiff = posEnemy - pos;

				pInfo->rotDest = universal::VecToRot(vecDiff);
				pInfo->rotDest.x -= D3DX_PI * 0.5f;
			}
		}
	}

	if (nMotion != CPlayer::MOTION_ASSAULT &&
		nMotion != CPlayer::MOTION_MELEE &&
		nMotion != CPlayer::MOTION_MELEE2 &&
		nMotion != CPlayer::MOTION_GRAB &&
		nMotion != CPlayer::MOTION_THROW &&
		nMotion != CPlayer::MOTION_DODGE &&
		pFragMotion->bStamp == false)
	{
		// 方向入力の取得
		CInputManager::SAxis axis = pInputManager->GetAxis();
		D3DXVECTOR3 axisMove = axis.axisMove;

		D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = pPlayer->GetRotation();

		float fLengthAxis = D3DXVec3Length(&axisMove);

		if (pFragMotion->bMove && fLengthAxis <= 0.3f)
		{// 急停止フラグ
			pFragMotion->bStop = true;
		}

		fLengthAxis *= param.fSpeedMove;

		if (nMotion == CPlayer::MOTION_SHOT || pInfo->bLockTarget)
		{// 視点固定時
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			vecMove += {sinf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x, 0.0f, cosf(rotCamera.y + D3DX_PI * 0.5f) * axisMove.x};
			vecMove += {sinf(rotCamera.y) * axisMove.z, 0.0f, cosf(rotCamera.y) * axisMove.z};
		}
		else
		{// 向きが自由な時
			vecMove -= {sinf(rot.y) * fLengthAxis, 0.0f, cosf(rot.y) * fLengthAxis};
		}

		// 移動速度の設定
		D3DXVECTOR3 move = pPlayer->GetMove();

		D3DXVec3Normalize(&vecMove, &vecMove);
		vecMove *= param.fSpeedMove;

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			vecMove *= fScale;
		}

		// ジャンプ操作
		if (pInfo->bLand)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// 地上用のジャンプ
				pFragMotion->bJump = true;
				pFragMotion->bMove = false;

				Sound::Play(CSound::LABEL_SE_BOOST00);
			};
		}
		else
		{
			if (pInfo->stateBoost != CPlayer::STATEBOOST_OVERHEAT)
			{// 空中のジャンプ操作はブースト容量が必要
				if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
				{// ジャンプボタンを押した瞬間だけ音を鳴らす
					Sound::Play(CSound::LABEL_SE_BOOST00);
				}

				if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
				{// ブースト上昇
					vecMove.y += 1.0f;

					pPlayer->AddBoost(-3.0f);

					pFragMotion->bFall = false;
				}
				else if(pFragMotion->bAir)
				{
					pFragMotion->bFall = true;
				}
			}

			// 踏みつけ判定
			pPlayer->Stamp();
		}

		// 入力方向の取得
		float fAngleInput = atan2f(axisMove.x, axisMove.z);

		if (pInfo->stateBoost != CPlayer::STATEBOOST_OVERHEAT)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
			{// ブースト回避
				vecMove +=
				{
					sinf(pInfoCamera->rot.y + fAngleInput) * param.fSpeedDodge,
					0.0f,
					cosf(pInfoCamera->rot.y + fAngleInput) * param.fSpeedDodge,
				};

				pPlayer->AddBoost(-50.0f);

				pFragMotion->bDodge = true;

				Sound::Play(CSound::LABEL_SE_DASH00);
			}
		}

		// 移動量の反映
		move += vecMove;

		pPlayer->SetMove(move);
	}
	else if (nMotion == CPlayer::MOTION_ASSAULT)
	{// 突進時の推進
		pPlayer->AddMoveForward(SPEED_ASSAULT);
	}
}

//===================================================================================
// カメラ操作クラス
//===================================================================================
//=====================================================
// コンストラクタ
//=====================================================
CPlayerControllerCamera::CPlayerControllerCamera()
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerControllerCamera::~CPlayerControllerCamera()
{

}

//=====================================================
// 更新処理
//=====================================================
void CPlayerControllerCamera::Update(CPlayer *pPlayer)
{
	// 入力マネージャー取得
	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
	{
		return;
	}

	// カメラ取得
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera == nullptr)
	{
		return;
	}

	CCamera::Camera *pInfoCamera = pCamera->GetCamera();

	// プレイヤー情報取得
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	if (pInputManager->GetTrigger(CInputManager::BUTTON_LOCK))
	{// ターゲットロック切り替え
		pInfo->bLockTarget = pInfo->bLockTarget ? false : true;

		pPlayer->ToggleLockTarget();
	}

	if (pInfo->bLockTarget)
	{// ロックしてる敵の切り替え
		pPlayer->SwitchLockEnemy();
	}

	// 方向入力の取得
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisCamera = axis.axisCamera;

	// カメラの回転
	pInfoCamera->rot.x += axisCamera.y * SPEED_ROLL_CAMERA;
	pInfoCamera->rot.y += axisCamera.x * SPEED_ROLL_CAMERA;

	universal::LimitValue(&pInfoCamera->rot.x, MAX_ANGLE_CAMERA, MIN_ANGLE_CAMERA);

	universal::LimitRot(&pInfoCamera->rot.x);
	universal::LimitRot(&pInfoCamera->rot.y);
}

//===================================================================================
// 攻撃操作クラス
//===================================================================================
//=====================================================
// コンストラクタ
//=====================================================
CPlayerControllerAttack::CPlayerControllerAttack()
{

}

//=====================================================
// デストラクタ
//=====================================================
CPlayerControllerAttack::~CPlayerControllerAttack()
{

}

//=====================================================
// 更新処理
//=====================================================
void CPlayerControllerAttack::Update(CPlayer *pPlayer)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	int nMotion = pPlayer->GetMotion();

	if (pInputManager == nullptr)
	{
		return;
	}

	// プレイヤー情報取得
	CPlayer::SInfo *pInfo = pPlayer->GetInfo();
	CPlayer::SFragMotion *pFragMotion = pPlayer->GetFragMotion();
	CPlayer::SParam param = pPlayer->GetParam();

	if (pInputManager->GetPress(CInputManager::BUTTON_SHOT) &&
		pInfo->aHeat[CPlayer::PARAM_GUN] == false)
	{// 射撃処理
		pFragMotion->bShot = true;
	}
	else
	{
		pFragMotion->bShot = false;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE) &&
		pInfo->aHeat[CPlayer::PARAM_MELEE] == false)
	{// 近接攻撃
		pFragMotion->bMelee = true;

		if (nMotion == CPlayer::MOTION_MELEE)
		{
			pFragMotion->bAddAttack = true;
		}
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_GRAB))
	{// 掴み処理

		if (nMotion != CPlayer::MOTION_THROW && nMotion != CPlayer::MOTION_GRAB)
		{
			pFragMotion->bGrab = true;
		}

		if (nMotion == CPlayer::MOTION_THROW)
		{
			// スローをキャンセル
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}