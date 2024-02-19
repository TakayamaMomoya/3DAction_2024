//*****************************************************
//
// ���U���g��Behavior[resultBehavior.h]
// Author:���R����
//
//*****************************************************

#ifndef _RESULTBEHAVIOR_H_
#define _RESULTBEHAVIOR_H_

//*****************************************************
// �O���錾
//*****************************************************
class CResult;
class CNumber;

//*****************************************************
// �N���X�̒�`
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
		TYPE_ALL = 0,	// ���z
		TYPE_ADD,	// ���Z��V
		TYPE_REPAIR,	// �C����
		TYPE_RESULT,	// ���z
		TYPE_MAX
	};

	CResultPlayerScore();
	~CResultPlayerScore() override;

private:
	void Init(CResult *pResult) override;
	void Update(CResult *pResult) override;

	CNumber *m_apNumber[TYPE_MAX];	// ���l
};

#endif