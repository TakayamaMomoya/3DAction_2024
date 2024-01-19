//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "player.h"
#include "enemyManager.h"
#include "collision.h"
#include "motion.h"
#include "inputManager.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "universal.h"
#include "slow.h"
#include "camera.h"
#include "manager.h"
#include "bullet.h"
#include "effect3D.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* BODY_PATH = "data\\MOTION\\motionArms01.txt";	// 見た目のパス
const float GRAVITY = 0.50f;	// 重力
const float SPEED_ROLL_CAMERA = 0.03f;	// カメラ回転速度
const float SPEED_BULLET = 50.0f;	// 弾速
const float POW_JUMP = 20.0f;	// ジャンプ力
const float POW_STAMP = 20.0f;	// 踏みつけの推進力
const float SPEED_MOVE = 1.6f;	// 移動速度
const float FACT_MOVE = 0.04f;	// 移動の減衰係数
const float SPEED_ASSAULT = 4.0f;	// 突進の移動速度
const float POW_ADDMELEE = 50.0f;	// 追撃の推進力
const float SPEED_DODGE = 50.0f;	// 回避推進力
const float POW_GRAB = 50.0f;	// 掴みの推進力
const float RADIUS_GRAB = 500.0f;	// 掴みの判定
const float POW_THROW = 100.0f;	// 投げの力
const float LENGTH_LOCKON = 5000.0f;	// ロックオンの長さ
const float ANGLE_LOCKON = D3DX_PI * 0.2f;	// ロックオンの角度
const float MELEE_DIST = 800.0f;	// 格闘に移る距離
const float MIN_ANGLE_CAMERA = D3DX_PI * 0.2f;	// カメラの下を見る制限
const float MAX_ANGLE_CAMERA = D3DX_PI * 0.7f;	// カメラの上を見る制限
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CPlayer::m_pPlayer = nullptr;	// 自身のポインタ

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_pPlayer = this;

	ZeroMemory(&m_info, sizeof(CPlayer::SInfo));
	ZeroMemory(&m_param, sizeof(CPlayer::SParam));
	ZeroMemory(&m_fragMotion, sizeof(CPlayer::SFragMotion));
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 生成処理
//=====================================================
CPlayer *CPlayer::Create(void)
{
	if (m_pPlayer == nullptr)
	{
		m_pPlayer = new CPlayer;

		if (m_pPlayer != nullptr)
		{
			m_pPlayer->Init();
		}
	}

	return m_pPlayer;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	// IDに対応したモデルの設定
	CMotion::Load((char*)BODY_PATH);

	// 継承クラスの初期化
	CMotion::Init();

	// 当たり判定の生成
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(20.0f);
		}
	}

	if (m_info.pClsnAttack == nullptr)
	{// 球の当たり判定生成
		m_info.pClsnAttack = CCollisionSphere::Create(CCollision::TAG_NONE, CCollision::TYPE_SPHERE, this);

		if (m_info.pClsnAttack != nullptr)
		{// 情報の設定
			m_info.pClsnAttack->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			m_info.pClsnAttack->SetRadius(0.0f);
		}
	}

	if (m_info.pCollisionCube == nullptr)
	{// 当たり判定生成
		m_info.pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER, this);

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 vtxMax = { 20.0f,100.0f,20.0f };
			D3DXVECTOR3 vtxMin = { -20.0f,0.0f,-20.0f };

			m_info.pCollisionCube->SetPosition(GetPosition());

			m_info.pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	m_info.fLife = m_param.fInitialLife;
	m_param.fSpeedMove = SPEED_MOVE;

	// 影の有効化
	SetPosShadow(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
	EnableShadow(true);

	SetMotion(MOTION_WALK_FRONT);

	m_info.bLand = true;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	if (m_info.pCollisionSphere != nullptr)
	{
		m_info.pCollisionSphere->Uninit();
		m_info.pCollisionSphere = nullptr;
	}

	if (m_info.pCollisionCube != nullptr)
	{
		m_info.pCollisionCube->Uninit();
		m_info.pCollisionCube = nullptr;
	}

	if (m_info.pClsnAttack != nullptr)
	{
		m_info.pClsnAttack->Uninit();
		m_info.pClsnAttack = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	CSlow *pSlow = CSlow::GetInstance();

	// 継承クラスの更新
	CMotion::Update();

	// ロックオン
	Lockon();

	// 入力
	Input();

	// プレイヤーの回転
	Rotation();

	// 位置の反映
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 move = GetMove();
	
	// 前回の位置を保存
	SetPositionOld(pos);

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		pos += move * fScale;
		SetPosition(pos);
	}
	else
	{
		pos += move;
		SetPosition(pos);
	}

	// 重力
	int nMotion = GetMotion();
	
	if (nMotion != MOTION_SHOT && 
		nMotion != MOTION_ASSAULT && 
		nMotion != MOTION_MELEE && 
		nMotion != MOTION_MELEE2 &&
		m_fragMotion.bStamp == false)
	{
		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			move.x += (0 - move.x) * FACT_MOVE * fScale;
			move.z += (0 - move.z) * FACT_MOVE * fScale;

			move.y -= GRAVITY * fScale;
		}
		else
		{
			move.x *= FACT_MOVE;
			move.z *= FACT_MOVE;

			move.y -= GRAVITY;
		}
	}
	else
	{
		move.x += (0 - move.x) * 0.05f;
		move.y += (0 - move.y) * 0.5f;
		move.z += (0 - move.z) * 0.05f;
	}

	SetMove(move);

	// 当たり判定の管理
	ManageCollision();

	// 状態管理
	ManageState();

	// モーション管理
	ManageMotion();

	if (m_info.pEnemyGrab != nullptr)
	{// 手に追従
		D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
		D3DXMATRIX mtxParent;
		D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

		universal::SetOffSet(&mtxParent, mtxPart, offset);

		CDebugProc::GetInstance()->Print("\n掴み位置[%f,%f,%f]", mtxParent._41, mtxParent._42, mtxParent._43);

		m_info.pEnemyGrab->SetMatrix(mtxParent);
	}

