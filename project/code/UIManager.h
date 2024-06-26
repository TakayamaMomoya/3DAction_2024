//*****************************************************
//
// UIマネージャー[UIManager.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;

//*****************************************************
// 定数定義
//*****************************************************

//*****************************************************
// クラスの定義
//*****************************************************
class CUIManager : public CObject
{
public:

	CUIManager();	// コンストラクタ
	~CUIManager();	// デストラクタ

	static CUIManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	void CreateFrame(void);
	void ManageFrame(void);

	float m_fCntFrame;	// フレームのカウンター
	bool m_bDisp;	// 表示するかどうか
	CUI *m_pCockpit;	// コックピットっぽいやつ
	CUI *m_pFrame;	// フレーム
	static CUIManager *m_pUIManager;	// 自身のポインタ
};

#endif
