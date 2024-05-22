//*****************************************************
//
// パーツの処理[parts.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PARTS_H_
#define _PARTS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_TEX	(5)	// テクスチャの数

//*****************************************************
// クラス定義
//*****************************************************
class CParts : public CObjectX
{
public:
	CParts();	// コンストラクタ
	~CParts();	// デストラクタ

	static CParts *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosOrg(void) { return m_posOrg; }
	void SetPosOrg(D3DXVECTOR3 pos) { m_posOrg = pos; }

private:
	D3DXVECTOR3 m_posOrg;								//最初の位置
};

#endif