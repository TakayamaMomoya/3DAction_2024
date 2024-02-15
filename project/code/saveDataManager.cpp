//*****************************************************
//
// セーブデータ管理[saveDataManager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "saveDataManager.h"
#include "checkPointManager.h"
#include <stdio.h>

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const char* SAVE_PATH = "data\\TEXT\\save.txt";	// セーブデータのパス
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CSaveDataManager *CSaveDataManager::m_pSaveDataManager = nullptr;	// 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CSaveDataManager::CSaveDataManager()
{
	m_nProgress = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CSaveDataManager::~CSaveDataManager()
{

}

//=====================================================
// 生成処理
//=====================================================
CSaveDataManager *CSaveDataManager::Create(void)
{
	if (m_pSaveDataManager == nullptr)
	{
		m_pSaveDataManager = new CSaveDataManager;

		if (m_pSaveDataManager != nullptr)
		{
			m_pSaveDataManager->Init();
		}
	}

	return m_pSaveDataManager;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSaveDataManager::Init(void)
{
	Load();

	return S_OK;
}

//=====================================================
// 読込
//=====================================================
void CSaveDataManager::Load(void)
{
	// 変数宣言
	char cTemp[256];
	int nCntPos = 0;

	// ファイルから読み込む
	FILE *pFile = fopen(SAVE_PATH, "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			(void)fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "PROGRESS") == 0)
			{// 進行状況読込
				(void)fscanf(pFile, "%s", &cTemp[0]);

				(void)fscanf(pFile, "%d", &m_nProgress);
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}

//=====================================================
// 終了処理
//=====================================================
void CSaveDataManager::Uninit(void)
{
	m_pSaveDataManager = nullptr;

	delete this;
}

//=====================================================
// 保存
//=====================================================
void CSaveDataManager::Save(void)
{
	CCheckPointManager *pCheck = CCheckPointManager::GetInstance();

	if (pCheck == nullptr)
		assert(("保存失敗！",false));

	int nProgress = pCheck->GetProgress();

	FILE *pFile = nullptr;

	pFile = fopen(SAVE_PATH, "w");

	if (pFile != nullptr)
	{
		// 進行状況
		fprintf(pFile, "PROGRESS = %d\n", nProgress);

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);
	}
}