//*****************************************************
//
//セーブデータ管理[saveDataManager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SAVEDATAMANAGER_H_
#define _SAVEDATAMANAGER_H_

#include "main.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSaveDataManager
{
public:
	CSaveDataManager();	// コンストラクタ
	~CSaveDataManager();	// デストラクタ

	static CSaveDataManager *Create(void);
	static CSaveDataManager *GetInstance(void) { return m_pSaveDataManager; }
	HRESULT Init(void);
	void Uninit(void);
	int GetProgress(void) { return m_nProgress; }
	void Save(void);
	void Load(void);

private:
	int m_nProgress;	// 現在の進行状況
	static CSaveDataManager *m_pSaveDataManager;	// 自身のポインタ
};

#endif