// デバッグ処理
#if _DEBUG

#endif // _DEBUG
}

//=====================================================
// ロックオン
//=====================================================
void CPlayer::Lockon(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	// 敵をロックオン
	pEnemyManager->Lockon(m_info.pEnemyGrab);
}

//=====================================================
// 入力処理
//=====================================================
void CPlayer::Input(void)
{
	// 移動操作
	InputMove();

	// カメラ操作
	InputCamera();

	// 攻撃操作
	InputAttack();
}

//=====================================================
// 移動の入力
//=====================================================
void CPlayer::InputMove(void)
{
	CSlow *pSlow = CSlow::GetInstance();
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

	// 目標方向の設定
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	// 入力方向の取得
	D3DXVECTOR3 vecInput = { 0.0f,0.0f,0.0f };

	vecInput += {sinf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x, 0.0f, cosf(pInfoCamera->rot.y + D3DX_PI * 0.5f) * axis.axisMove.x};
	vecInput += {sinf(pInfoCamera->rot.y) * axis.axisMove.z, 0.0f, cosf(pInfoCamera->rot.y) * axis.axisMove.z};

	float fLengthAxis = D3DXVec3Length(&axisMove);

	int nMotion = GetMotion();

	if (fLengthAxis >= 0.3f && nMotion != MOTION_SHOT)
	{// 通常移動時の目標向き設定
		m_info.rotDest.y = atan2f(vecInput.x, vecInput.z);

		CDebugProc *pDebugProc = CDebugProc::GetInstance();

		pDebugProc->Print("\n通常移動");
	}

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT || nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2 || nMotion == MOTION_THROW)
	{// 敵の方向を向く処理
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

			if (pEnemyLockon != nullptr)
			{
				D3DXVECTOR3 pos = GetPosition();
				pos.y += 200.0f;

				D3DXVECTOR3 posEnemy = pEnemyLockon->GetMtxPos(0);

				D3DXVECTOR3 vecDiff = posEnemy - pos;

				m_info.rotDest = universal::VecToRot(vecDiff);
				m_info.rotDest.x -= D3DX_PI * 0.5f;
			}
		}
	}

	if (nMotion != MOTION_ASSAULT &&
		nMotion != MOTION_MELEE &&
		nMotion != MOTION_MELEE2 &&
		nMotion != MOTION_GRAB &&
		nMotion != MOTION_THROW &&
		m_fragMotion.bStamp == false)
	{
		// 方向入力の取得
		CInputManager::SAxis axis = pInputManager->GetAxis();
		D3DXVECTOR3 axisMove = axis.axisMove;

		D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
		D3DXVECTOR3 rot = GetRot();

		float fLengthAxis = D3DXVec3Length(&axisMove);

		if (m_fragMotion.bMove && fLengthAxis <= 0.3f)
		{// 急停止フラグ
			m_fragMotion.bStop = true;
		}

		fLengthAxis *= SPEED_MOVE;

		if (nMotion == MOTION_SHOT)
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
		D3DXVECTOR3 move = GetMove();

		D3DXVec3Normalize(&vecMove, &vecMove);
		vecMove *= m_param.fSpeedMove;

		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			vecMove *= fScale;
		}

		if (m_info.bLand)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// ジャンプ操作
				m_fragMotion.bJump = true;
				m_fragMotion.bMove = false;
			};
		}
		else
		{
			if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
			{// ブースト上昇
				vecMove.y += 1.0f;
			};

			if (pInputManager->GetTrigger(CInputManager::BUTTON_JUMP))
			{// 踏みつけ
				Stamp();
			}
		}
		
		float fAngleInput = atan2f(axisMove.x, axisMove.z);

		if (pInputManager->GetTrigger(CInputManager::BUTTON_DODGE))
		{// ブースト回避
			vecMove +=
			{
				sinf(pInfoCamera->rot.y + fAngleInput) * SPEED_DODGE,
				0.0f,
				cosf(pInfoCamera->rot.y + fAngleInput) * SPEED_DODGE,
			};
		}

		move += vecMove;

		SetMove(move);
	}
	else if (nMotion == MOTION_ASSAULT)
	{
		AddMoveForward(SPEED_ASSAULT);
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CInputKeyboard::GetInstance();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// スローにする
			if (pSlow != nullptr)
			{
				pSlow->SetScale(0.25f);
			}
		}

		if (pKeyboard->GetRelease(DIK_RETURN))
		{// スロー解除
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}

		if (pKeyboard->GetTrigger(DIK_O))
		{// 可動パーツリセット
			ResetEnableMotion();
		}
	}
