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
#include "meshfield.h"
#include "cameraBehavior.h"
#include "heat.h"
#include "particle.h"
#include "debrisSpawner.h"
#include "game.h"
#include "boostEffect.h"
#include "animEffect3D.h"
#include "anim3D.h"
#include "continue.h"
#include "pause.h"
#include "inpact.h"
#include "sound.h"
#include "UIManager.h"
#include "orbit.h"
#include "playercontroller.h"
#include "rader.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float LIMIT_HEIGHT = 4000.0f;	// 高さ制限
const D3DXVECTOR3 POS_PARAM[CPlayer::PARAM_MAX] =
{// パラメータ表示の位置
	{SCREEN_WIDTH * 0.5f - 370.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// 銃
	{SCREEN_WIDTH * 0.5f + 370.0f,SCREEN_HEIGHT * 0.5f - 100.0f,0.0f},// 近接
};
const char* PATH_PARAM[CPlayer::PARAM_MAX] =
{// パラメータUIのテクスチャパス
	"data\\TEXTURE\\UI\\frame00.png",
	"data\\TEXTURE\\UI\\frame01.png",
};
const int RAND_SHOT = 60;	// 射撃精度のランダム幅
const float SIZE_HIT_EFFECT = 600.0f;	// ヒットエフェクトのサイズ
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
	// パラメーター読込
	Load();

	// モデルの設定
	CMotion::Load((char*)&m_param.pathBody[0]);

	// 継承クラスの初期化
	CMotion::Init();

	m_info.rotDest = D3DXVECTOR3(0.0f, 1.57f, 0.0f);
	SetRotation(D3DXVECTOR3(0.0f, -1.57f, 0.0f));

	// 当たり判定の生成
	if (m_info.pCollisionSphere == nullptr)
	{
		m_info.pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE, this);

		if (m_info.pCollisionSphere != nullptr)
		{
			m_info.pCollisionSphere->SetRadius(50.0f);
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

	m_info.state = STATE_NORMAL;
	m_info.stateBoost = STATEBOOST_NORMAL;
	m_info.bLand = true;

	SetMotion(MOTION_WALK_FRONT);

	// カメラの行動設定
	Camera::ChangeBehavior(new CFollowPlayer);

	// UIの生成
	for (int i = 0; i < PARAM_MAX; i++)
	{
		m_info.apHeatUI[i] = CHeat::Create();

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetPosition(POS_PARAM[i]);
			m_info.apHeatUI[i]->BindTextureFrame(PATH_PARAM[i]);
		}
	}

	for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
	{// コントローラーの生成
		CreateController((TYPE_CONTROLLER)i);
	}

	// レーダー生成
	CRader::Create();

	return S_OK;
}

