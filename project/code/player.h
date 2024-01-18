//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CObject3D;
class CEnemy;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_BLOW,	// 吹き飛ばされている状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};
	struct SParam
	{
		float fSpeedMove;		// 移動速度
		float fInitialLife;	// 初期体力
	};

	CPlayer(int nPriority = 4);	// コンストラクタ
	~CPlayer();	// デストラクタ

	static CPlayer *Create(void);
	static CPlayer *GetInstance(void) { return m_pPlayer; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(float fDamage);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK_FRONT,	// 前歩
		MOTION_SHOT,	// 射撃
		MOTION_JUMP,	// ジャンプ
		MOTION_AIR,	// 滞空
		MOTION_ASSAULT,	// 突進状態
		MOTION_MELEE,	// 近接攻撃
		MOTION_MELEE2,	// ２回目の近接攻撃
		MOTION_STOP,	// 急停止モーション
		MOTION_GRAB,	// 掴みモーション
		MOTION_THROW,	// 投げモーション
		MOTION_STAMP,	// 踏みつけモーション
		MOTION_MAX
	};
	struct SFragMotion
	{
		bool bMove;	// 移動
		bool bJump;	// ジャンプ
		bool bStamp;	// 踏みつけ
		bool bShot;	// 射撃
		bool bMelee;	// 近接攻撃
		bool bAddAttack;	// 追加攻撃
		bool bGrab;	// 掴み
		bool bAir;	// 滞空
		bool bStop;	// 急停止
	};
	struct SInfo
	{
		float fLife;	// 体力
		STATE state;	// 状態
		CCollisionSphere *pCollisionSphere;	// 球の当たり判定
		CCollisionSphere *pClsnAttack;	// 攻撃の当たり判定
		CCollisionCube *pCollisionCube;	// 立方体の当たり判定
		bool bLand;	// 着地しているかどうか
		CEnemy *pEnemyGrab;	// 掴んでいる敵
		D3DXVECTOR3 rotDest;	// 目標の向き
	};

	void Lockon(void);
	void Input(void);
	void InputMove(void);
	void Stamp(void);
	void InputCamera(void);
	void InputAttack(void);
	void Rotation(void);
	void ManageCollision(void);
	void ManageState(void);
	void ManageMotion(void);
	void StartMelee(void);
	void AddMoveForward(float fSpeed);
	void AddMoveUp(float fSpeed);
	void Event(EVENT_INFO *pEventInfo);
	void Shot(D3DXVECTOR3 posMazzle);
	void ManageAttack(D3DXVECTOR3 pos,float fRadius);
	CEnemy *GetLockOn(void);
	void EndMelee(void);
	void Debug(void);

	SInfo m_info;	// 自身の情報
	SParam m_param;	// パラメーター情報
	SFragMotion m_fragMotion;	// モーションのフラグ

	static CPlayer *m_pPlayer;	// 自身のポインタ
};

#endif