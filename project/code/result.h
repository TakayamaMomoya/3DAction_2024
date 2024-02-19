//*****************************************************
//
// リザルトの処理[result.h]
// Author:高山桃也
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "scene.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace Result
{
const int BASE_REWARD = 100000;	// 基本報酬
}

//*****************************************************
// 前方宣言
//*****************************************************
class CMeshCylinder;
class CResultBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CResult : public CScene
{
public:
	
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeBehavior(CResultBehavior *pBehavior);

private:
	CResultBehavior *m_pBehavior;
	CMeshCylinder *m_pCylinder;	// 背景のシリンダー
};

#endif