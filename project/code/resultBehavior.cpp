//*****************************************************
//
// リザルトのBehavior[resultBehavior.cpp]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "resultBehavior.h"
#include "number.h"
#include "result.h"
#include "saveDataManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const int NUM_PLACE = 6;

const D3DXVECTOR3 POS_NUMBER[CResultPlayerScore::TYPE_MAX] =
{// 初期表示スコアの数値の位置
	{ SCRN_MID.x, SCRN_MID.y - 170.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 30.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 120.0f, 0.0f },
	{ SCRN_MID.x, SCRN_MID.y + 280.0f, 0.0f },
};

const D3DXVECTOR2 SIZE_NUMBER = { 25.0f,50.0f };	// 数字のサイズ

const int COST_REPAIR = 50;	// 損傷率1%における修理費用
}

//=====================================================
// コンストラクタ
//=====================================================
CResultBehavior::CResultBehavior()
{

}

//=====================================================
// デストラクタ
//=====================================================
CResultBehavior::~CResultBehavior()
{

}

//=====================================================
// プレイヤースコア表示
//=====================================================
CResultPlayerScore::CResultPlayerScore()
{// コンストラクタ
	ZeroMemory(&m_apNumber[0], sizeof(m_apNumber));
}

CResultPlayerScore::~CResultPlayerScore()
{// デストラクタ
	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{
		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->Uninit();
			m_apNumber[i] = nullptr;
		}
	}
}

void CResultPlayerScore::Init(CResult *pResult)
{// 初期化
	int nBaseReward = Result::BASE_REWARD;
	int nRepair = 0;
	int nAddReward = 0;

	// セーブデータの取得
	CSaveDataManager *pSave = CSaveDataManager::GetInstance();

	if (pSave == nullptr)
		assert(("セーブデータの取得ができません", false));

	CSaveDataManager::SInfo info = *pSave->GetInfo();

	nAddReward = info.nAddReward;

	// 修理費用計算
	float fRate = info.fLife / info.fInitialLife;

	fRate = (1.0f - fRate) * 100.0f;

	nRepair = COST_REPAIR * (int)fRate;

	// 総額
	int nResult = nBaseReward + nAddReward - nRepair;

	int aValue[TYPE_NUMBER::TYPE_MAX] =
	{// 値の保存用
		nBaseReward,nAddReward,nRepair,nResult
	};

	for (int i = 0; i < TYPE_NUMBER::TYPE_MAX; i++)
	{// 数字の生成
		m_apNumber[i] = CNumber::Create(NUM_PLACE, 0);

		if (m_apNumber[i] != nullptr)
		{
			m_apNumber[i]->SetSizeAll(SIZE_NUMBER.x, SIZE_NUMBER.y);
			m_apNumber[i]->SetPosition(POS_NUMBER[i]);
			m_apNumber[i]->SetValue(aValue[i],NUM_PLACE);
		}
	}
}

void CResultPlayerScore::Update(CResult *pResult)
{// 更新

}