#endif
}

//=====================================================
// 踏みつけ
//=====================================================
void CPlayer::Stamp(void)
{
	if (m_info.pClsnAttack == nullptr)
	{// 判定のエラー
		return;
	}

	D3DXVECTOR3 pos = GetPosition();

	m_info.pClsnAttack->SetRadius(100.0f);
	m_info.pClsnAttack->SetPosition(pos);

	if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
	{// 対象との当たり判定
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{
			// 当たったオブジェクトのヒット処理
			pObj->Hit(5.0f);
		}

		m_fragMotion.bStamp = true;
	}
}

//=====================================================
// カメラ操作
//=====================================================
void CPlayer::InputCamera(void)
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

	// 方向入力の取得
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisCamera = axis.axisCamera;

	// カメラの回転
	pInfoCamera->rot.x -= axisCamera.y * SPEED_ROLL_CAMERA;
	pInfoCamera->rot.y += axisCamera.x * SPEED_ROLL_CAMERA;

	universal::LimitValue(&pInfoCamera->rot.x, MAX_ANGLE_CAMERA, MIN_ANGLE_CAMERA);

	universal::LimitRot(&pInfoCamera->rot.x);
	universal::LimitRot(&pInfoCamera->rot.y);
}

//=====================================================
// 攻撃の入力
//=====================================================
void CPlayer::InputAttack(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	int nMotion = GetMotion();

	if (pInputManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_SHOT))
	{// 射撃処理
		m_fragMotion.bShot = true;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_MELEE))
	{// 近接攻撃処理
		m_fragMotion.bMelee = true;

		if (nMotion == MOTION_MELEE)
		{
			m_fragMotion.bAddAttack = true;
		}
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_GRAB))
	{// 掴み処理

		if (nMotion != MOTION_THROW && nMotion != MOTION_GRAB)
		{
			m_fragMotion.bGrab = true;
		}

		if (nMotion == MOTION_THROW)
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

//=====================================================
// プレイヤーの回転
//=====================================================
void CPlayer::Rotation(void)
{
	// 移動方向の取得
	D3DXVECTOR3 move = GetMove();

	float fAngleMove = atan2f(-move.x, -move.z);
	float fLenghtMove = sqrtf(move.x * move.x + move.z * move.z);

	int nMotion = GetMotion();

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT || nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2 || nMotion == MOTION_THROW)
	{
		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager == nullptr)
		{
			return;
		}

		CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

		if (pEnemyLockon == nullptr)
		{
			// カメラ取得
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera == nullptr)
			{
				return;
			}

			CCamera::Camera *pInfoCamera = pCamera->GetCamera();

			// 向きの補正
			D3DXVECTOR3 rot = GetRot();
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			rotCamera.x -= D3DX_PI * 0.5f;
			rotCamera.y;

			universal::LimitRot(&rotCamera.x);
			universal::LimitRot(&rotCamera.y);

			universal::FactingRot(&m_info.rotDest.x, rotCamera.x, 0.15f);
			universal::FactingRot(&m_info.rotDest.y, rotCamera.y, 0.15f);
		}
	}
	else
	{
		if (fLenghtMove >= 6.0f)
		{
			// 向きの補正
			D3DXVECTOR3 rot = GetRot();

			SetRot(rot);

			if (m_info.bLand)
			{
				m_fragMotion.bMove = true;
			}
		}
		else
		{
			m_fragMotion.bMove = false;
		}

		m_info.rotDest.x = 0.0f;
	}

	// 向きの補正
	D3DXVECTOR3 rot = GetRot();

	float fFact = 0.1f;

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT)
	{
		fFact = 0.4f;
	}

	universal::FactingRot(&rot.y, m_info.rotDest.y + D3DX_PI, fFact);
	universal::FactingRot(&rot.x, m_info.rotDest.x, fFact);

	SetRot(rot);
}

