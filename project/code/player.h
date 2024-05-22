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
#include <iostream>
#include <list>

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CObject3D;
class CEnemy;
class CHeat;
class CBoostEffect;
class COrbit;
class CPlayerController;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayer : public CMotion
{
public:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機
		MOTION_WALK_FRONT,	// 前歩
		MOTION_SHOT,	// 射撃
		MOTION_JUMP,	// ジャンプ
		MOTION_AIR,	// 滞空
		MOTION_FALL,	// 降下
		MOTION_ASSAULT,	// 突進状態
		MOTION_MELEE,	// 近接攻撃
		MOTION_MELEE2,	// ２回目の近接攻撃
		MOTION_STOP,	// 急停止モーション
		MOTION_GRAB,	// 掴みモーション
		MOTION_THROW,	// 投げモーション
		MOTION_STAMP,	// 踏みつけモーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_NEUTRAL_TITLE,	// タイトル待機
		MOTION_LAUNCH,	// 出撃
		MOTION_DODGE,	// 回避
		MOTION_APPER,	// 出現
		MOTION_MAX
	};
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_BLOW,	// 吹き飛ばされている状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};
	enum STATEBOOST
	{
		STATEBOOST_NONE = 0,	// 何でもない状態
		STATEBOOST_NORMAL,	// 通常状態
		STATEBOOST_OVERHEAT,	// オーバーヒート状態
		STATEBOOST_MAX
	};
	enum TYPE_CONTROLLER
	{
		TYPE_MOVE = 0,	// 移動
		TYPE_CAMERA,	// カメラ
		TYPE_ATTACK,	// 攻撃
		TYPE_MAX
	};
	enum PARAM
	{
		PARAM_GUN = 0,	// 銃の熱
		PARAM_MELEE,	// 近接武器の熱
		PARAM_MAX
	};
	struct SDamageQuake
	{
		float fLineMin;	// 揺らす最小ダメージ
		float fLineMax;	// 揺らす最大ダメージ
		float fPowQuake;	// 揺らす強さ
		int nFrameQuake;	// 揺れるフレーム数
	};
	struct SParam
	{
		std::string pathBody;	// 見た目のパス
		float fInitialLife;	// 初期体力
		float fSpeedMove;		// 移動速度
		float fFactMove;	// 移動量の減衰係数
		float fInitialBoost;		// ブースト残量初期値
		float fRegenBoost;	// ブーストの回復量
		float fGravity;	// 重力の大きさ
		float fSpeedBullet;	// 弾の速度
		float fDamageBullet;	// 弾の威力
		float fAccuracyBullet;	// 弾の精度
		float fPowJump;	// ジャンプ力
		float fSpeedDodge;	// 回避の速度
		float fPowStamp;	// 踏みつけの垂直推進力
		float fSpeedStamp;	// 踏みつけの水平推進力
		float fPowAddMelee;	// 追撃時の推進力
		float fPowGrab;	// 掴みの推進力
		float fRadiusGrab;	// 掴みの判定の半径
		float fPowThrow;	// 投げの力
		float fLengthLockOn;	// ロックオン距離
		float fDistMelee;	// 突進から攻撃にうつるまでの距離
		float fTimeRegenParam;	// 各攻撃のパラメーターの回復にかかる時間
		float fSizeHitEffect;	// 格闘攻撃ヒットエフェクトのサイズ
	};
	struct SInfoThruster
	{
		CBoostEffect *pFire;	// 噴射炎
		int nIdxParent;	// 親パーツ番号
		D3DXVECTOR3 offset;	// オフセット
		D3DXVECTOR3 vector;	// 噴射ベクトル
		D3DXVECTOR2 size;	// サイズ
		int nRot;	// 方向ID
	};
	struct SInfo
	{
		float fLife;	// 体力
		float fBoost;	// ブースト残量
		float aParam[PARAM_MAX];	// パラメータ
		bool aHeat[PARAM_MAX];	// オーバーヒートフラグ
		CHeat *apHeatUI[PARAM_MAX];	// パラメータ
		STATE state;	// 状態
		STATEBOOST stateBoost;	// ブーストの状態
		CCollisionSphere *pCollisionSphere;	// 球の当たり判定
		CCollisionSphere *pClsnAttack;	// 攻撃の当たり判定
		CCollisionCube *pCollisionCube;	// 立方体の当たり判定
		SInfoThruster *pThruster;	// スラスター情報
		int nNumThruster;	// スラスターの数
		bool bLockTarget;	// ターゲットロックするかどうか
		bool bLand;	// 着地しているかどうか
		bool bMelee;	// 近接攻撃が当たったかどうか
		CEnemy *pEnemyGrab;	// 掴んでいる敵
		D3DXVECTOR3 rotDest;	// 目標の向き
		COrbit *pOrbitWeapon;	// 武器の軌跡
		CPlayerController *apController[TYPE_CONTROLLER::TYPE_MAX];	// コントローラー配列
	};
	struct SFragMotion
	{
		bool bMove;	// 移動
		bool bJump;	// ジャンプ
		bool bDodge;	// 回避
		bool bStamp;	// 踏みつけ
		bool bShot;	// 射撃
		bool bMelee;	// 近接攻撃
		bool bAddAttack;	// 追加攻撃
		bool bGrab;	// 掴み
		bool bAir;	// 滞空
		bool bFall;	// 落下
		bool bStop;	// 急停止
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
	bool IsTargetLock(void) { return m_info.bLockTarget; }
	void EnableLock(bool bLock) { m_info.bLockTarget = bLock; }
	float GetBoost(void) { return m_info.fBoost; }
	float GetLife(void) { return m_info.fLife; }
	SParam GetParam(void) { return m_param; }
	STATEBOOST GetStateBoost(void) { return m_info.stateBoost; }
	bool IsLock(void) { return m_info.bLockTarget; }
	SInfo *GetInfo(void) { return &m_info; }
	SFragMotion *GetFragMotion(void) { return &m_fragMotion; }
	void AddMoveStamp(void);
	void AddBoost(float fValue);
	void AddMoveForward(float fSpeed);
	void Stamp(void);
	void ToggleLockTarget(void);
	void SwitchLockEnemy(void);
	void CreateController(TYPE_CONTROLLER type);
	void DeleteController(TYPE_CONTROLLER type);

private:
	void Load(void);
	void Lockon(void);
	void Input(void);
	void Rotation(void);
	void ManageCollision(void);
	void ManageState(void);
	void ManageMotion(void);
	void ManageParam(void);
	void Boost(void);
	void StartMelee(void);
	void AddMoveUp(float fSpeed);
	void Event(EVENT_INFO *pEventInfo);
	void Shot(D3DXVECTOR3 posMazzle);
	void ManageAttack(D3DXVECTOR3 pos,float fRadius);
	CEnemy *GetLockOn(void);
	void EndMelee(void);
	void Death(void);
	void Debug(void);

	SInfo m_info;	// 自身の情報
	SParam m_param;	// パラメータ情報
	SFragMotion m_fragMotion;	// モーションのフラグ
	std::list<SDamageQuake> m_listDamageQuake;	// 被弾時のカメラ揺れパラメーター
	static CPlayer *m_pPlayer;	// 自身のポインタ
};

#endif