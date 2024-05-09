//*****************************************************
//
// レーダーの処理[rader.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RADER_H_
#define _RADER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "meshcylinder.h"

//*****************************************************
// クラス定義
//*****************************************************
class CRader : public CMeshCylinder
{
public:
	CRader(void);	// コンストラクタ
	~CRader();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRader *Create(void);

private:
	void ChaseTarget(void);
	void Wave(void);

	D3DXVECTOR3 *m_pPosInitial;	// 頂点の初期位置
};
#endif