//=====================================================
// 状態管理
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_info.state)
	{
	case STATE_NORMAL:
	{// 通常状態

	}
		break;
	case STATE_DAMAGE:
	{// ダメージ状態

	}
		break;
	case STATE_DEATH:
	{// 死亡状態

	}
		break;
	default:
		break;
	}
}

//=====================================================
// 当たり判定管理
//=====================================================
void CPlayer::ManageCollision(void)
{
	// 当たり判定の追従
	if (m_info.pCollisionSphere != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetMtxPos(0);

		// 敵との接触判定
		m_info.pCollisionSphere->SetPosition(posWaist);

		bool bHit = m_info.pCollisionSphere->OnEnter(CCollision::TAG_ENEMY);

		if (bHit)
		{
			//Hit(5.0f);
		}

		if (m_info.pCollisionCube != nullptr)
		{
			D3DXVECTOR3 pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();
			D3DXVECTOR3 move = GetMove();

			// 判定の追従
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// ブロックとの押し出し判定
			m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			if (pos.y <= 0.0f && posOld.y >= 0.0f)
			{
				pos.y = 0.0f;
				move.y = 0.0f;

				SetPosition(pos);
				
				int nMotion = GetMotion();
				bool bFinish = IsFinish();

				if (nMotion == MOTION_AIR)
				{
					m_info.bLand = true;
					m_fragMotion.bAir = false;
					m_fragMotion.bJump = false;
				}
			}
			else
			{
				m_info.bLand = false;

				m_fragMotion.bAir = true;
			}

			SetMove(move);
		}

		if (m_info.pCollisionSphere != nullptr)
		{
			pos = GetPosition();

			// 敵との押し出し判定
			m_info.pCollisionSphere->SetPosition(pos);

			m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG_ENEMY);

			SetPosition(pos);
		}
	}
}

