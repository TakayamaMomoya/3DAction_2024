//*****************************************************
//
// 2D円の処理[fan2D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN2D_H_
#define _FAN2D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "fan.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan2D	: public CFan
{
public:
	CFan2D(int nPriority = 7);
	~CFan2D();	//	デストラクタ

	static CFan2D *Create(int nPriority = 7,int nNumVtx = 16);	// 生成処理
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;	// 終了処理
	void Update(void) override;	// 更新処理
	void Draw(void) override;	// 描画処理
	void SetVtx(void) override;	// 頂点設定

private:
};

#endif