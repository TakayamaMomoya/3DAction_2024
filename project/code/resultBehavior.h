//*****************************************************
//
// リザルトのBehavior[resultBehavior.h]
// Author:高山桃也
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// 前方宣言
//*****************************************************
class CResult;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CResultBehavior
{
public:
	CResultBehavior();
	virtual ~CResultBehavior();

	virtual void Init(CResult *pResult) = 0;
	virtual void Update(CResult *pResult) = 0;
private:

};

class CResultPlayerScore : public CResultBehavior
{
public:
	enum TYPE_NUMBER
	{
		TYPE_ALL = 0,	// 総額
		TYPE_ADD,	// 加算報酬
		TYPE_REPAIR,	// 修理費
		TYPE_RESULT,	// 総額
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CResult *pResult) override;
	void Update(CResult *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// 数値
};

#endif