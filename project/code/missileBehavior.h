//*****************************************************
//
// ミサイルのビヘイビア[missileBehavior.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _MISSILEBEHAVIOR_H_
#define _MISSILEBEHAVIOR_H_

//****************************************************
// 前方宣言
//****************************************************
class CMissile;

//****************************************************
// クラスの定義
//****************************************************
// 基底クラス
class CMissileBehavior
{
public:
	CMissileBehavior();
	virtual ~CMissileBehavior();

	virtual void Init(CMissile *pMissile) = 0;
	virtual void Uninit(CMissile *pMissile) = 0;
	virtual void Update(CMissile *pMissile) = 0;

private:
};

class CMissileLine : public CMissileBehavior
{
public:
	CMissileLine();
	virtual ~CMissileLine();

	void Init(CMissile *pMissile) override;
	void Uninit(CMissile *pMissile) override;
	void Update(CMissile *pMissile) override;

private:
};

#endif