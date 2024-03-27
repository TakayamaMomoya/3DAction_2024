//*****************************************************
//
// 2D円の処理[fan2D.h]
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

	virtual HRESULT Init(void);	// 初期化処理
	virtual void Uninit(void);	// 終了処理
	virtual void Update(void);	// 更新処理
	virtual void Draw(void);	// 描画処理

	void SetPosition(D3DXVECTOR3 pos);	// 設定処理
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 取得処理
	void SetRotation(D3DXVECTOR3 rot);// 向き設定処理
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// 向き取得処理
	D3DXCOLOR GetCol(void) { return m_col; }
	void SetCol(D3DXCOLOR col);
	virtual void SetVtx(void) = 0;
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }
	int GetIdxTexture(void) { return m_nIdxTexture; }
	void SetRateAngle(float fAngle) { m_fRateAngle = fAngle; }
	float GetRateAngle(void) { return m_fRateAngle; }
	float GetRadius(void) { return m_fRadius; }
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	void SetAngleMax(float fAngle) { m_fAngleMax = fAngle; }
	float GetMaxAngle(void) { return m_fAngleMax; }
	int GetNumVtx(void) { return m_nNumVtx; }
	void SetNumVtx(int nValue) { m_nNumVtx = nValue; }
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