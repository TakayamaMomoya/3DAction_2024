//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MISSILE_H_
#define _MISSILE_H_

#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMissile : public CEnemy
{// プレイヤーに向かう敵
public:
	CMissile();	// コンストラクタ
	~CMissile();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMissile *Create(D3DXVECTOR3 pos);
	void Hit(float fDamage) { Death(); }
	void HitField(void) override;
	void SetChaseSpeed(float fSpeed) { m_fChaseSpeed = fSpeed; }

private:
	void ChasePlayer(void);	// 追跡処理
	void Tilt(void);	// 傾く処理
	void Death(void);	// 死亡処理

	float m_fTimerHit;	// 着弾タイマー
	int m_nDeathTimer;	// 死亡カウンター
	bool m_bEndChase;	// 追尾終了フラグ
	float m_fChaseSpeed;	// 追尾性能
};

#endif
