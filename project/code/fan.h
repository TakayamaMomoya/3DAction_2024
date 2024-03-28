//*****************************************************
//
// 扇ポリゴンの処理[fan.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN_H_
#define _FAN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan	: public CObject
{
public:
	CFan(int nPriority = 6);
	~CFan();	//	デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	virtual void SetVtx(void) = 0;

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }

	void SetRotation(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	void SetCol(D3DXCOLOR col);
	D3DXCOLOR GetCol(void) { return m_col; }

	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }

	void SetRateAngle(float fAngle) { m_fRateAngle = fAngle; }
	float GetRateAngle(void) { return m_fRateAngle; }

	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius(void) { return m_fRadius; }

	void SetAngleMax(float fAngle) { m_fAngleMax = fAngle; }
	float GetAngleMax(void) { return m_fAngleMax; }

	void SetNumVtx(int nValue) { m_nNumVtx = nValue; }
	int GetNumVtx(void) { return m_nNumVtx; }

	LPDIRECT3DVERTEXBUFFER9 *GetVtxBuff(void) { return &m_pVtxBuff; }

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXCOLOR m_col;	// 色
	float m_fRadius;	// 半径
	float m_fRateAngle;	// 角度の割合
	float m_fAngleMax;	// 最大角度
	int m_nNumVtx;	// 外周の頂点数
	int m_nIdxTexture;	// テクスチャの番号
};

#endif