//=====================================================
// モーション管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	int nMotion = GetMotion();
	bool bFinish = IsFinish();

	if (m_fragMotion.bStamp || nMotion == MOTION_STAMP)
	{// 踏みつけモーション
		if (nMotion != MOTION_STAMP)
		{
			SetMotion(MOTION_STAMP);
		}
		else
		{
			if (bFinish)
			{
				SetMotion(MOTION_AIR);
			}
		}
	}
	else if (nMotion == MOTION_THROW)
	{// 投げモーション
		if (bFinish)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bGrab)
	{// 掴みモーション
		if (nMotion != MOTION_GRAB)
		{
			SetMotion(MOTION_GRAB);

			AddMoveForward(POW_GRAB);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bGrab = false;
			}
		}
	}
	else if (nMotion == MOTION_MELEE2)
	{
		if (bFinish)
		{
			SetMotion(MOTION_AIR);

			EndMelee();
		}
	}
	else if (nMotion == MOTION_MELEE)
	{
		if (bFinish)
		{
			if (m_fragMotion.bAddAttack)
			{
				SetMotion(MOTION_MELEE2);
				m_fragMotion.bAddAttack = false;
				m_fragMotion.bMelee = false;

				CEnemy *pEnemyLockon = GetLockOn();

				if (pEnemyLockon != nullptr)
				{
					D3DXVECTOR3 pos = GetPosition();
					D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

					if (universal::DistCmp(pos, posEnemy, MELEE_DIST, nullptr) == false)
					{
						AddMoveForward(POW_ADDMELEE);
					}
				}
			}
			else
			{
				SetMotion(MOTION_AIR);

				EndMelee();
			}
		}
	}
	else if (m_fragMotion.bMelee)
	{// 近接攻撃モーション
		if (nMotion != MOTION_ASSAULT)
		{
			SetMotion(MOTION_ASSAULT);
		}
		else
		{
			StartMelee();
		}
	}
	else if (m_fragMotion.bShot)
	{// 射撃モーション
		if (nMotion != MOTION_SHOT)
		{
			SetMotion(MOTION_SHOT);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bShot = false;
			}
		}
	}
	else if (m_fragMotion.bAir)
	{// 滞空モーション
		if (nMotion != MOTION_AIR)
		{
			SetMotion(MOTION_AIR);
		}
	}
	else if (m_fragMotion.bJump)
	{// ジャンプモーション
		if (nMotion != MOTION_JUMP)
		{
			SetMotion(MOTION_JUMP);
		}
		else
		{

		}
	}
	else if (m_fragMotion.bStop)
	{// 急停止モーション
		if (nMotion != MOTION_STOP)
		{
			SetMotion(MOTION_STOP);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bStop = false;
			}
		}
	}
	else if (m_fragMotion.bMove)
	{// 歩きモーション
		if (nMotion != MOTION_WALK_FRONT)
		{
			SetMotion(MOTION_WALK_FRONT);
		}
	}
	else
	{// 待機モーション
		if (nMotion != MOTION_NEUTRAL)
		{
			SetMotion(MOTION_NEUTRAL);
		}
	}
}

