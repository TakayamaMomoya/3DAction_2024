//*****************************************************
//
// 3D円の処理[fan3D.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _FAN3D_H_
#define _FAN3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "fan.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CFan3D	: public CFan
{
public:
	CFan3D(int nPriority = 6);
	~CFan3D();	//	デストラクタ

	static CFan3D *Create(int nPriority = 3,int nNumVtx = 16);	// 生成処理
	HRESULT Init(void) override;	// 初期化処理
	void Uninit(void) override;	// 終了処理
	void Update(void) override;	// 更新処理
	void Draw(void) override;	// 描画処理
	void DrawNormal(void);	// 通常描画
	void DrawBillboard(void);	// ビルボード描画
	void SetVtx(void) override;	// 頂点設定

private:
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	bool m_bBillboard;	// ビルボードかどうか
};

#endif