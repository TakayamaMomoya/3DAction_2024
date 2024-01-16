//*****************************************************
//
// 通常敵の処理[enemyNormal.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemyNormal.h"
#include "manager.h"
#include "collision.h"
#include "debugproc.h"
#include "particle.h"
#include <stdio.h>
#include "player.h"
#include "game.h"
#include "enemyManager.h"
#include "debrisSpawner.h"
#include "block.h"
#include "motion.h"

//=====================================================
// コンストラクタ
//=====================================================
CEnemyNormal::CEnemyNormal()
{

}

//=====================================================
// デストラクタ
//=====================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyNormal::Init(void)
{
	// 継承クラスの初期化
	CEnemy::Init();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyNormal::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyNormal::Update(void)
{
	// 継承クラスの更新
	CEnemy::Update();

	CBlock *pBlock = GetTouchBlock();

	if (pBlock != nullptr)
	{
		// 生存チェック
		CBlock **ppBlock = CBlock::GetBlock();

		for (int i = 0; i < NUM_OBJECT; i++)
		{
			if (ppBlock[i] == pBlock)
			{
				pBlock->Hit(5.0f);
			}
		}
	}

	CEnemy::STATE state = GetState();

	if (state == CEnemy::STATE::STATE_DEATH)
	{// 死亡モーション
		int nMotion = GetMotion();

		if (nMotion != MOTION_DEATH)
		{
			SetMotion(MOTION_DEATH);
		}
	}
	else
	{

	}
}

//=====================================================
// 目標の追跡
//=====================================================
void CEnemyNormal::ChaseTarget(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CEnemyNormal::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}