//=====================================================
// 近接攻撃の始まるタイミング
//=====================================================
void CPlayer::StartMelee(void)
{
	bool bFinish = IsFinish();
	bool bNear = false;

	CEnemy *pEnemyLockon = GetLockOn();

	if (pEnemyLockon != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

		if (universal::DistCmp(pos, posEnemy, MELEE_DIST, nullptr))
		{
			bNear = true;

			// 移動を止める
			D3DXVECTOR3 move = GetMove();

			move *= 0.7f;

			SetMove(move);
		}
	}

	if (bFinish || bNear)
	{// 条件を満たしたら格闘攻撃に移行
		m_fragMotion.bMelee = false;

		SetMotion(MOTION_MELEE);

		m_info.rotDest.x = 0.0f;

		CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

		if (pEnemyManager != nullptr)
		{
			pEnemyManager->EnableLockTarget(true);
		}
	}
}

//=====================================================
// 前方に移動量を加える
//=====================================================
void CPlayer::AddMoveForward(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRot();

	vecMove =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * fSpeed,
		cosf(rot.x - D3DX_PI * 0.5f) * fSpeed,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * fSpeed
	};

	move += vecMove;

	SetMove(move);
}

//=====================================================
// 上に移動量を加える
//=====================================================
void CPlayer::AddMoveUp(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };

	vecMove.y += fSpeed;

	move += vecMove;

	SetMove(move);
}

//=====================================================
// イベントの管理
//=====================================================
void CPlayer::Event(EVENT_INFO *pEventInfo)
{
	int nMotion = GetMotion();

	D3DXVECTOR3 offset = pEventInfo->offset;
	D3DXMATRIX mtxParent;
	D3DXMATRIX mtxPart = *GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

	universal::SetOffSet(&mtxParent, mtxPart, offset);

	D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42 ,mtxParent._43 };

	if (nMotion == MOTION_SHOT)
	{// 弾を発射
		Shot(pos);
	}

	if (nMotion == MOTION_JUMP)
	{// ジャンプ
		D3DXVECTOR3 move = GetMove();

		move.y += POW_JUMP;

		SetMove(move);
	}

	if (nMotion == MOTION_STAMP)
	{// 踏みつけ
		AddMoveUp(POW_STAMP);

		m_fragMotion.bStamp = false;
	}

	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{// 近接攻撃
		ManageAttack(pos,300.0f);
	}

	if (nMotion == MOTION_GRAB)
	{// 掴み
		// 敵を掴む
		if (m_info.pClsnAttack == nullptr)
		{// 判定のエラー
			return;
		}

		m_info.pClsnAttack->SetPosition(pos);
		m_info.pClsnAttack->SetRadius(RADIUS_GRAB);

		if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
		{// 対象との当たり判定
			CObject *pObj = m_info.pClsnAttack->GetOther();

			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager != nullptr)
			{// 敵チェック
				// 掴む
				CEnemy *pEnemy = pEnemyManager->GetHead();
				CEnemy *pEnemyGrab = nullptr;

				while (pEnemy != nullptr)
				{
					CEnemy *pEnemyNext = pEnemy->GetNext();

					if ((CObject*)pEnemy == pObj)
					{
						pEnemyGrab = pEnemy;

						pEnemyGrab->DeleteCollision();
						pEnemyGrab->EnableIndependent(true);
						SetMotion(MOTION_THROW);

						// スローにする
						CSlow *pSlow = CSlow::GetInstance();

						if (pSlow != nullptr)
						{
							pSlow->SetScale(0.1f);
						}

						m_fragMotion.bGrab = false;
					}

					pEnemy = pEnemyNext;
				}

				// 掴む敵の決定
				m_info.pEnemyGrab = pEnemyGrab;
			}
		}
	}

	if (nMotion == MOTION_THROW)
	{// 掴んだ敵を投げ飛ばす
		if (m_info.pEnemyGrab != nullptr)
		{
			m_info.pEnemyGrab->EnableIndependent(false);

			D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
			D3DXMATRIX mtxParent;
			D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

			universal::SetOffSet(&mtxParent, mtxPart, offset);

			D3DXVECTOR3 pos = { mtxParent._41,mtxParent._42, mtxParent._43 };

			m_info.pEnemyGrab->SetPosition(pos);
			m_info.pEnemyGrab->CreateCollision();
			m_info.pEnemyGrab->SetState(CEnemy::STATE::STATE_THROWN);

			// 投げ方向に移動量を設定
			D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
			D3DXVECTOR3 rot = GetRot();

			vecMove =
			{
				sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * POW_THROW,
				cosf(rot.x - D3DX_PI * 0.5f) * POW_THROW,
				sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * POW_THROW
			};

			m_info.pEnemyGrab->SetMove(vecMove);

			m_info.pEnemyGrab = nullptr;

			// スローにする
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetScale(1.0f);
			}
		}
	}
}

