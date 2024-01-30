//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "missile.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "universal.h"
#include "debugproc.h"
#include "particle.h"
#include "collision.h"
#include "orbit.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float MAX_SPEED = 30.0f;	// 移動速度
const float CHASE_SPEED = 3.0f;	// 追跡速度
const int INITIAL_LIFE = 1;	// 初期体力
const int DEATH_TIME = 240;	// 自滅までの時間
}

//=====================================================
// コンストラクタ
//=====================================================
CMissile::CMissile()
{
	m_nDeathTimer = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// 生成処理
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// 初期化処理
		pMissile->Init();
	}

	return pMissile;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMissile::Init(void)
{
	Load("data\\MOTION\\motionMissileSmall.txt");

	// 継承クラスの初期化
	CEnemy::Init();

	// 当たり判定をミサイルに変更
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		pCollision->SetTag(CCollision::TAG::TAG_ROCKET);
	}

	return S_OK;
}

//=====================================================
// 死亡処理
//=====================================================
void CMissile::Death(void)
{
	// 自身の終了
	Uninit();
}

//=====================================================
// 終了処理
//=====================================================
void CMissile::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMissile::Update(void)
{
	D3DXVECTOR3 vecMove;
	D3DXMATRIX mtx;

	// 位置保存
	SetPositionOld(GetPosition());

	// 追跡処理
	ChasePlayer();

	// 速度制限
	vecMove = GetMove();

	if (D3DXVec3Length(&vecMove) > MAX_SPEED)
	{
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecMove *= MAX_SPEED;

		SetMove(vecMove);
	}

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// 傾きの制御
	Tilt();

	// 当たり判定をミサイルに変更
	CCollisionSphere *pCollision = CEnemy::GetClsnSphere();

	if (pCollision != nullptr)
	{
		if (pCollision->IsTriggerEnter(CCollision::TAG_PLAYER))
		{
			CObject *pObj = pCollision->GetOther();

			if (pObj != nullptr)
			{
				// 当たったオブジェクトのヒット処理
				pObj->Hit(0.3f);

				Death();
			}
		}
	}

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 追跡処理
//=====================================================
void CMissile::ChasePlayer(void)
{
	// 情報取得
	CPlayer *pPlayer = CPlayer::GetInstance();

	// 計算用変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (pPlayer == nullptr)
	{// 死亡
		Uninit();

		return;
	}

	pos = pPlayer->GetMtxPos(0);

	// 差分を取得
	vecDiff = pos - GetPosition();

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= CHASE_SPEED;

	// 移動量設定
	SetMove(GetMove() + vecDiff);
}

//=====================================================
// 傾きの管理
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y軸平面の角度を取得
	rot.y = atan2f(move.x, move.z);

	// Y軸平面の移動量を取得
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// 横平面の角度を取得
	rot.x = atan2f(fLength,move.y);

	// 回転の設定
	SetRot(rot);
}

//=====================================================
// 描画処理
//=====================================================
void CMissile::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}