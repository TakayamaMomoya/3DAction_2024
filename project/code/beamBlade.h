//*****************************************************
//
// ビームブレードの処理[beamBlade.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BEAMBLADE_H_
#define _BEAMBLADE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CObject3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CBeamBlade : public CObject
{
public:
	CBeamBlade(int nPriority = 3);	// コンストラクタ
	~CBeamBlade();	// デストラクタ

	static CBeamBlade *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	struct SInfo
	{
		CMeshCylinder *pCylinder;
		CObject3D *pBlade;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	SInfo m_info;
};

#endif