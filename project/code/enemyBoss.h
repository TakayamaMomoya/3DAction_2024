//*****************************************************
//
// ボス敵の処理[enemyBoss.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYBOSS_H_
#define _ENEMYBOSS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyBoss : public CEnemy
{
public:
	CEnemyBoss();	// コンストラクタ
	~CEnemyBoss();	// デストラクタ

	static CEnemyBoss *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChaseTarget(void);

private:
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_WALK,	// 歩きモーション
		MOTION_ATTACK,	// 攻撃モーション
		MOTION_DEATH,	// 死亡モーション
		MOTION_MAX
	};

	void Death(void);
	
	static CEnemyBoss *m_pEnemyBoss;	// 自身のポインタ
};

#endif
