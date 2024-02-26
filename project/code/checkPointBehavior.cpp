//*****************************************************
//
// チェックポイントのBehavior[checkpointBehavior.h]
// Author:高山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "checkPointBehavior.h"
#include "checkPointManager.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{

}

//=====================================================
// コンストラクタ
//=====================================================
CCheckPointBehavior::CCheckPointBehavior()
{

}

//=====================================================
// デストラクタ
//=====================================================
CCheckPointBehavior::~CCheckPointBehavior()
{

}

//=====================================================
// 移動パート
//=====================================================
CCheckPointMove::CCheckPointMove()
{// コンストラクタ

}

CCheckPointMove::~CCheckPointMove()
{// デストラクタ

}

void CCheckPointMove::Init(CCheckPointManager *pCheckPoint)
{// 初期化

}

void CCheckPointMove::Uninit(CCheckPointManager *pCheckPoint)
{// 終了

}

void CCheckPointMove::Update(CCheckPointManager *pCheckPoint)
{// 更新

}

//=====================================================
// 戦闘状態
//=====================================================
CCheckPointBattle::CCheckPointBattle()
{// コンストラクタ
	ZeroMemory(&m_apLimit[0], sizeof(m_apLimit));
}

CCheckPointBattle::~CCheckPointBattle()
{// デストラクタ

}

void CCheckPointBattle::Init(CCheckPointManager *pCheckPoint)
{// 初期化

}

void CCheckPointBattle::Uninit(CCheckPointManager *pCheckPoint)
{// 終了
	Object::DeleteObject((CObject**)&m_apLimit[0], CheckPointBehavior::NUM_LIMIT);
}

void CCheckPointBattle::Update(CCheckPointManager *pCheckPoint)
{// 更新

}