//=====================================================
// 読込処理
//=====================================================
void CPlayer::Load(void)
{
	char cTemp[256];
	int nThruster = 0;

	FILE *pFile = fopen("data\\TEXT\\playerParam.txt", "r");

	if (pFile != nullptr)
	{
		while (true)
		{
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "BODY_PATH") == 0)
			{// 見た目のパス
				char aPath[MAX_STRING];

				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%s", &aPath[0]);

				m_param.pathBody = aPath;
			}

			if (strcmp(cTemp, "LIFE") == 0)
			{// 初期体力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fInitialLife);

				m_info.fLife = m_param.fInitialLife;
			}


			if (strcmp(cTemp, "SPEED_MOVE") == 0)
			{// 移動速度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedMove);
			}

			if (strcmp(cTemp, "FACT_MOVE") == 0)
			{// 移動量の減衰係数
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fFactMove);
			}

			if (strcmp(cTemp, "BOOST") == 0)
			{// ブースト最大量
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fInitialBoost);

				m_info.fBoost = m_param.fInitialBoost;
			}

			if (strcmp(cTemp, "REGEN_BOOST") == 0)
			{// ブースト回復量
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fRegenBoost);
			}

			if (strcmp(cTemp, "GRAVITY") == 0)
			{// 重力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fGravity);
			}

			if (strcmp(cTemp, "SPEED_BULLET") == 0)
			{// 弾の速度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedBullet);
			}

			if (strcmp(cTemp, "DAMAGE_BULLET") == 0)
			{// 弾の威力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fDamageBullet);
			}

			if (strcmp(cTemp, "ACC_SHOT") == 0)
			{// 射撃精度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fAccuracyBullet);
			}

			if (strcmp(cTemp, "POW_JUMP") == 0)
			{// ジャンプ力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowJump);
			}

			if (strcmp(cTemp, "SPEED_DODGE") == 0)
			{// 回避速度
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedDodge);
			}

			if (strcmp(cTemp, "SPEED_STAMP") == 0)
			{// 踏みつけの水平推進力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSpeedStamp);
			}

			if (strcmp(cTemp, "POW_ADDMELEE") == 0)
			{// 追撃推進力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowAddMelee);
			}

			if (strcmp(cTemp, "POW_THROW") == 0)
			{// 投げの力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowThrow);
			}

			if (strcmp(cTemp, "POW_STAMP") == 0)
			{// 踏みつけの垂直推進力
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fPowStamp);
			}

			if (strcmp(cTemp, "LENGTH_LOCKON") == 0)
			{// ロックオン距離
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fLengthLockOn);
			}

			if (strcmp(cTemp, "MELEE_DIST") == 0)
			{// 突進から格闘に移る距離
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fDistMelee);
			}

			if (strcmp(cTemp, "DECREASE_PARAM") == 0)
			{// パラメータ全回復にかかる時間
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fTimeRegenParam);
			}

			if (strcmp(cTemp, "SIZE_HITEFFECT") == 0)
			{// 格闘ヒットエフェクトのサイズ
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%f", &m_param.fSizeHitEffect);
			}

			if (strcmp(cTemp, "QUAKESET") == 0)
			{
				while (true)
				{
					SDamageQuake m_quake;

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LINE") == 0)
					{// 揺れのしきい値
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_quake.fLineMin);
						(void)fscanf(pFile, "%f", &m_quake.fLineMax);
					}

					if (strcmp(cTemp, "POW") == 0)
					{// 揺れの強さ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_quake.fPowQuake);
					}

					if (strcmp(cTemp, "FRAME") == 0)
					{// 揺れの長さ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_quake.nFrameQuake);
					}

					if (strcmp(cTemp, "END_QUAKESET") == 0)
					{
						m_listDamageQuake.push_back(m_quake);

						break;
					}
				}
			}

			if (strcmp(cTemp, "NUM_THRUSTER") == 0)
			{// スラスタ数読込
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_info.nNumThruster);

				if (m_info.pThruster == nullptr)
				{
					m_info.pThruster = new SInfoThruster[m_info.nNumThruster];

					if (m_info.pThruster != nullptr)
					{
						ZeroMemory(m_info.pThruster, sizeof(SInfoThruster) * m_info.nNumThruster);
					}
				}
			}

			if (strcmp(cTemp, "THRUSTERSET") == 0)
			{
				while (true)
				{
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARENT") == 0)
					{// 親番号
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nIdxParent);
					}

					if (strcmp(cTemp, "OFFSET") == 0)
					{// オフセット位置
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].offset.z);
					}

					if (strcmp(cTemp, "VECTOR") == 0)
					{// ベクトル
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.y);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].vector.z);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// サイズ
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.x);
						(void)fscanf(pFile, "%f", &m_info.pThruster[nThruster].size.y);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// 向き
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_info.pThruster[nThruster].nRot);
					}

					if (strcmp(cTemp, "END_THRUSTER") == 0)
					{
						if (m_info.pThruster[nThruster].pFire == nullptr)
						{
							m_info.pThruster[nThruster].pFire = CBoostEffect::Create();
						
							if (m_info.pThruster[nThruster].pFire != nullptr)
							{
								m_info.pThruster[nThruster].pFire->SetRadius(m_info.pThruster[nThruster].size.x);
								m_info.pThruster[nThruster].pFire->SetHeight(m_info.pThruster[nThruster].size.y);
							}
						}

						nThruster++;

						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}

		fclose(pFile);
	}
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	m_pPlayer = nullptr;

	Object::DeleteObject((CObject**)&m_info.pCollisionSphere);
	Object::DeleteObject((CObject**)&m_info.pCollisionCube);
	Object::DeleteObject((CObject**)&m_info.pClsnAttack);
	Object::DeleteObject((CObject**)&m_info.apHeatUI[0],PARAM_MAX);
	Object::DeleteObject((CObject**)&m_info.pOrbitWeapon);

	if (m_info.pThruster != nullptr)
	{// スラスター情報の破棄
		for (int i = 0; i < m_info.nNumThruster; i++)
		{
			Object::DeleteObject((CObject**)&m_info.pThruster[i].pFire);
		}
	}

	for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
	{// コントローラーの破棄
		DeleteController((TYPE_CONTROLLER)i);
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// コントローラー生成
//=====================================================
void CPlayer::CreateController(TYPE_CONTROLLER type)
{
	if (m_info.apController[type] != nullptr)
	{// 既にコントローラーがある場合は一度削除する
		delete m_info.apController[type];
		m_info.apController[type] = nullptr;
	}

	// コントローラーの生成
	switch (type)
	{
	case CPlayer::TYPE_MOVE:
		m_info.apController[type] = new CPlayerControllerMove;
		break;
	case CPlayer::TYPE_CAMERA:
		m_info.apController[type] = new CPlayerControllerCamera;
		break;
	case CPlayer::TYPE_ATTACK:
		m_info.apController[type] = new CPlayerControllerAttack;
		break;
	default:
		assert(("プレイヤーコントローラー生成時に不正な種類が入力されています", false));
		break;
	}
}

//=====================================================
// コントローラー削除
//=====================================================
void CPlayer::DeleteController(TYPE_CONTROLLER type)
{
	if (m_info.apController[type] != nullptr)
	{
		delete m_info.apController[type];
		m_info.apController[type] = nullptr;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	CSlow *pSlow = CSlow::GetInstance();

	// ロックオン
	Lockon();

	if (GetMotion() != MOTION_APPER)
	{
		// 入力
		Input();

		// プレイヤーの回転
		Rotation();
	}

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
		nMotion != MOTION_GRAB &&
		nMotion != MOTION_THROW &&
		nMotion != MOTION_DODGE &&
		m_fragMotion.bStamp == false)
	{
		if (pSlow != nullptr)
		{
			float fScale = pSlow->GetScale();

			move.x += (0 - move.x) * m_param.fFactMove * fScale;
			move.z += (0 - move.z) * m_param.fFactMove * fScale;

			move.y -= m_param.fGravity * fScale;
		}
		else
		{
			float fScale = pSlow->GetScale();

			move.x *= m_param.fFactMove;
			move.z *= m_param.fFactMove;

			move.y -= m_param.fGravity * fScale;
		}
	}
	else
	{
		if (nMotion == MOTION_DODGE)
		{
			move.x += (0 - move.x) * 0.02f;
			move.y += (0 - move.y) * 0.5f;
			move.z += (0 - move.z) * 0.02f;
		}
		else
		{
			move.x += (0 - move.x) * 0.05f;
			move.y += (0 - move.y) * 0.1f;
			move.z += (0 - move.z) * 0.05f;
		}
	}

	if (move.y > 24.0f)
	{// 上昇力制限
		move.y = 24.0f;
	}

	SetMove(move);

	// 当たり判定の管理
	ManageCollision();

	// 状態管理
	ManageState();

	// モーション管理
	ManageMotion();

	// パラメーター管理
	ManageParam();

	if (m_info.pEnemyGrab != nullptr)
	{// 手に追従
		D3DXVECTOR3 offset = { 0.0f,-100.0f,0.0f };
		D3DXMATRIX mtxParent;
		D3DXMATRIX mtxPart = *GetParts(5)->pParts->GetMatrix();

		universal::SetOffSet(&mtxParent, mtxPart, offset);

		CDebugProc::GetInstance()->Print("\n掴み位置[%f,%f,%f]", mtxParent._41, mtxParent._42, mtxParent._43);

		m_info.pEnemyGrab->SetMatrix(mtxParent);
	}

	// 空中の場合、ブーストの回復量が落ちる
	if (m_info.bLand)
	{
		AddBoost(m_param.fRegenBoost);
	}
	else
	{
		AddBoost(m_param.fRegenBoost * 0.2f);
	}
	
	if (nMotion == MOTION_WALK_FRONT)
	{// 足元に砂煙を出す
		D3DXVECTOR3 posToe = GetMtxPos(11);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);

		posToe = GetMtxPos(14);
		CParticle::Create(posToe, CParticle::TYPE::TYPE_SAND_SMOKE);
	}

	// ブーストエフェクト制御
	Boost();

	// 継承クラスの更新
	CMotion::Update();

// デバッグ処理
#if _DEBUG
	Debug();
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
	for (CPlayerController *pController : m_info.apController)
	{// 各コントローラーの更新
		if (pController != nullptr)
		{
			pController->Update(this);
		}
	}

	if (CManager::GetMode() == CScene::MODE_GAME)
	{
		CInputManager *pInputManager = CInputManager::GetInstance();

		if (pInputManager != nullptr)
		{
			if (pInputManager->GetTrigger(CInputManager::BUTTON_PAUSE))
			{
				CPause::Create();
			}
		}
	}
}

