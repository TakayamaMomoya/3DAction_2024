//*****************************************************
//
// プレイヤー操作[playercontroller.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerController
{// コントローラー基底クラス
public:
	CPlayerController();	// コンストラクタ
	~CPlayerController();	// デストラクタ

	virtual void Update(CPlayer *pPlayer) = 0;

private:
};

class CPlayerControllerMove : public CPlayerController
{// 移動操作クラス
public:
	CPlayerControllerMove();	// コンストラクタ
	~CPlayerControllerMove();	// デストラクタ

	void Update(CPlayer *pPlayer) override;

private:
};

class CPlayerControllerCamera : public CPlayerController
{// カメラ操作クラス
public:
	CPlayerControllerCamera();	// コンストラクタ
	~CPlayerControllerCamera();	// デストラクタ

	void Update(CPlayer *pPlayer) override;

private:
};

class CPlayerControllerAttack : public CPlayerController
{// 攻撃操作クラス
public:
	CPlayerControllerAttack();	// コンストラクタ
	~CPlayerControllerAttack();	// デストラクタ

	void Update(CPlayer *pPlayer) override;

private:
};

#endif