//=====================================================
// 射撃
//=====================================================
void CPlayer::Shot(D3DXVECTOR3 posMazzle)
{
	D3DXVECTOR3 rot = GetRot();

	D3DXVECTOR3 move =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * SPEED_BULLET,
		cosf(rot.x - D3DX_PI * 0.5f) * SPEED_BULLET,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * SPEED_BULLET
	};

	CBullet *pBullet = CBullet::Create(posMazzle, move, 5, CBullet::TYPE_PLAYER, false, 40.0f, 5.0f,
		D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));
}

//=====================================================
// 攻撃判定の管理
//=====================================================
void CPlayer::ManageAttack(D3DXVECTOR3 pos, float fRadius)
{
	if (m_info.pClsnAttack == nullptr)
	{// 判定のエラー
		return;
	}

	m_info.pClsnAttack->SetPosition(pos);
	m_info.pClsnAttack->SetRadius(fRadius);

	if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
	{// 対象との当たり判定
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{
			m_info.pClsnAttack->DamageAll(CCollision::TAG::TAG_ENEMY, 5.0f);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// 継承クラスの描画
	CMotion::Draw();

	// デバッグ表示
	Debug();
}

//=====================================================
// ヒット処理
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_info.state == STATE_NORMAL)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// 死亡判定
			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			Uninit();
		}
		else
		{// ダメージ判定
			m_info.state = STATE_DAMAGE;
		}
	}
}

//=====================================================
// ロックオンの敵取得
//=====================================================
CEnemy *CPlayer::GetLockOn(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return nullptr;
	}

	CEnemy *pEnemyLockon = pEnemyManager->GetLockon();

	return pEnemyLockon;
}

//=====================================================
// 格闘の終了
//=====================================================
void CPlayer::EndMelee(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->EnableLockTarget(false);
	}
}

//=====================================================
// デバッグ表示
//=====================================================
void CPlayer::Debug(void)
{
#ifndef _DEBUG

	return;

#endif

	CDebugProc *pDebugProc = CDebugProc::GetInstance();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\nプレイヤーの位置[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	pDebugProc->Print("\nプレイヤーの移動量[%f,%f,%f]", GetMove().x, GetMove().y, GetMove().z);
	pDebugProc->Print("\n目標の向き[%f,%f,%f]", GetRot().x, GetRot().y, GetRot().z);

	int nMotion = GetMotion();

	pDebugProc->Print("\nモーション[%d]", nMotion);
	pDebugProc->Print("\n着地[%d]", m_info.bLand);
	pDebugProc->Print("\n移動[%d]", m_fragMotion.bMove);
	pDebugProc->Print("\n射撃[%d]", m_fragMotion.bShot);
	pDebugProc->Print("\n追撃[%d]", m_fragMotion.bAddAttack);
	pDebugProc->Print("\n空中[%d]", m_fragMotion.bAir);
}