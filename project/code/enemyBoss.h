//*****************************************************
//
// ボス敵の処理[enemyboss.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_

#include "enemy.h"

class CStateBoss;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyBoss : public CEnemy
{
public:
	enum MOTION
	{// モーション
		MOTION_APPER = 0,	// 出現モーション
		MOTION_MISSILE,	// ミサイルモーション
		MOTION_DASH,	// 突進モーション
		MOTION_SHOT_UNDER,	// 射撃モーション
		MOTION_SHOT_UPPER,	// 反転射撃モーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_MAX
	};

	CEnemyBoss();	// コンストラクタ
	~CEnemyBoss();	// デストラクタ

	static CEnemyBoss *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemyBoss *GetInstance(void) { return m_pEnemyBoss; }
	void Hit(float fDamage);
	void ChangeState(CStateBoss *pNext);
	void Event(EVENT_INFO *pEventInfo);

private:
	enum IDXPARTS
	{// パーツの番号
		IDX_WAIST = 0,	// 腰
		IDX_BODY,	// 胴
		IDX_HEAD,	// 頭
		IDX_SHOULDER_L,	// 左肩
		IDX_ARM_L,	// 左腕
		IDX_HAND_L,	// 左手
		IDX_SHOULDER_R,	// 右肩
		IDX_ARM_R,	// 右手
		IDX_HAND_R,	// 右手
		IDX_LEG_L,	// 左腿
		IDX_FOOT_L,	// 左脛
		IDX_TOE_L,	// 左足先
		IDX_LEG_R,	// 右腿
		IDX_FOOT_R,	// 右脛
		IDX_TOE_R,	// 右足先
		IDX_WEAPON,	// 武器
		IDX_MAX
	};
	struct Sinfo
	{// 自身の情報
		CStateBoss *pState;	// 状態
		D3DXVECTOR3 posDest;	// 目標位置
		int nCntAttack;	// 攻撃カウンター
		int nNumAttack;	// 攻撃した回数
		int nCntState;	// 状態遷移カウンター
	};

	void ManageCollision(void);
	void FollowCollision(void);
	bool FollowDest(void);

	static CEnemyBoss *m_pEnemyBoss;	// 自身のポインタ
	Sinfo m_info;	// 自身の情報
};

#endif
