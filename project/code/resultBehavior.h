//*****************************************************
//
// UgΜBehavior[resultBehavior.h]
// Author:Rη
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// OϋιΎ
//*****************************************************
class CResult;
class CNumber;

//*****************************************************
// NXΜθ`
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
		TYPE_ALL = 0,	// z
		TYPE_ADD,	// ΑZρV
		TYPE_REPAIR,	// Cο
		TYPE_RESULT,	// z
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CResult *pResult) override;
	void Update(CResult *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// l
};

#endif