//=====================================================
// 踏みつけ
//=====================================================
void CPlayer::Stamp(void)
{
	if (m_info.pClsnAttack == nullptr || m_fragMotion.bStamp)
	{// 判定のエラー
		return;
	}

	D3DXVECTOR3 pos = GetPosition();

	m_info.pClsnAttack->SetRadius(100.0f);
	m_info.pClsnAttack->SetPosition(pos);

	if (m_info.pClsnAttack->IsTriggerEnter(CCollision::TAG::TAG_ENEMY))
	{// 対象との当たり判定
		CObject *pObj = m_info.pClsnAttack->GetOther();

		if (pObj != nullptr)
		{// 踏める敵かチェック
			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager == nullptr)
			{
				return;
			}

			std::list<CEnemy*> listEnemy = pEnemyManager->GetList();

			for (auto pEnemy : listEnemy)
			{
				if ((CObject*)pEnemy == pObj)
				{
					bool bStamp = pEnemy->IsStamp();

					if (bStamp)
					{
						m_fragMotion.bStamp = true;

						D3DXVECTOR3 move = GetMove();

						move *= 0.0;

						SetMove(move);

						// 当たったオブジェクトのヒット処理
						pObj->Hit(5.0f);
					}
				}
			}
		}
	}
}

//=====================================================
// ロックしてる敵の切り替え
//=====================================================
void CPlayer::SwitchLockEnemy(void)
{
	CInputManager *pInputManager = CInputManager::GetInstance();
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pInputManager == nullptr || pEnemyManager == nullptr)
	{
		return;
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_RIGHT))
	{// 右方向
		pEnemyManager->SwitchTarget(1, 0,m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_LEFT))
	{// 左方向
		pEnemyManager->SwitchTarget(-1, 0, m_info.pEnemyGrab);
	}

	if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_UP))
	{// 上方向
		pEnemyManager->SwitchTarget(0, 1, m_info.pEnemyGrab);
	}
	else if (pInputManager->GetTrigger(CInputManager::BUTTON_TRIGGER_DOWN))
	{// 下方向
		pEnemyManager->SwitchTarget(0, -1, m_info.pEnemyGrab);
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
			D3DXVECTOR3 rot = GetRotation();
			D3DXVECTOR3 rotCamera = pInfoCamera->rot;

			rotCamera.x -= D3DX_PI * 0.5f;
			rotCamera.y;

			universal::LimitRot(&rotCamera.x);
			universal::LimitRot(&rotCamera.y);

			universal::FactingRot(&m_info.rotDest.x, -rotCamera.x, 0.15f);
			universal::FactingRot(&m_info.rotDest.y, rotCamera.y, 0.15f);
		}
	}
	else
	{
		if (fLenghtMove >= 6.0f)
		{
			// 向きの補正
			D3DXVECTOR3 rot = GetRotation();

			SetRotation(rot);

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
	D3DXVECTOR3 rot = GetRotation();

	float fFact = 0.1f;

	if (nMotion == MOTION_SHOT || nMotion == MOTION_ASSAULT)
	{
		fFact = 0.4f;
	}

	universal::FactingRot(&rot.y, m_info.rotDest.y + D3DX_PI, fFact);
	universal::FactingRot(&rot.x, m_info.rotDest.x, fFact);

	SetRotation(rot);
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
		bool bLandMesh = false;
		bool bLandBlock = false;
		int nMotion = GetMotion();

		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posWaist = GetMtxPos(0);
		D3DXVECTOR3 move = GetMove();

		// 敵との接触判定
		m_info.pCollisionSphere->SetPosition(posWaist + move);

		if (m_info.pCollisionCube != nullptr)
		{
			pos = GetPosition();
			D3DXVECTOR3 posCollision = m_info.pCollisionCube->GetPosition();

			// 判定の追従
			m_info.pCollisionCube->SetPositionOld(posCollision);
			m_info.pCollisionCube->SetPosition(pos);

			// ブロックとの押し出し判定
			bLandBlock = m_info.pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

			// メッシュフィールドとの当たり判定
			pos = GetPosition();
			D3DXVECTOR3 posOld = GetPositionOld();

			CMeshField *pMesh = CMeshField::GetInstance();

			if (pMesh != nullptr)
			{
				float fHeight = pMesh->GetHeight(pos,&move);

				if (pos.y <= fHeight)
				{
					pos.y = fHeight;
					move.y = 0.0f;

					SetPosition(pos);

					bool bFinish = IsFinish();

					bLandMesh = true;
				}
			}

			SetMove(move);
		}

		m_info.pCollisionSphere->PushCollision(&pos, CCollision::TAG::TAG_ENEMY);

		if (pos.y > LIMIT_HEIGHT)
		{
			pos.y = LIMIT_HEIGHT;
		}

		SetPosition(pos);

		m_info.bLand = bLandMesh || bLandBlock;
		m_fragMotion.bAir = !m_info.bLand;

		if (m_info.bLand)
		{
			if (nMotion == MOTION_AIR || nMotion == MOTION_FALL)
			{
				m_fragMotion.bJump = false;
				m_fragMotion.bAir = false;
				m_fragMotion.bFall = false;
			}
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

	if (nMotion == MOTION_APPER)
	{
		if (bFinish)
		{
			SetMotion(MOTION_NEUTRAL);

			Camera::ChangeBehavior(new CFollowPlayer);

			CUIManager *pUIManager = CUIManager::GetInstance();

			if (pUIManager != nullptr)
			{
				pUIManager->EnableDisp(true);
			}
		}
	}
	else if (nMotion == MOTION_DEATH)
	{
		if (bFinish)
		{
			Death();
		}
	}
	else if (m_fragMotion.bDodge)
	{
		if (nMotion != MOTION_DODGE)
		{
			SetMotion(MOTION_DODGE);
		}
		else
		{
			if (bFinish)
			{
				m_fragMotion.bDodge = false;
			}
		}
	}
	else if (m_fragMotion.bStamp || nMotion == MOTION_STAMP)
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

			AddMoveForward(m_param.fPowGrab);
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
				Sound::Play(CSound::LABEL_SE_DASH00);

				SetMotion(MOTION_MELEE2);
				m_fragMotion.bAddAttack = false;
				m_fragMotion.bMelee = false;
				m_info.bMelee = false;

				CEnemy *pEnemyLockon = GetLockOn();

				if (pEnemyLockon != nullptr)
				{
					D3DXVECTOR3 pos = GetPosition();
					D3DXVECTOR3 posEnemy = pEnemyLockon->GetPosition();

					if (universal::DistCmp(pos, posEnemy, m_param.fDistMelee, nullptr) == false)
					{
						Camera::ChangeBehavior(new CMoveCylinder);

						AddMoveForward(m_param.fPowAddMelee);
					}
				}

				m_info.aParam[PARAM_MELEE] += 0.9f;
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
			Sound::Play(CSound::LABEL_SE_DASH00);

			SetMotion(MOTION_ASSAULT);

			D3DXVECTOR3 move = GetMove();

			move *= 0.01f;

			SetMove(move);
		}
		else
		{
			StartMelee();
		}
	}
	else if (m_fragMotion.bShot)
	{// 射撃モーション
		if (nMotion != MOTION_SHOT || (nMotion == MOTION_SHOT && bFinish))
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
	else if (m_fragMotion.bFall)
	{// 降下モーション
		if (nMotion != MOTION_FALL)
		{
			SetMotion(MOTION_FALL);
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
// パラメータ管理
//=====================================================
void CPlayer::ManageParam(void)
{
	for (int i = 0; i < PARAM_MAX; i++)
	{
		if (m_info.aParam[i] > 1.0f)
		{
			m_info.aParam[i] = 1.0f;
			m_info.aHeat[i] = true;
		}

		float fDeltaTime = CManager::GetDeltaTime();

		m_info.aParam[i] -= 1.0f * (fDeltaTime / m_param.fTimeRegenParam);

		if (m_info.aParam[i] < 0.0f)
		{
			m_info.aParam[i] = 0.0f;
			m_info.aHeat[i] = false;
		}

		if (m_info.apHeatUI[i] != nullptr)
		{
			m_info.apHeatUI[i]->SetParam(m_info.aParam[i]);
		}
	}
}

//=====================================================
// ブーストエフェクトの制御
//=====================================================
void CPlayer::Boost(void)
{
	int nMotion = GetMotion();

	MultiplyMtx();

	if (m_info.pThruster == nullptr || nMotion == MOTION_APPER)
		return;

	CInputManager *pInputManager = CInputManager::GetInstance();

	if (pInputManager == nullptr)
		return;

	// 目標方向の設定
	CInputManager::SAxis axis = pInputManager->GetAxis();
	D3DXVECTOR3 axisMove = axis.axisMove;

	D3DXVECTOR3 move = GetMove();
	float fSpeed = sqrtf(axisMove.x * axisMove.x + axisMove.z * axisMove.z);

	universal::LimitValue(&fSpeed, 1.0f, 0.0f);

	for (int i = 0; i < m_info.nNumThruster; i++)
	{
		if (m_info.pThruster[i].pFire != nullptr)
		{
			int nParent = m_info.pThruster[i].nIdxParent;
			D3DXVECTOR3 offset = m_info.pThruster[i].offset;
			D3DXVECTOR3 vector = m_info.pThruster[i].vector;

			D3DXVECTOR3 posBoost;
			D3DXVECTOR3 vecBoost;

			D3DXMATRIX mtxParent = *GetParts(nParent)->pParts->GetMatrix();
			D3DXMATRIX mtx;
			D3DXMATRIX mtxVec;

			universal::SetOffSet(&mtx, mtxParent, offset);
			universal::SetOffSet(&mtxVec, mtx, vector);

			posBoost = { mtx._41, mtx._42 ,mtx._43 };
			vecBoost = { mtxVec._41 - posBoost.x,mtxVec._42 - posBoost.y,mtxVec._43 - posBoost.z };

			D3DXVECTOR3 rot = universal::VecToRot(vecBoost);
			rot.x *= -1;
			rot.x += D3DX_PI;

			float fHeight = m_info.pThruster[i].pFire->GetHeight();
			
			if (!m_info.bLand)
			{
				fSpeed *= 0.5f;
			}

			if (pInputManager->GetPress(CInputManager::BUTTON_JUMP))
			{
				fSpeed = 1.0f;
			}

			float fDest = m_info.pThruster[i].size.y * fSpeed;
			
			float fDiff = fDest - fHeight;

			float fFact = 0.2f;

			if (fDiff < 0.0f)
			{
				fFact = 0.8f;
			}

			fHeight += fDiff * fFact;

			m_info.pThruster[i].pFire->SetHeight(fHeight);
			m_info.pThruster[i].pFire->SetRotation(rot);
			m_info.pThruster[i].pFire->SetPosition(posBoost);
		}
	}

	// 武器の軌跡
	if (nMotion == MOTION_MELEE || nMotion == MOTION_MELEE2)
	{
		if (m_info.pOrbitWeapon == nullptr)
		{
			D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();
			D3DXVECTOR3 offset = { 0.0f,0.0f,-200.0f };

			m_info.pOrbitWeapon = COrbit::Create(mtx, D3DXVECTOR3(0.0f, 0.0f, 0.0f), offset, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);
		}

		if (m_info.pOrbitWeapon != nullptr)
		{
			D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();

			m_info.pOrbitWeapon->SetPositionOffset(mtx, 0);
		}
	}
	else
	{
		if (m_info.pOrbitWeapon != nullptr)
		{
			Object::DeleteObject((CObject**)&m_info.pOrbitWeapon);
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

		if (universal::DistCmp(pos, posEnemy, m_param.fDistMelee, nullptr))
		{
			Camera::ChangeBehavior(new CMoveCylinder);

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

		m_info.aParam[PARAM_MELEE] += 0.7f;
	}
}

//=====================================================
// 前方に移動量を加える
//=====================================================
void CPlayer::AddMoveForward(float fSpeed)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 vecMove = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 rot = GetRotation();

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

	if (nMotion == MOTION_APPER)
	{// 出現時の煙
		Sound::Play(CSound::LABEL_SE_LAND00);

		D3DXVECTOR3 posParticle = GetPosition();

		CParticle::Create(posParticle, CParticle::TYPE::TYPE_APPER_SMOKE);
	}

	if (nMotion == MOTION_DODGE)
	{// 回避
		D3DXMATRIX *pMtxPart = GetParts(pEventInfo->nIdxParent)->pParts->GetMatrix();

		universal::SetOffSet(pMtxPart, *pMtxPart,D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f));

		CInpact::Create(0.1f, pMtxPart);
	}

	if (nMotion == MOTION_SHOT)
	{// 弾を発射
		Shot(pos);
	}

	if (nMotion == MOTION_JUMP)
	{// ジャンプ
		D3DXVECTOR3 move = GetMove();

		move.y += m_param.fPowJump;

		SetMove(move);
	}

	if (nMotion == MOTION_STAMP)
	{// 踏みつけ
		AddMoveUp(m_param.fPowStamp);

		AddMoveStamp();

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
		m_info.pClsnAttack->SetRadius(m_param.fRadiusGrab);


		if (m_info.pClsnAttack->OnEnter(CCollision::TAG::TAG_ENEMY))
		{// 対象との当たり判定
			CObject *pObj = m_info.pClsnAttack->GetOther();

			CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

			if (pEnemyManager != nullptr)
			{// 敵チェック
				// 掴む
				CEnemy *pEnemyGrab = nullptr;

				std::list<CEnemy*> listEnemy = pEnemyManager->GetList();

				for (auto pEnemy : listEnemy)
				{
					CEnemy::TYPE type = pEnemy->GetType();

					if ((CObject*)pEnemy == pObj && type != CEnemy::TYPE_BOSS)
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
				}

				// 掴む敵の決定
				m_info.pEnemyGrab = pEnemyGrab;

				if (m_info.pEnemyGrab != nullptr)
				{// 掴んでいる敵からロックを外す
					CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

					if (pEnemyManager != nullptr)
					{
						pEnemyManager->SetEnemyLock(nullptr);
					}
				}
			}
		}
	}

	if (nMotion == MOTION_THROW)
	{// 掴んだ敵を投げ飛ばす
		if (m_info.pEnemyGrab != nullptr)
		{
			Sound::Play(CSound::LABEL_SE_IMPACT00);
			Sound::Play(CSound::LABEL_SE_IMPACT01);

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
			D3DXVECTOR3 rot = GetRotation();

			vecMove =
			{
				sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * m_param.fPowThrow,
				cosf(rot.x - D3DX_PI * 0.5f) * m_param.fPowThrow,
				sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * m_param.fPowThrow
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
	Sound::Play(CSound::LABEL_SE_SHOT00);

	D3DXVECTOR3 rot = GetRotation();

	D3DXVECTOR3 move =
	{
		sinf(rot.x - D3DX_PI * 0.5f) * sinf(rot.y) * m_param.fSpeedBullet,
		cosf(rot.x - D3DX_PI * 0.5f) * m_param.fSpeedBullet,
		sinf(rot.x - D3DX_PI * 0.5f) * cosf(rot.y) * m_param.fSpeedBullet
	};

	CEnemyManager *pEnemymanager = CEnemyManager::GetInstance();

	if (pEnemymanager != nullptr)
	{
		CEnemy *pEnemyLock = pEnemymanager->GetLockon();

		if (pEnemyLock != nullptr)
		{
			D3DXVECTOR3 posEnemy = pEnemyLock->GetMtxPos(0);
			D3DXVECTOR3 moveEnemy = pEnemyLock->GetMove();

			D3DXVECTOR3 posPridiction = universal::LinePridiction(posMazzle, m_param.fSpeedBullet, posEnemy, moveEnemy);

			posPridiction +=
			{
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
				(float)universal::RandRange(RAND_SHOT, -RAND_SHOT),
			};

			D3DXVECTOR3 vecDiff = posEnemy - posMazzle;

			universal::VecConvertLength(&vecDiff, m_param.fSpeedBullet);

			move = vecDiff;
		}
	}

	CBullet *pBullet = CBullet::Create(posMazzle, move, 5, CBullet::TYPE_PLAYER, false, 40.0f, m_param.fDamageBullet,
		D3DXCOLOR(1.0f, 0.6f, 0.0f, 1.0f));

	// 熱量を加算
	m_info.aParam[PARAM_GUN] += 0.1f;

	// エフェクト発生
	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim = pAnimManager->CreateEffect(posMazzle, CAnimEffect3D::TYPE_MUZZLEFLUSH);

		if (pAnim != nullptr)
		{
			pAnim->EnableZtest(false);
		}
	}
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

		if (pObj != nullptr && !m_info.bMelee)
		{
			Sound::Play(CSound::LABEL_SE_HIT01);

			// ダメージエフェクトの生成
			CAnimEffect3D *pAnim3D = CAnimEffect3D::GetInstance();

			if (pAnim3D != nullptr)
			{
				CAnim3D *pEffect = pAnim3D->CreateEffect(pos, CAnimEffect3D::TYPE::TYPE_HIT00);

				if (pEffect != nullptr)
				{
					pEffect->SetSize(SIZE_HIT_EFFECT, SIZE_HIT_EFFECT);
					pEffect->EnableZtest(true);
				}
			}

			// ヒットストップ
			CSlow *pSlow = CSlow::GetInstance();

			if (pSlow != nullptr)
			{
				pSlow->SetSlowTime(0.2f, 0.01f);
			}

			// カメラ揺れ
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				pCamera->SetQuake(1.01f, 1.01f, 10);
			}

			m_info.pClsnAttack->DamageAll(CCollision::TAG::TAG_ENEMY, 7.5f);
		}

		m_info.bMelee = true;
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
	if (m_info.state != STATE_DEATH)
	{
		m_info.fLife -= fDamage;

		if (m_info.fLife <= 0.0f)
		{// 死亡判定
			Sound::Play(CSound::LABEL_SE_WARNING00);

			m_info.fLife = 0.0f;

			m_info.state = STATE_DEATH;

			SetMotion(MOTION::MOTION_DEATH);

			for (int i = 0; i < TYPE_CONTROLLER::TYPE_MAX; i++)
			{// コントローラーの削除
				DeleteController((TYPE_CONTROLLER)i);
			}
		}
		else
		{// ダメージ判定
			m_info.state = STATE_DAMAGE;

			// カメラ揺れ
			CCamera *pCamera = CManager::GetCamera();

			if (pCamera != nullptr)
			{
				for (SDamageQuake quake : m_listDamageQuake)
				{
					if (fDamage >= quake.fLineMin && fDamage < quake.fLineMax)
						pCamera->SetQuake(quake.fPowQuake, quake.fPowQuake, quake.nFrameQuake);
				}
			}
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
	if (m_info.pOrbitWeapon != nullptr)
	{
		D3DXMATRIX mtx = *GetParts(16)->pParts->GetMatrix();

		m_info.pOrbitWeapon->SetEnd(true);

		m_info.pOrbitWeapon = nullptr;
	}

	Camera::ChangeBehavior(new CLookEnemy);

	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager != nullptr)
	{
		pEnemyManager->EnableLockTarget(false);
	}

	m_info.bMelee = false;
}

//=====================================================
// ターゲットロックの切り替え
//=====================================================
void CPlayer::ToggleLockTarget(void)
{
	CEnemyManager *pEnemyManager = CEnemyManager::GetInstance();

	if (pEnemyManager == nullptr)
	{
		return;
	}

	pEnemyManager->EnableLockTarget(m_info.bLockTarget);
}

//=====================================================
// ブースト加算
//=====================================================
void CPlayer::AddBoost(float fValue)
{
	m_info.fBoost += fValue;

	if (m_info.fBoost < 0.0f)
	{// オーバーヒート判定
		m_info.stateBoost = STATEBOOST_OVERHEAT;
	}

	if (m_info.stateBoost == STATEBOOST_OVERHEAT)
	{// オーバーヒートからの復帰
		float fRate = m_info.fBoost / m_param.fInitialBoost;

		if (fRate > 0.5f)
		{
			m_info.stateBoost = STATEBOOST_NORMAL;
		}
	}

	universal::LimitValue(&m_info.fBoost, m_param.fInitialBoost, 0.0f);
}

//=====================================================
// 踏みつけの移動量加算
//=====================================================
void CPlayer::AddMoveStamp(void)
{
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

	CInputManager::SAxis axis = pInputManager->GetAxis();

	axis.axisMove;

	float fLengthInput = sqrtf(axis.axisMove.x * axis.axisMove.x + axis.axisMove.y * axis.axisMove.y);

	if (fLengthInput >= 0.3f)
	{
		float fAngleInput = atan2f(axis.axisMove.x, axis.axisMove.y);

		D3DXVECTOR3 move = GetMove();

		move +=
		{
			sinf(pInfoCamera->rot.y + fAngleInput) * m_param.fSpeedStamp,
			0.0f,
			cosf(pInfoCamera->rot.y + fAngleInput) * m_param.fSpeedStamp,
		};

		SetMove(move);
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CPlayer::Death(void)
{
	CSound *pSound = CSound::GetInstance();

	if (pSound != nullptr)
	{
		pSound->Stop(CSound::LABEL_SE_WARNING00);
	}

	Sound::Play(CSound::LABEL_SE_EXPLOSION01);

	D3DXVECTOR3 pos = GetMtxPos(0);

	// エフェクト発生
	CAnimEffect3D *pAnimManager = CAnimEffect3D::GetInstance();

	if (pAnimManager != nullptr)
	{
		CAnim3D *pAnim = pAnimManager->CreateEffect(pos, CAnimEffect3D::TYPE_EXPLOSION);

		if (pAnim != nullptr)
		{
			pAnim->SetSize(600.0f, 600.0f);
		}
	}

	// 破片生成
	CDebrisSpawner::Create(pos, CDebrisSpawner::TYPE::TYPE_DEATH);
	
	Uninit();

	CContinue::Create();
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
	pDebugProc->Print("\n目標の向き[%f,%f,%f]", GetRotation().x, GetRotation().y, GetRotation().z);
	pDebugProc->Print("\nブースト残量[%f]", m_info.fBoost);
	pDebugProc->Print("\n体力[%f]", m_info.fLife);
	for (int i = 0; i < PARAM_MAX; i++)
		pDebugProc->Print("\n熱量[%f][%d]", m_info.aParam[i],m_info.aHeat[i]);

	int nMotion = GetMotion();

	pDebugProc->Print("\nモーション[%d]", nMotion);
	pDebugProc->Print("\nターゲットロック[%d]", m_info.bLockTarget);
	pDebugProc->Print("\n着地[%d]", m_info.bLand);
	pDebugProc->Print("\n空中[%d]", m_fragMotion.bAir);

	CInputKeyboard *pKey = CInputKeyboard::GetInstance();

	if (pKey != nullptr)
	{
		if (pKey->GetTrigger(DIK_O))
		{
			m_info.fLife = 1.0f;
		}
	}
}